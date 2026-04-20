#include "delay.h"
#include "FreeRTOS.h"
#include "task.h"
#include "core_cm4.h"   // for __get_IPSR()

// 168MHz / 8 = 21MHz → 1us ≈ 21 ticks
static uint32_t fac_us = 21;

// 判断当前是否在中断中
static inline int in_isr(void)
{
    return (__get_IPSR() != 0);
}

void delay_Init(void)
{
    // 仅用于裸机阶段的计时基准（RTOS 启动后不依赖它）
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    SysTick->LOAD = 0xFFFFFF;
    SysTick->VAL  = 0;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

// ⚠️ 仅用于“短延时”（如 WS2812、几十微秒级别）
void delay_us(uint32_t nus)
{
    uint32_t ticks, told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;

    ticks = nus * fac_us;
    told  = SysTick->VAL;

    while (tcnt < ticks)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told) tcnt += told - tnow;
            else             tcnt += reload - tnow + told;

            told = tnow;
        }
    }
}

// ms 延时：根据运行环境自动选择
void delay_ms(uint32_t ms)
{
    // RTOS 已启动 且 不在中断中 → 用 vTaskDelay（不占CPU）
    if ((xTaskGetSchedulerState() == taskSCHEDULER_RUNNING) && !in_isr())
    {
        // 0ms 直接让出一次调度
        if (ms == 0)
        {
            taskYIELD();
            return;
        }

        // 直接一次性延时，避免 for 循环
        vTaskDelay(pdMS_TO_TICKS(ms));
    }
    else
    {
        // 裸机 或 中断中 → 只能忙等（但尽量短）
        while (ms--)
        {
            delay_us(1000);
        }
    }
}

void delay_s(uint32_t s)
{
    // 不再循环 1000 次 delay_ms，直接走 RTOS 延时（更高效）
    delay_ms(s * 1000);
}