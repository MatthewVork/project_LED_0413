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

#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// FreeRTOS 头文件
#include "FreeRTOS.h"
#include "task.h"

// 硬件与业务头文件
#include "delay.h"
#include "usart.h"
#include "ws2812.h"
#include "esp8266.h"       
#include "esp8266_mqtt.h"
#include "mic.h"
#include "lcd.h"
#include "touch.h"
#include "tim.h"

// LVGL 头文件
#include "lvgl.h"
#include "lv_port_disp.h"
#include "semphr.h"
#include "timers.h"

// =====================================================================
// [全局变量区] - 情报中心与状态机
// =====================================================================
#include "lv_port_indev.h"
#include "..\ui\ui.h"

#define Off         0x00
#define White       0x01
#define Red         0x02
#define Yellow      0x03
#define Blue        0x04
#define Fire        0x05
#define Breath      0x06
#define Audio       0x07
#define Meteor      0x08
#define Rainbow     0x09

extern uint8_t RX_Command;
extern uint8_t RX_Flag;
extern uint8_t current_mode; 
extern uint8_t  g_esp8266_rx_buf[ESP_BUF_SIZE];
extern uint32_t g_esp8266_rx_cnt;
extern uint32_t g_esp8266_rx_end;
extern volatile float global_brightness;
extern volatile uint8_t bt_control_enabled;
extern volatile uint8_t bt_is_waiting;
extern uint32_t bt_start_tick;

extern void Trigger_Bluetooth_Toast(uint8_t is_connected);
extern void Trigger_BT_Disconnected(void);
extern void Trigger_BT_Success(void);
extern void Sync_LCD_Light_Panel(uint8_t mode);

#endif
