#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_

#include "main.h"

void BT_State_Pin_Init(void);
void Bluetooth_P4_Init(uint32_t baudrate);
void USART1_SendString(char *str);
void USART1_IRQHandler(void);

extern uint8_t RX_Command;
extern uint8_t RX_Flag; 

#endif