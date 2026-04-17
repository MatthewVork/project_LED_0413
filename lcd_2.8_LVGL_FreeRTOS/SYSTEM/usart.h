#ifndef __USART_H
#define __USART_H

#include "stm32f4xx.h"
#include "stdio.h"

// ==========================================================
// 串口 1 初始化 (复用引脚 PA9/PA10)
// 功能：printf 调试打印 + 蓝牙指令接收
// ==========================================================
void Usart1_init(u32 BaudRate);

// ==========================================================
// 串口 3 初始化 (复用引脚 PB10/PB11)
// 功能：ESP8266 WiFi 模块数据收发
// ==========================================================
void Usart3_init(u32 BaudRate);

void usart3_send_str(char *str);
void usart3_send_bytes(uint8_t *buf, uint32_t len);
void TIM3_IRQHandler(void);

#endif
