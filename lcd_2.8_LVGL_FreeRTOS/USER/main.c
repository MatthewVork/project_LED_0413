#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ws2812.h"
#include "esp8266.h"       
#include "esp8266_mqtt.h"
#include "delay.h"
#include "usart.h"
#include "mic.h"
// === 新增屏幕与 UI 头文件 ===
#include "lcd.h"
#include "touch.h"
#include "tim.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "..\ui\ui.h"
#include "main.h"

// =====================================================================
// [全局变量区]
// =====================================================================
#define ESP_BUF_SIZE 512

volatile uint8_t current_mode = 0;  // 保证编译器不乱优化的 volatile
volatile uint8_t input_flag = 0;    // 🚀 情报官：有新指令吗？(0:没有, 1:有)
volatile uint8_t input_source = 0;  // 🚀 追踪器：谁发的？(1:蓝牙, 2:OneNET, 3:触摸屏)

uint8_t rainbow_offset = 0; 
uint8_t meteor_pos = 0;

// 通讯变量
volatile uint8_t RX_Command;
volatile uint8_t RX_Flag;
volatile uint8_t  g_esp8266_rx_buf[ESP_BUF_SIZE]; 
volatile uint32_t g_esp8266_rx_cnt = 0;           
volatile uint32_t g_esp8266_rx_end = 0;  // 🚀 用这个替代你原来的 delay_ms(30) 等待

// 系统挂钟，给屏幕和动画供能
volatile uint32_t g_sys_tick = 0; 

