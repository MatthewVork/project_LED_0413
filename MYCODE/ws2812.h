#ifndef _WS2812_H_
#define _WS2812_H_
#define LED_COUNT    60
#define RESET_PULSES 60 

#include "main.h"

void WS2812_Init(void);
void WS2812_Audio_Sync_Step(void);
void WS2812_Clear(void);
void WS2812_SetPixelColor(uint8_t index, uint8_t r, uint8_t g, uint8_t b);
void WS2812_Update(void);
void WS2812_Fill(uint8_t r, uint8_t g, uint8_t b);
void WS2812_BlinkRed_Twice(void);
void WS2812_BlinkGreen_Twice(void);
void WS2812_SetWheelColor(uint8_t index, uint8_t pos);
void WS2812_Rainbow_Step(uint8_t offset);
void WS2812_Meteor_Step(uint8_t *pos);
void WS2812_Breathing_Step(uint16_t *angle);
void WS2812_Fire_Step(void);

#endif