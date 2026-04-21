#include "ws2812.h"

uint16_t T1H, T0H, TIM_PERIOD;

// 🚀 强行写死：60颗灯的 16位数据缓存区 (外加 60 个复位脉冲)
uint16_t g_ledDataBuffer[60 * 24 + 60]; 

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
  // 还原老版本的上拉，确保硬件时序一致
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
  // 还原老版本的 HalfWord
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

// 🚀 强行写死：关 60 颗灯
void WS2812_Clear(void) {
  for(uint16_t i=0; i<(24*60); i++) g_ledDataBuffer[i] = T0H;
  for(uint16_t i=(24*60); i<(24*60 + 60); i++) g_ledDataBuffer[i] = 0; 
}

// 🚀 强行写死：上限 60 颗
void WS2812_SetPixelColor(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
  if(index >= 60) return;
	
	r = (uint8_t)(r * global_brightness);
  g = (uint8_t)(g * global_brightness);
  b = (uint8_t)(b * global_brightness);
	
  uint32_t color = ((uint32_t)g << 16) | ((uint32_t)r << 8) | b;
  uint16_t* p = &g_ledDataBuffer[index * 24];
  for(int i = 0; i < 24; i++) p[i] = ((color >> (23 - i)) & 0x01) ? T1H : T0H;
}

// 🚀 强行写死：发 60 颗的数据包
void WS2812_Update(void) {
  TIM_Cmd(TIM1, DISABLE);
  DMA_ClearFlag(DMA2_Stream1, DMA_FLAG_TCIF1 | DMA_FLAG_HTIF1 | DMA_FLAG_TEIF1 | DMA_FLAG_DMEIF1 | DMA_FLAG_FEIF1);
  DMA_SetCurrDataCounter(DMA2_Stream1, 24 * 60 + 60); 
  DMA_Cmd(DMA2_Stream1, ENABLE);
  TIM_Cmd(TIM1, ENABLE);
  while(!DMA_GetFlagStatus(DMA2_Stream1, DMA_FLAG_TCIF1));
  DMA_ClearFlag(DMA2_Stream1, DMA_FLAG_TCIF1);
  DMA_Cmd(DMA2_Stream1, DISABLE);
  TIM_Cmd(TIM1, DISABLE);
}

// 🚀 强行写死：填满 60 颗
void WS2812_Fill(uint8_t r, uint8_t g, uint8_t b) {
  for(int i=0; i<60; i++) WS2812_SetPixelColor(i, r, g, b);
  WS2812_Update();
}

// =====================================================================
// [高级丝滑版] 条状拾音律动灯
// =====================================================================
void WS2812_Audio_Sync_Step(void) {
    uint16_t volume = Get_Audio_Raw();
    uint16_t noise_floor = 400;  
    uint16_t max_volume = 2500;  
    
    int target_leds = 0; 
    if (volume > noise_floor) {
        if (volume > max_volume) volume = max_volume; 
        target_leds = ((volume - noise_floor) * 60) / (max_volume - noise_floor); // 写死 60
    }
    
    static float smooth_leds = 0; 
    if (target_leds > smooth_leds) {
        smooth_leds = smooth_leds * 0.2 + target_leds * 0.8;                                                                                
    } else {
        smooth_leds -= 2; 
        if (smooth_leds < 0) smooth_leds = 0; 
    }
    
    int display_leds = (int)smooth_leds; 
    
    WS2812_Clear(); 
    
    for(int i = 0; i < display_leds; i++) {
        if (i < 20) {
            WS2812_SetPixelColor(i, 0, 100, 0);       // 0-19: 绿
        } 
        else if (i < 40) {
            WS2812_SetPixelColor(i, 100, 100, 0);     // 20-39: 黄
        } 
        else {
            WS2812_SetPixelColor(i, 100, 0, 0);       // 40-59: 红
        }
    }
    
    WS2812_Update();
} 

// =====================================================================
// 特效函数区 (全部写死 60)
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
    for(int i = 0; i < 60; i++) { // 写死 60
        uint8_t color_pos = ((i * 256 / 60) + offset) & 0xFF; // 写死 60
        WS2812_SetWheelColor(i, color_pos);
    }
    WS2812_Update();
}

void WS2812_Meteor_Step(uint8_t *pos) {
    WS2812_Clear();
    for(int j = 0; j < 10; j++) { 
        int p = (*pos) - j;
        if(p >= 0 && p < 60) { // 写死 60
            uint8_t brightness = 255 - (j * 25); 
            WS2812_SetPixelColor(p, brightness/2, brightness/2, 255); 
        }
    }
    WS2812_Update();
    (*pos)++;
    if(*pos >= 60 + 10) *pos = 0; // 写死 60
}

void WS2812_Breathing_Step(uint16_t *angle) {
    static int8_t dir = 1;
    static uint8_t val = 0;
    WS2812_Fill(0, val, val); 
    val += dir;
    if(val == 255 || val == 0) dir = -dir; 
}

void WS2812_Fire_Step(void) {
    for(int i = 0; i < 60; i++) { // 写死 60
        uint8_t r = 200 + (rand() % 55);
        uint8_t g = rand() % 80;
        WS2812_SetPixelColor(i, r, g, 0);
    }
    WS2812_Update();
}
