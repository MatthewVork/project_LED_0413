#include "stm32f4xx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ************************* 硬件配置区 *************************
#define LED_COUNT    60
#define RESET_PULSES 60 

uint8_t RX_Command = 0;
uint8_t RX_Flag = 0; 

uint16_t T1H, T0H, TIM_PERIOD;
uint16_t g_ledDataBuffer[LED_COUNT * 24 + RESET_PULSES]; 

// =====================================================================
// [1] 系统精确延时
// =====================================================================
static void DWT_Init(void) {
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CYCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}
static void Delay_ms(uint32_t ms) {
  uint32_t clk = SystemCoreClock / 1000;
  uint32_t cnt = DWT->CYCCNT;
  while(DWT->CYCCNT - cnt < ms * clk);
}

// =====================================================================
// [2] 蓝牙 STATE 引脚监测初始化 (P5_RXD -> PA3)
// =====================================================================
void BT_State_Pin_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

// =====================================================================
// [3] 蓝牙/电脑通信串口 USART1 (P4接口: PA9->TX, PA10->RX)
// =====================================================================
void Bluetooth_P4_Init(uint32_t baudrate) {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART1, ENABLE);
}

void USART1_SendString(char *str) {
    while (*str) {
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        USART_SendData(USART1, *str++);
    }
}

void USART1_IRQHandler(void) {
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        uint8_t incoming_byte = USART_ReceiveData(USART1); 
        if(incoming_byte != '\r' && incoming_byte != '\n' && incoming_byte != ' ') {
            RX_Command = incoming_byte; 
            RX_Flag = 1;                
        }
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}

// =====================================================================
// [4] WS2812B 核心驱动 (PA8 -> TIM1)
// =====================================================================
void WS2812_Clear(void) {
  for(uint16_t i=0; i<(24*LED_COUNT); i++) g_ledDataBuffer[i] = T0H;
  for(uint16_t i=(24*LED_COUNT); i<(24*LED_COUNT + RESET_PULSES); i++) g_ledDataBuffer[i] = 0; 
}

void WS2812_SetPixelColor(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
  if(index >= LED_COUNT) return;
  uint32_t color = ((uint32_t)g << 16) | ((uint32_t)r << 8) | b;
  uint16_t* p = &g_ledDataBuffer[index * 24];
  for(int i = 0; i < 24; i++) p[i] = ((color >> (23 - i)) & 0x01) ? T1H : T0H;
}

void WS2812_Update(void) {
  TIM_Cmd(TIM1, DISABLE); 
  DMA_ClearFlag(DMA2_Stream1, DMA_FLAG_TCIF1 | DMA_FLAG_HTIF1 | DMA_FLAG_TEIF1 | DMA_FLAG_DMEIF1 | DMA_FLAG_FEIF1);
  DMA_SetCurrDataCounter(DMA2_Stream1, 24 * LED_COUNT + RESET_PULSES);
  DMA_Cmd(DMA2_Stream1, ENABLE);
  TIM_Cmd(TIM1, ENABLE);
  while(!DMA_GetFlagStatus(DMA2_Stream1, DMA_FLAG_TCIF1));
  DMA_ClearFlag(DMA2_Stream1, DMA_FLAG_TCIF1);
  DMA_Cmd(DMA2_Stream1, DISABLE);
  TIM_Cmd(TIM1, DISABLE);
}

void WS2812_Fill(uint8_t r, uint8_t g, uint8_t b) {
  for(int i=0; i<LED_COUNT; i++) WS2812_SetPixelColor(i, r, g, b);
  WS2812_Update();
}

void WS2812_Init(void) {
  TIM_PERIOD = (SystemCoreClock / 800000) - 1;  
  T1H = (uint16_t)(SystemCoreClock * 0.8e-6);    
  T0H = (uint16_t)(SystemCoreClock * 0.4e-6);    

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_TimeBaseStructure.TIM_Period = TIM_PERIOD;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  TIM_OCInitTypeDef TIM_OCInitStructure;
  TIM_OCStructInit(&TIM_OCInitStructure);
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC1Init(TIM1, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
  
  TIM_DMACmd(TIM1, TIM_DMA_CC1, ENABLE);
  TIM_CtrlPWMOutputs(TIM1, ENABLE);

  DMA_InitTypeDef DMA_InitStructure;
  DMA_DeInit(DMA2_Stream1);
  DMA_InitStructure.DMA_Channel = DMA_Channel_6;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(TIM1->CCR1);
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)g_ledDataBuffer;
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_Init(DMA2_Stream1, &DMA_InitStructure);
  
  DMA_Cmd(DMA2_Stream1, DISABLE);
  TIM_Cmd(TIM1, DISABLE);

  WS2812_Clear(); 
}

