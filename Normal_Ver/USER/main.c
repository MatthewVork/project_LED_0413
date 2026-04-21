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

// =====================================================================
// [全局变量区]
// =====================================================================
#define ESP_BUF_SIZE 512

uint8_t current_mode = 0;   
uint8_t rainbow_offset = 0; 
uint8_t meteor_pos = 0;  

// =====================================================================
// [6] 终极大本营：主函数逻辑
// =====================================================================
int main(void) {
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    SystemInit();
    Delay_Init();
    
    // �?件底层初始化
    WS2812_Init();
    Audio_ADC_Init();
    Usart1_init(9600); 
    Usart3_init(115200); 
    BT_State_Pin_Init();     
    
    // 这里调用你之前的 OneNET初始化函数    
    esp8266_mqtt_init();   

    uint8_t last_bt_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3); 
    
    // 开机自检蓝色
    WS2812_Fill(0, 0, 50); 
    printf("\r\nSystem Ready! 60 LEDs Online!\r\n");
    
    while(1) {
        // -------------------------------------------------------------
        // [控制�? 1]：蓝牙状态机与单字�?��?�理
        uint8_t current_bt_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3);
        if (last_bt_state == 1 && current_bt_state == 0) {
            WS2812_BlinkRed_Twice(); current_mode = 0; 
        } else if (last_bt_state == 0 && current_bt_state == 1) {
            WS2812_BlinkGreen_Twice();
        }
        last_bt_state = current_bt_state; 

        if(RX_Flag) {
            printf(">> Bluetooth Command: %c\r\n", RX_Command);
            switch(RX_Command) {
                case '1': current_mode = 1; WS2812_Fill(255, 0, 0); break; 
                case '2': current_mode = 2; WS2812_Fill(0, 255, 0); break; 
                case '3': current_mode = 3; WS2812_Fill(0, 0, 255); break; 
                case '4': current_mode = 4; break; 
                case '5': current_mode = 5; meteor_pos = 0;   break; 
                case '6': current_mode = 6; break; 
                case '7': current_mode = 7; break; 
                case '8': current_mode = 8; break; 
                case '0': current_mode = 0; WS2812_Fill(0, 0, 0);   break; 
            }
            RX_Flag = 0; 
        }

        // -------------------------------------------------------------
        // [收件�? A]：OneNET 云�??指令 (采用你的原版：净化防乱码 + �?动回�?)
        if(g_esp8266_rx_cnt > 0) {
            delay_ms(30);  // 等数�?收完
            
            // 🌟 你的神技：跳过前面的 MQTT 二进制乱码，直接定位�? JSON 的开�?
            char *json_start = NULL;
            for (int i = 0; i < g_esp8266_rx_cnt; i++) {
                if (g_esp8266_rx_buf[i] == '{') { 
                    json_start = (char *)&g_esp8266_rx_buf[i];
                    break;
                }
            }

            // 如果成功抓到了干净�? JSON 数据
            if (json_start != NULL) {
                printf("\r\n>> 成功过滤乱码！纯净 JSON 如下:\r\n%s\r\n", json_start);
                
                // 1. 🌟 你的神技：精准提取流水号 ID
                char msg_id[20] = {0};
                char *id_p = strstr(json_start, "\"id\":"); 
                if(id_p) {
                    id_p += 5; 
                    while(*id_p == ' ' || *id_p == '\"') id_p++; 
                    for(int i = 0; i < 19; i++) {
                        if(id_p[i] == '\"' || id_p[i] == ',' || id_p[i] == '}') break; 
                        msg_id[i] = id_p[i];
                    }
                }

                // 2. 执�?�动作，并打上标志位 (同�?�你现有的模�?)
                uint8_t cmd_executed = 0;
                if(strstr(json_start, "\"WorkMode\":0"))       { current_mode = 0; printf(">> 云�??执�??: 关灯\r\n"); WS2812_Fill(0, 0, 0); cmd_executed = 1; }
                else if(strstr(json_start, "\"WorkMode\":1"))  { current_mode = 1; printf(">> 云�??执�??: 模式1(�?)\r\n"); WS2812_Fill(255, 0, 0); cmd_executed = 1; }
                else if(strstr(json_start, "\"WorkMode\":2"))  { current_mode = 2; printf(">> 云�??执�??: 模式2(�?)\r\n"); WS2812_Fill(0, 255, 0); cmd_executed = 1; }
                else if(strstr(json_start, "\"WorkMode\":3"))  { current_mode = 3; printf(">> 云�??执�??: 模式3(�?)\r\n"); WS2812_Fill(0, 0, 255); cmd_executed = 1; }
                else if(strstr(json_start, "\"WorkMode\":5"))  { current_mode = 5; printf(">> 云�??执�??: 流星\r\n"); meteor_pos = 0; cmd_executed = 1; }
                else if(strstr(json_start, "\"WorkMode\":8"))  { current_mode = 8; printf(">> 云�??执�??: 声控律动\r\n"); cmd_executed = 1; }

                // 3. 终极回信：动作执行成功且抓到�? ID，立刻发货！
                if(cmd_executed && msg_id[0] != '\0') {
                    mqtt_reply_set_command(msg_id); // 完美调用你原版的回码函数
                    printf(">> 已向 OneNET 回�?�确认信�?(ID: %s)\r\n", msg_id);
                }
            } 

            // 处理完毕，清空防爆护盾缓�?
            memset((void *)g_esp8266_rx_buf, 0, ESP_BUF_SIZE); 
            g_esp8266_rx_cnt = 0;
            g_esp8266_rx_end = 0;
        }

        // -------------------------------------------------------------
        // [动画执�?�] 状态机非阻塞调�?
        switch(current_mode) {
            case 4: WS2812_Rainbow_Step(rainbow_offset++); delay_ms(20); break;
            case 5: WS2812_Meteor_Step(&meteor_pos); delay_ms(30); break;
            case 6: WS2812_Breathing_Step(NULL); delay_ms(10); break;
            case 7: WS2812_Fire_Step(); delay_ms(50); break;
            case 8: WS2812_Audio_Sync_Step(); break;
            default: delay_ms(20); break; 
        }
    }
}