#include "stm32f4xx.h"
#include "delay.h"
#include "usart.h"
#include "lcd.h"
#include "key.h"  
#include "touch.h" 
#include "tim.h"
#include "lvgl.h"
#include "led.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "..\ui\ui.h"

// 1. 如果有自定义字体或演示函数，放在这里
LV_FONT_DECLARE(my_font);

int main(void)
{
    // --- 第一步：地基初始化 (宪法级) ---
    // 统一使用 Group 2 (2位抢占，2位响应)，确保中断逻辑不打架
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 

    // --- 第二步：统一度量衡 ---
    Delay_Init();         // 🚀 初始化你刚换好的寄存器版延时
    Usart1_init(115200);   // 开启调试窗口
    Led_Init();           // LED 指示灯
    
    // --- 第三步：硬件初始化 ---
    // 因为 delay_ms 已经标准化，LCD 初始化将不再卡死
    LCD_Init();           
    // LCD_ShowString(30, 50, 200, 16, 16, "LCD Matrix Ready!"); // 验证 LCD 是否活着

    // --- 第四步：图形库初始化 ---
    lv_init();             // LVGL 核心初始化
    lv_port_disp_init();   // 注册显示接口
    lv_port_indev_init();  // 注册触摸接口
    
    // 开启 1ms 定时器提供 LVGL 系统心跳
    Tim3_init(83, 999);    
    
    // 初始化从 SquareLine Studio 导出的 UI 界面
    ui_init();             

    printf("\r\n>>> Bare-Metal System Started Success! <<<\r\n");

    // --- 第五步：终极裸机循环 ---
    while(1)
    {
        // 核心任务：维持 UI 刷新 (最高优先级调用)
        lv_task_handler(); 

        // 辅助任务：心跳指示灯 (每隔 500ms 闪烁一次)
        static uint32_t led_tick = 0;
        if(++led_tick > 100) { // 因为 handler 运行很快，这里用计数模拟
            GPIO_ToggleBits(GPIOF, GPIO_Pin_9);
            led_tick = 0;
        }

        // 给 CPU 极其微小的休眠，防止总线过热
        delay_ms(5); 
    }
}

// ---------------------------------------------------------
// 硬件中断：LVGL 的心跳来源 (每 1ms 进入一次)
// ---------------------------------------------------------
void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
    {
        // 向图形库报告系统时间已经过去 1ms
        lv_tick_inc(1);  
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}