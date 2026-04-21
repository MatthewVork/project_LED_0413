#include "bluetooth.h"

// =====================================================================
// 蓝牙 STATE 引脚监测初始化 (P5_RXD -> PA3)
// =====================================================================
void BT_State_Pin_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}
// =====================================================================
// 蓝牙控制引脚初始化 (P5_TXD -> PA2)
// =====================================================================
void BT_EN_Control_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 开启 GPIOA 时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;             // PA2 即 TX2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;         // 设置为推挽输出
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;        // 改为下拉，确保默认不通电
    GPIO_Init(GPIOA, &GPIO_InitStructure);                // 执行初始化

    // 默认输出低电平，保证开机时蓝牙模块是断电不可搜索的
    GPIO_ResetBits(GPIOA, GPIO_Pin_2);
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