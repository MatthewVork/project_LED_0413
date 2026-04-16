#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ws2812.h"
#include "esp8266.h"       
#include "esp8266_mqtt.h"  

// =====================================================================
// [全局变量区]
// =====================================================================
#define ESP_BUF_SIZE 512

uint8_t current_mode = 0;   
uint8_t rainbow_offset = 0; 
uint8_t meteor_pos = 0;  

// =====================================================================
// [1] 系统精确延时 (使用 DWT 周期计数器)
// =====================================================================
static void DWT_Init(void) {
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}
void Delay_ms(uint32_t ms) {
    uint32_t clk = SystemCoreClock / 1000;
    uint32_t cnt = DWT->CYCCNT;
    while(DWT->CYCCNT - cnt < ms * clk);
}

// =====================================================================
// [3] 电脑与蓝牙串口 USART1 (PA9, PA10) - 绝对安全版
// =====================================================================
void Bluetooth_P4_Init(uint32_t baudrate) {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART1, ENABLE);
}

int fputc(int ch, FILE *f) {
    while((USART1->SR & 0X40) == 0); 
    USART1->DR = (uint8_t) ch;
    return ch;
}

void USART1_IRQHandler(void) {
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        uint8_t incoming_byte = USART_ReceiveData(USART1); 
        if(incoming_byte != '\r' && incoming_byte != '\n' && incoming_byte != ' ') {
            RX_Command = incoming_byte; 
            RX_Flag = 1;                
        }
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}

// =====================================================================
// [4] 云端大脑串口 USART3 (PB10, PB11 连接 ESP8266) - 防溢出装甲版
// =====================================================================
void USART3_Init_Safe(uint32_t baudrate) {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART3, ENABLE);
}

void usart3_send_str(char *str) {
    while (*str) {
        while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
        USART_SendData(USART3, *str++);
    }
}
void usart3_send_bytes(uint8_t *buf, uint32_t len) {
    while(len--) {
        while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
        USART_SendData(USART3, *buf++);
    }
}

void USART3_IRQHandler(void) {
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
        uint8_t d = USART_ReceiveData(USART3);
        // 核心装甲：满 512 字节前允许存入，绝不越界踩踏灯带内存！
        if (g_esp8266_rx_cnt < ESP_BUF_SIZE - 1) {
            g_esp8266_rx_buf[g_esp8266_rx_cnt++] = d;
            g_esp8266_rx_buf[g_esp8266_rx_cnt] = '\0'; // 始终保持字符串结尾，方便 strstr 查找
        } else {
            g_esp8266_rx_end = 1; 
        }
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    }
}

// =====================================================================
// [6] 终极大本营：主函数逻辑
// =====================================================================
int main(void) {
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    SystemInit();
    DWT_Init();
    
    // 硬件底层初始化
    WS2812_Init();
    Audio_ADC_Init();
    Bluetooth_P4_Init(9600); 
    USART3_Init_Safe(115200); 
    BT_State_Pin_Init();     
    
    // 这里调用你之前的 OneNET / ESP8266 初始化函数
    esp8266_init();        
    esp8266_mqtt_init();   

    uint8_t last_bt_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3); 
    
    // 开机自检蓝色
    WS2812_Fill(0, 0, 50); 
    printf("\r\nSystem Ready! 60 LEDs Online!\r\n");
    
    while(1) {
        // -------------------------------------------------------------
        // [控制路 1]：蓝牙状态机与单字符处理
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
        // [收件箱 A]：OneNET 云端指令 (采用你的原版：净化防乱码 + 自动回码)
        if(g_esp8266_rx_cnt > 0) {
            Delay_ms(30);  // 等数据收完
            
            // 🌟 你的神技：跳过前面的 MQTT 二进制乱码，直接定位到 JSON 的开头
            char *json_start = NULL;
            for (int i = 0; i < g_esp8266_rx_cnt; i++) {
                if (g_esp8266_rx_buf[i] == '{') { 
                    json_start = (char *)&g_esp8266_rx_buf[i];
                    break;
                }
            }

            // 如果成功抓到了干净的 JSON 数据
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

                // 2. 执行动作，并打上标志位 (同步你现有的模式)
                uint8_t cmd_executed = 0;
                if(strstr(json_start, "\"WorkMode\":0"))       { current_mode = 0; printf(">> 云端执行: 关灯\r\n"); WS2812_Fill(0, 0, 0); cmd_executed = 1; }
                else if(strstr(json_start, "\"WorkMode\":1"))  { current_mode = 1; printf(">> 云端执行: 模式1(红)\r\n"); WS2812_Fill(255, 0, 0); cmd_executed = 1; }
                else if(strstr(json_start, "\"WorkMode\":2"))  { current_mode = 2; printf(">> 云端执行: 模式2(绿)\r\n"); WS2812_Fill(0, 255, 0); cmd_executed = 1; }
                else if(strstr(json_start, "\"WorkMode\":3"))  { current_mode = 3; printf(">> 云端执行: 模式3(蓝)\r\n"); WS2812_Fill(0, 0, 255); cmd_executed = 1; }
                else if(strstr(json_start, "\"WorkMode\":5"))  { current_mode = 5; printf(">> 云端执行: 流星\r\n"); meteor_pos = 0; cmd_executed = 1; }
                else if(strstr(json_start, "\"WorkMode\":8"))  { current_mode = 8; printf(">> 云端执行: 声控律动\r\n"); cmd_executed = 1; }

                // 3. 终极回信：动作执行成功且抓到了 ID，立刻发货！
                if(cmd_executed && msg_id[0] != '\0') {
                    mqtt_reply_set_command(msg_id); // 完美调用你原版的回码函数
                    printf(">> 已向 OneNET 回复确认信！(ID: %s)\r\n", msg_id);
                }
            } 

            // 处理完毕，清空防爆护盾缓冲
            memset((void *)g_esp8266_rx_buf, 0, ESP_BUF_SIZE); 
            g_esp8266_rx_cnt = 0;
            g_esp8266_rx_end = 0;
        }

        // -------------------------------------------------------------
        // [动画执行] 状态机非阻塞调度
        switch(current_mode) {
            case 4: WS2812_Rainbow_Step(rainbow_offset++); Delay_ms(20); break;
            case 5: WS2812_Meteor_Step(&meteor_pos); Delay_ms(30); break;
            case 6: WS2812_Breathing_Step(NULL); Delay_ms(10); break;
            case 7: WS2812_Fire_Step(); Delay_ms(50); break;
            case 8: WS2812_Audio_Sync_Step(); break;
            default: Delay_ms(20); break; 
        }
    }
}