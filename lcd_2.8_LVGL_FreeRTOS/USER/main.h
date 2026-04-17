#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f4xx.h"
#include "ws2812.h"
#include "bluetooth.h"
#include "esp8266.h"
#include "esp8266_mqtt.h"
#include "mic.h"
#define ESP_BUF_SIZE 512

void USART3_Init_Safe(uint32_t baudrate);
void usart3_send_str(char *str);
void usart3_send_bytes(uint8_t *buf, uint32_t len);

extern uint8_t RX_Command;
extern uint8_t RX_Flag;
extern uint8_t current_mode; 
extern uint8_t  g_esp8266_rx_buf[ESP_BUF_SIZE];
extern uint32_t g_esp8266_rx_cnt;
extern uint32_t g_esp8266_rx_end;



#endif