// =====================================================================
// [5] 动画特效组件区
// =====================================================================
void WS2812_BlinkRed_Twice(void) {
    for(int i = 0; i < 2; i++) {
        WS2812_Fill(255, 0, 0); Delay_ms(200);          
        WS2812_Fill(0, 0, 0);   Delay_ms(200);          
    }
}
void WS2812_BlinkGreen_Twice(void) {
    for(int i = 0; i < 2; i++) {
        WS2812_Fill(0, 255, 0); Delay_ms(200);
        WS2812_Fill(0, 0, 0); Delay_ms(200);
    }
}
void WS2812_SetWheelColor(uint8_t index, uint8_t pos) {
    pos = 255 - pos;
    uint8_t r, g, b;
    if(pos < 85) { r = 255 - pos * 3; g = 0; b = pos * 3; } 
    else if(pos < 170) { pos -= 85; r = 0; g = pos * 3; b = 255 - pos * 3; } 
    else { pos -= 170; r = pos * 3; g = 255 - pos * 3; b = 0; }
    WS2812_SetPixelColor(index, r, g, b);
}
void WS2812_Rainbow_Step(uint8_t offset) {
    for(int i = 0; i < LED_COUNT; i++) {
        uint8_t color_pos = ((i * 256 / LED_COUNT) + offset) & 0xFF;
        WS2812_SetWheelColor(i, color_pos);
    }
    WS2812_Update();
}
void WS2812_Meteor_Step(uint8_t *pos) {
    WS2812_Clear();
    for(int j = 0; j < 10; j++) { 
        int p = (*pos) - j;
        if(p >= 0 && p < LED_COUNT) {
            uint8_t brightness = 255 - (j * 25); 
            WS2812_SetPixelColor(p, brightness/2, brightness/2, 255); 
        }
    }
    WS2812_Update();
    (*pos)++;
    if(*pos >= LED_COUNT + 10) *pos = 0; 
}
void WS2812_Breathing_Step(uint16_t *angle) {
    static int8_t dir = 1;
    static uint8_t val = 0;
    WS2812_Fill(0, val, val); 
    val += dir;
    if(val == 255 || val == 0) dir = -dir; 
}
void WS2812_Fire_Step(void) {
    for(int i = 0; i < LED_COUNT; i++) {
        uint8_t r = 200 + (rand() % 55);
        uint8_t g = rand() % 80;
        WS2812_SetPixelColor(i, r, g, 0);
    }
    WS2812_Update();
}

// =====================================================================
// [6] 麦克风 ADC 采集 (适配 Camera 排针第 14 脚: PA6 -> ADC1_IN6)
// =====================================================================
void Audio_ADC_Init(void) {
    GPIO_InitTypeDef  GPIO_InitStructure;
    ADC_InitTypeDef   ADC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    // 配置 PA6 为模拟输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN; 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 

    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;      
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;               
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;         
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;      
    ADC_InitStructure.ADC_NbrOfConversion = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_Cmd(ADC1, ENABLE);
}

uint16_t Get_Audio_Raw(void) {
    // 对应 ADC 通道 6
    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_480Cycles);
    
    ADC_SoftwareStartConv(ADC1);           
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)); 
    return ADC_GetConversionValue(ADC1);   
}