int main(void) {
    // 1. 底层与时钟
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    // SystemInit(); // 启动文件已调，可省
    Delay_Init();
    
    // 2. 硬件外设初始化 (融合你的和屏幕的)
    WS2812_Init();
    Audio_ADC_Init();        // 恢复你的麦克风
    BT_State_Pin_Init();     // 恢复你的蓝牙状态引脚
    Usart1_init(9600); 
    Usart3_init(115200); 
    
    LCD_Init();           
    tp_dev.init();        
    
    // 3. 云端初始化
    printf("Connecting to OneNET...\r\n");
    esp8266_mqtt_init();  // 🚀 注意：这步比较慢，期间屏幕不会亮 
    
    // 4. UI 界面初始化
    lv_init();             
    lv_port_disp_init();   
    lv_port_indev_init();  
    Tim3_init(83, 999);    // 启动 TIM3 给系统提供心跳
    ui_init();             

    // 开机自检
    uint8_t last_bt_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3); 
    WS2812_Fill(0, 0, 50); 
    printf("\r\nSystem Ready! 60 LEDs & LVGL UI Online!\r\n");
    
    // 动画定时器记录
    uint32_t last_anim_time = 0;
    uint32_t anim_interval = 20; // 动态控制动画快慢
    
    while(1) 
    {
        // =============================================================
        // [最高优先级]：维持屏幕丝滑
        // =============================================================
        lv_task_handler(); 

        // =============================================================
        // [输入源 1]：蓝牙指令解析
        // =============================================================
        // ... (保留你原来的 PA3 状态引脚判断) ...

        if(RX_Flag) {
            switch(RX_Command) {
                case '1': current_mode = 1; break; 
                case '2': current_mode = 2; break; 
                case '3': current_mode = 3; break; 
                case '4': current_mode = 4; break; 
                case '5': current_mode = 5; meteor_pos = 0; break; 
                case '6': current_mode = 6; break; 
                case '7': current_mode = 7; break; 
                case '8': current_mode = 8; break; 
                case '0': current_mode = 0; break; 
            }
            input_source = 1; // 🚀 标记来源：1 代表蓝牙
            input_flag = 1;   // 🚀 触发情报中心
            RX_Flag = 0; 
        }

        // =============================================================
        // [输入源 2]：OneNET 云端指令 (包含 30ms 软断帧)
        // =============================================================
        if (g_esp8266_rx_cnt > 0 && g_esp8266_rx_end == 0) {
            static uint32_t rx_wait_time = 0;
            static uint32_t last_cnt = 0;
            if (g_esp8266_rx_cnt != last_cnt) {
                last_cnt = g_esp8266_rx_cnt;
                rx_wait_time = g_sys_tick; 
            } else if ((g_sys_tick - rx_wait_time) >= 30) {
                g_esp8266_rx_end = 1;  
                last_cnt = 0;          
            }
        }
                
        if (g_esp8266_rx_end == 1) { 
            char *json_start = NULL;
            for (int i = 0; i < g_esp8266_rx_cnt; i++) {
                if (g_esp8266_rx_buf[i] == '{') { json_start = (char *)&g_esp8266_rx_buf[i]; break; }
            }

            if (json_start != NULL) {
                // 提取 msg_id 用于回码
                char msg_id[20] = {0};
                char *id_p = strstr(json_start, "\"id\":"); 
                if(id_p) {
                    id_p += 5; 
                    while(*id_p == ' ' || *id_p == '\"' || *id_p == ':') id_p++; 
                    for(int i = 0; i < 19; i++) {
                        if(id_p[i] == '\"' || id_p[i] == ',' || id_p[i] == '}' || id_p[i] == ' ') break; 
                        msg_id[i] = id_p[i];
                    }
                }

                uint8_t cmd_executed = 0;
                if(strstr(json_start, "\"WorkMode\":0"))      { current_mode = 0; cmd_executed = 1; }
                else if(strstr(json_start, "\"WorkMode\":1")) { current_mode = 1; cmd_executed = 1; }
                else if(strstr(json_start, "\"WorkMode\":2")) { current_mode = 2; cmd_executed = 1; }
                else if(strstr(json_start, "\"WorkMode\":3")) { current_mode = 3; cmd_executed = 1; }
                else if(strstr(json_start, "\"WorkMode\":5")) { current_mode = 5; meteor_pos = 0; cmd_executed = 1; }
                else if(strstr(json_start, "\"WorkMode\":8")) { current_mode = 8; cmd_executed = 1; }

                if(cmd_executed) {
                    input_source = 2; // 🚀 标记来源：2 代表 OneNET
                    input_flag = 1;   // 🚀 触发情报中心
                    if(msg_id[0] != '\0') { mqtt_reply_set_command(msg_id); }
                }
            } 
            memset((void *)g_esp8266_rx_buf, 0, ESP_BUF_SIZE); 
            g_esp8266_rx_cnt = 0;
            g_esp8266_rx_end = 0; 
        }

        // =============================================================
        // [情报中心]：统一打印输入方式和模式信息 (你的核心架构需求)
        // =============================================================
        if (input_flag == 1) 
        {
            printf("\r\n------------------------------------\r\n");
            // 1. 打印输入方式
            if (input_source == 1)      printf("[指令来源] >>> 蓝牙端 (Bluetooth)\r\n");
            else if (input_source == 2) printf("[指令来源] >>> 云平台 (OneNET)\r\n");
            else if (input_source == 3) printf("[指令来源] >>> 触摸屏 (LVGL UI)\r\n");

            // 2. 打印对应的模式信息
            printf("[执行动作] >>> ");
            switch(current_mode) {
                case 0: printf("关闭氛围灯 (OFF)\r\n"); break;
                case 1: printf("切换为: 红色常亮\r\n"); break;
                case 2: printf("切换为: 绿色常亮\r\n"); break;
                case 3: printf("切换为: 蓝色常亮\r\n"); break;
                case 4: printf("切换为: 七彩跑马灯 (Rainbow)\r\n"); break;
                case 5: printf("切换为: 流星雨 (Meteor)\r\n"); break;
                case 6: printf("切换为: 呼吸灯 (Breathing)\r\n"); break;
                case 7: printf("切换为: 火焰特效 (Fire)\r\n"); break;
                case 8: printf("切换为: 音乐律动 (Audio Sync)\r\n"); break;
                default:printf("未知模式\r\n"); break;
            }
            printf("------------------------------------\r\n");

            // 3. 收起旗帜，等待下一次指令
            input_flag = 0;
            input_source = 0;
        }

        // =============================================================
        // [动画执行]：真正干活的底层渲染引擎 (不掺杂任何判断逻辑)
        // =============================================================
        if ((g_sys_tick - last_anim_time) >= anim_interval) 
        {
            last_anim_time = g_sys_tick; 
            switch(current_mode) {
                case 0: WS2812_Fill(0, 0, 0);                  anim_interval = 100; break; 
                case 1: WS2812_Fill(255, 0, 0);                anim_interval = 100; break; 
                case 2: WS2812_Fill(0, 255, 0);                anim_interval = 100; break; 
                case 3: WS2812_Fill(0, 0, 255);                anim_interval = 100; break; 
                case 4: WS2812_Rainbow_Step(rainbow_offset++); anim_interval = 20;  break;
                case 5: WS2812_Meteor_Step(&meteor_pos);       anim_interval = 30;  break;
                case 6: WS2812_Breathing_Step(NULL);           anim_interval = 10;  break;
                case 7: WS2812_Fire_Step();                    anim_interval = 50;  break;
                case 8: WS2812_Audio_Sync_Step();              anim_interval = 20;  break; 
                default: anim_interval = 100; break; 
            }
        }

        // =============================================================
        // [心跳保活]：每 10 秒向 OneNET 发送一次 PING，防止掉线
        // =============================================================
        static uint32_t last_heartbeat_time = 0;
        if ((g_sys_tick - last_heartbeat_time) >= 10000) 
        {
            last_heartbeat_time = g_sys_tick; 
            mqtt_send_heart();
            printf(">> [Heartbeat] MQTT Ping Sent to OneNET\r\n");
        }
    }
}
 
// ---------------------------------------------------------
// 硬件中断：LVGL 心跳 + 系统挂钟 (千万别删！)
// ---------------------------------------------------------
void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
    {
        lv_tick_inc(1);  
        g_sys_tick++; // 我们的超级定时器   
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}