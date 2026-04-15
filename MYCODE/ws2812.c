#include "ws2812.h"

uint16_t T1H, T0H, TIM_PERIOD;
uint16_t g_ledDataBuffer[LED_COUNT * 24 + RESET_PULSES]; 

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
    //delay_ms(10); 
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


// =====================================================================
// [5] 动画特效组件区
// =====================================================================
void WS2812_BlinkRed_Twice(void) {
    for(int i = 0; i < 2; i++) {
        WS2812_Fill(255, 0, 0); delay_ms(200);          
        WS2812_Fill(0, 0, 0);   delay_ms(200);          
    }
}
void WS2812_BlinkGreen_Twice(void) {
    for(int i = 0; i < 2; i++) {
        WS2812_Fill(0, 255, 0); delay_ms(200);
        WS2812_Fill(0, 0, 0); delay_ms(200);
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