// =====================================================================
// [高级丝滑版] 条状拾音律动灯 (带重力回落效果)
// =====================================================================
void WS2812_Audio_Sync_Step(void) {
    uint16_t volume = Get_Audio_Raw();
    
    // 【阈值配置】依然根据你串口看到的实际数值来定
    uint16_t noise_floor = 400;  
    uint16_t max_volume = 2500;  
    
    int target_leds = 0; 
    
    // 1. 计算当前瞬间“本应该”亮几颗灯
    if (volume > noise_floor) {
        if (volume > max_volume) volume = max_volume; 
        target_leds = ((volume - noise_floor) * LED_COUNT) / (max_volume - noise_floor);
    }
    
    // ---------------------------------------------------------
    // 🌟 核心去闪烁魔法：引入“虚拟平滑灯柱”
    // ---------------------------------------------------------
    static float smooth_leds = 0; // 必须是 static，记住上一帧的位置
    
    if (target_leds > smooth_leds) {
        // 声音变大：允许瞬间爆发冲上去，为了视觉更有弹性，做一点微小的缓冲
        smooth_leds = smooth_leds * 0.2 + target_leds * 0.8;                                                                                
    } 
    else {
        // 声音变小：拒绝瞬间熄灭！让它以固定的速度“掉”下来
        smooth_leds -= 2; // 【调节这里】：数字越小（比如 0.5），掉得越慢越柔和；数字越大，掉得越快。
        
        if (smooth_leds < 0) {
            smooth_leds = 0; // 触底反弹归零
        }
    }
    
    int display_leds = (int)smooth_leds; // 最终实际要亮的灯数
    // ---------------------------------------------------------
    
    WS2812_Clear(); 
    
    for(int i = 0; i < display_leds; i++) {
        if (i < LED_COUNT / 3) {
            WS2812_SetPixelColor(i, 0, 100, 0);       // 绿 (稍微把亮度调到了200，不那么刺眼)
        } 
        else if (i < (LED_COUNT * 2) / 3) {
            WS2812_SetPixelColor(i, 100, 100, 0);     // 黄
        } 
        else {
            WS2812_SetPixelColor(i, 100, 0, 0);       // 红
        }
    }
    
    WS2812_Update();
    
    // 帧率控制，20ms 相当于一秒刷新 50 次，人眼看着非常流畅
    Delay_ms(10); 
} 

// =====================================================================
// [7] 主函数逻辑
// =====================================================================
int main(void) {
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  SystemInit();
  DWT_Init();
  
  WS2812_Init();
  Audio_ADC_Init();
  Bluetooth_P4_Init(9600); 
  BT_State_Pin_Init();     
  
  uint8_t last_bt_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3); // 开机读取初始状态
  uint8_t current_mode = 0;   
  uint8_t rainbow_offset = 0; 
  uint8_t meteor_pos = 0;  

  uint32_t last_print_time = DWT->CYCCNT;
  char print_buf[64];

  WS2812_Fill(10, 10, 10); 
  
  while(1) {
      // -------------------------------------------------------------
      // 每 0.5 秒打印一次，直接看最真实的裸数据！
      if (DWT->CYCCNT - last_print_time >= SystemCoreClock / 2) { 
          last_print_time = DWT->CYCCNT; 
          
          // 不循环了！不相减了！直接读一针见血的原始数据！
          uint16_t real_volume = Get_Audio_Raw();
          
          sprintf(print_buf, "Raw Volume: %d\r\n", real_volume);
          USART1_SendString(print_buf);
      }
      
      // -------------------------------------------------------------
      // 监测蓝牙状态引脚 (PA3)
      // -------------------------------------------------------------
      uint8_t current_bt_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3);
      if (last_bt_state == 1 && current_bt_state == 0) {
          WS2812_BlinkRed_Twice(); 
          current_mode = 0; 
      }
      else if (last_bt_state == 0 && current_bt_state == 1) {
          WS2812_BlinkGreen_Twice();
      }
      last_bt_state = current_bt_state; 

      // -------------------------------------------------------------
      // 处理蓝牙指令
      // -------------------------------------------------------------
      if(RX_Flag) {
          switch(RX_Command)
          {
              case '1': current_mode = 1; WS2812_Fill(255, 0, 0); break; 
              case '2': current_mode = 2; WS2812_Fill(0, 255, 0); break; 
              case '3': current_mode = 3; WS2812_Fill(0, 0, 255); break; 
              case '4': current_mode = 4; break; 
              case '5': current_mode = 5; meteor_pos = 0;   break; 
              case '6': current_mode = 6; break; 
              case '7': current_mode = 7; break; 
              case '8': current_mode = 8; break; // 发送 '8' 进入声控律动模式
              case '0': current_mode = 0; WS2812_Fill(0, 0, 0);   break; 
          }
          RX_Flag = 0; 
      }
      
      // -------------------------------------------------------------
      // 动画状态机执行
      // -------------------------------------------------------------
      switch(current_mode) {
          case 4: WS2812_Rainbow_Step(rainbow_offset++); Delay_ms(20); break;
          case 5: WS2812_Meteor_Step(&meteor_pos); Delay_ms(30); break;
          case 6: WS2812_Breathing_Step(NULL); Delay_ms(10); break;
          case 7: WS2812_Fire_Step(); Delay_ms(50); break;
          case 8: WS2812_Audio_Sync_Step(); /* 函数内已有延时，无需额外Delay */ break;
          default: Delay_ms(20); break;
      }
  }
}