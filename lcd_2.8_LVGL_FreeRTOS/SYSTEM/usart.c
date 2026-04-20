#include "usart.h"
#include "main.h"

// ==========================================================
// [外部变量声明区] 
// ==========================================================
// 1. 蓝牙指令接收变量 (定义在 main.c)
extern uint8_t RX_Command;
extern uint8_t RX_Flag;

// 2. ESP8266 WiFi 接收变量 (定义在 main.c 或 esp8266.c)
extern uint8_t  g_esp8266_rx_buf[ESP_BUF_SIZE]; 
extern uint32_t g_esp8266_rx_cnt;               
extern uint32_t  g_esp8266_rx_end;               

// ==========================================================
// [重定向区] 禁用半主机模式，printf 专供 USART1
// ==========================================================
#pragma import(__use_no_semihosting)             
struct __FILE { int handle; }; 
FILE __stdout;       
int _sys_exit(int x) { x = x; return 0; } 

int fputc(int ch, FILE *f)
{     
    USART_SendData(USART1, (uint8_t)ch);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);     
    return ch;
}

// ==========================================================
// [初始化区] USART1 (PA9/PA10) - 蓝牙与打印
// ==========================================================
void Usart1_init(u32 BaudRate)
{
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
    
    USART_InitStructure.USART_BaudRate = BaudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    
    // 中断优先级配置 (Group2: 抢占 1, 响应 1)
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    USART_Cmd(USART1, ENABLE);
}

// ==========================================================
// [初始化区] USART3 (PB10/PB11) - ESP8266 WiFi
// ==========================================================
void Usart3_init(u32 BaudRate)
{
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
    
    USART_InitStructure.USART_BaudRate = BaudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);
    
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    
    // 中断优先级配置 (Group2: 抢占 1, 响应 2 -> 优先级略低于蓝牙)
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;        
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    USART_Cmd(USART3, ENABLE);
}

// ==========================================================
// [中断服务区] USART1 - 处理蓝牙指令
// ==========================================================
void USART1_IRQHandler(void)                
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
	{
        // 1. 先把接收到的字符暂存下来，别急着放进信箱
		uint8_t temp_char = USART_ReceiveData(USART1); 
        
        // 2. 🚀 核心防线：只允许数字 '0' 到 '9' 丢进信箱！
        // 彻底屏蔽手机 APP 自动加的 \r, \n, 空格等无效字符
        if (temp_char >= '0' && temp_char <= '9') 
        {
            RX_Command = temp_char; // 放进信箱
            RX_Flag = 1;            // 举起旗子通知保安
        }

		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}

// ==========================================================
// [中断服务区] USART3 - 处理 ESP8266 数据
// ==========================================================
void USART3_IRQHandler(void)
{
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        uint8_t res = USART_ReceiveData(USART3);
        
        // 存入 ESP8266 缓冲区
        if(g_esp8266_rx_cnt < ESP_BUF_SIZE)
        {
            g_esp8266_rx_buf[g_esp8266_rx_cnt++] = res;
        }
        
        // 如果你需要在这里判断换行符或者定时器超时来置位 g_esp8266_rx_end，可以在这里加逻辑
        // g_esp8266_rx_end = 1; 
        
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    }
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