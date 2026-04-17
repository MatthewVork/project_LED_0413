#include "bluetooth.h"

// =====================================================================
// [2] 蓝牙 STATE 引脚监测初始化 (P5_RXD -> PA3)
// =====================================================================
void BT_State_Pin_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void USART1_SendString(char *str) {
    while (*str) {
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        USART_SendData(USART1, *str++);
    }
}

void BT_Printf(char *fmt, ...) {
    char bt_tx_buf[1024]; 
    va_list ap;
    
    va_start(ap, fmt);
    vsprintf(bt_tx_buf, fmt, ap);
    va_end(ap);
    
    // 调用串口1的底层发送，把信封扔给蓝牙模块
    USART1_SendString(bt_tx_buf); 
}