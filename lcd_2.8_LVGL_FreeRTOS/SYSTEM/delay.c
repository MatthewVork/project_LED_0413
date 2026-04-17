#include "delay.h"
#include "FreeRTOS.h"
#include "task.h"

// 你的时钟是 168MHz/8 = 21MHz，所以 1us 需要 21 个时钟周期
static uint32_t fac_us = 21; 

void delay_Init(void)
{
    // ?? 恢复开机供氧！专门为了让 LCD_Init 等硬件初始化能正常跑完。
    // 等 FreeRTOS 启动后，会自动重写这里的配置，不用担心冲突。
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    SysTick->LOAD = 0xFFFFFF; // 随便给个足够大的重载值
    SysTick->VAL = 0x00;      // 清空计数器
    SysTick->CTRL |= 0x01;    // 开启定时器！
}

// 采用“只读不写”的安全延时
void delay_us(u32 nus)
{
    u32 ticks;
    u32 told, tnow, tcnt = 0;
    u32 reload;

    reload = SysTick->LOAD; 
    ticks = nus * fac_us;   
    told = SysTick->VAL;    

    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told) 
                tcnt += told - tnow;
            else 
                tcnt += reload - tnow + told;
            
            told = tnow;
            
            if (tcnt >= ticks) break; 
        }
    }
}

// 智能毫秒延时
void delay_ms(u32 nms)
{
    // 判断操作系统是否已经启动了？
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
        // 如果 OS 启动了，就用不占 CPU 的睡眠延时
        vTaskDelay(pdMS_TO_TICKS(nms));
    }
    else
    {
        // 如果 OS 还没启动（比如 LCD_Init 阶段），就老老实实用微秒死等
        delay_us(nms * 1000);
    }
}

void delay_s(u32 ns)
{
    for(int i=0; i<ns; i++)
    {
        delay_ms(1000);
    }
}