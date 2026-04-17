#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_

#include "main.h"
#include <stdarg.h>

void BT_State_Pin_Init(void);
void Bluetooth_P4_Init(uint32_t baudrate);
void USART1_SendString(char *str);
void USART1_IRQHandler(void);
void BT_Printf(char *fmt, ...);

extern volatile uint8_t RX_Command;
extern volatile uint8_t RX_Flag; 

#endif
