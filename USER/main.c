#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include "delay.h"
#include "usart.h"
#include "esp8266.h"
#include "esp8266_mqtt.h"
#include "mic.h"       
#include "ws2812.h"    

uint8_t current_mode = 0; 

int main(void) {	
    uint32_t delay_1ms_cnt = 0;
    uint8_t rainbow_offset = 0;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    Delay_Init();
    usart1_init(9600); // P4 蓝牙波特率
    Audio_ADC_Init();  
    WS2812_Init();     
    WS2812_Fill(0, 0, 50); // 开机自检：蓝色
	
		printf("\r\n==================================\r\n");
    printf("        系统上电，通道测试正常      \r\n");
    printf("   请在手机蓝牙助手发送 'go' 开始连网\r\n");
    printf("==================================\r\n");
    printf("\r\n?? OneNET + 蓝牙双控 系统启动\r\n");
	
		while(1)
    {
        if(g_usart1_rx_cnt > 0)
        {
            delay_ms(20); // 等数据收完
            char* rx_str = (char*)g_usart1_rx_buf;
            
            if(strstr(rx_str, "go"))
            {
                memset((void *)g_usart1_rx_buf, 0, sizeof(g_usart1_rx_buf));
                g_usart1_rx_cnt = 0;
                printf("\r\n?? 收到 'go' 指令，开始执行连网流程！\r\n");
                break; // ?? 打破死循环，正式放行往下走
            }
            
            memset((void *)g_usart1_rx_buf, 0, sizeof(g_usart1_rx_buf));
            g_usart1_rx_cnt = 0;
        }
    }

    while(esp8266_mqtt_init() != 0) {
        printf("?? 联网失败，正在重试...\r\n");
        delay_s(1);
    }
    printf("?? OneNET 在线成功！\r\n");	
    WS2812_Clear(); WS2812_Update();

    while(1) {
        // [收件箱 A]：OneNET 云端指令 (净化防乱码 + 自动回码版)
        if(g_esp8266_rx_cnt > 0) {
            delay_ms(30);  
            
            // ?? 核心修复：跳过前面的 MQTT 二进制乱码，直接定位到 JSON 的开头
            char *json_start = NULL;
            for (int i = 0; i < g_esp8266_rx_cnt; i++) {
                if (g_esp8266_rx_buf[i] == '{') { // 只要找到了 '{'，后面的就是纯净数据
                    json_start = (char *)&g_esp8266_rx_buf[i];
                    break;
                }
            }

            // 如果成功抓到了干净的 JSON 数据
            if (json_start != NULL) {
                printf("\r\n? 成功过滤乱码！纯净 JSON 如下:\r\n%s\r\n", json_start);
                
                // 1. ?? 提取流水号 ID
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

                // 2. ? 执行动作，并打上标志位
                uint8_t cmd_executed = 0;
                if(strstr(json_start, "\"WorkMode\":0"))      { current_mode = 0; printf("?? 云端执行: 关灯\r\n"); cmd_executed = 1; }
                else if(strstr(json_start, "\"WorkMode\":1"))  { current_mode = 1; printf("?? 云端执行: 律动\r\n"); cmd_executed = 1; }
                else if(strstr(json_start, "\"WorkMode\":2"))  { current_mode = 2; printf("?? 云端执行: 彩虹\r\n"); cmd_executed = 1; }
                else if(strstr(json_start, "\"WorkMode\":3"))  { current_mode = 3; printf("?? 云端执行: 火焰\r\n"); cmd_executed = 1; }

                // 3. ?? 终极回信：动作执行成功且抓到了 ID，立刻发货！
                if(cmd_executed && msg_id[0] != '\0') {
                    mqtt_reply_set_command(msg_id);
                    printf("? 已向 OneNET 回复确认信！(ID: %s)\r\n", msg_id);
                }
            } 

            memset((void *)g_esp8266_rx_buf, 0, 512); 
            g_esp8266_rx_cnt = 0;
        }

        // [收件箱 B]：蓝牙本地指令
        if(g_usart1_rx_cnt > 0) {
            delay_ms(20);  
            char* rx_str = (char*)g_usart1_rx_buf;
            if(strstr(rx_str, "off"))      { current_mode = 0; printf("?? 蓝牙: 关灯\r\n"); }
            else if(strstr(rx_str, "on"))  { current_mode = 1; printf("?? 蓝牙: 律动\r\n"); }
            else if(strstr(rx_str, "rbw")) { current_mode = 2; printf("?? 蓝牙: 彩虹\r\n"); }
            else if(strstr(rx_str, "fire")){ current_mode = 3; printf("?? 蓝牙: 火焰\r\n"); }
            memset((void *)g_usart1_rx_buf, 0, 512); g_usart1_rx_cnt = 0;
        }

        delay_ms(1); delay_1ms_cnt++;
        
        // 动画刷新 (20ms)
        if((delay_1ms_cnt % 20) == 0) {
            switch(current_mode) {
                case 0: WS2812_Clear(); WS2812_Update(); break;
                case 1: WS2812_Audio_Sync_Step(); break;
                case 2: WS2812_Rainbow_Step(rainbow_offset++); break;
                case 3: WS2812_Fire_Step(); break;
            }
        }
        
        // 6秒同步状态
        if((delay_1ms_cnt % 6000) == 0) { mqtt_report_devices_status(); }
        
        // 60秒保活心跳
        if((delay_1ms_cnt % 60000) == 0) { mqtt_send_heart(); }
    }
}