#include "main.h"

uint8_t current_mode = 0;  // 当前模式
uint8_t input_flag   = 0;  // 是否有新指令 (0:无, 1:有)
uint8_t input_source = 0;  // 追踪器：1:蓝牙, 2:OneNET, 3:触摸屏

volatile float global_brightness = 1.0f;
volatile uint8_t bt_control_enabled = 0;  // 🚀 蓝牙鉴权锁 (0:锁死, 1:放行)

uint8_t rainbow_offset = 0; 
uint8_t meteor_pos = 0;  

// 通讯缓冲变量
uint8_t RX_Command;
uint8_t RX_Flag;

uint8_t  g_esp8266_rx_buf[ESP_BUF_SIZE]; 
uint32_t g_esp8266_rx_cnt = 0;           
uint32_t g_esp8266_rx_end = 0;

volatile uint8_t bt_is_waiting = 0;    // 1: 正在等待手机连接蓝牙, 0: 闲置
uint32_t bt_start_tick = 0;            // 记录按下按钮的时间戳

// =====================================================================
// [FreeRTOS 句柄声明]
// =====================================================================
SemaphoreHandle_t xGuiMutex;            // UI 互斥锁

TaskHandle_t Task_GUI_Handle;           // 任务句柄：刷屏
TaskHandle_t Task_LED_Handle;           // 任务句柄：灯效
TaskHandle_t Task_Comms_Handle;         // 任务句柄：通讯

TimerHandle_t Timer_Heartbeat_Handle;   // 软件定时器句柄：心跳

// =====================================================================
// [任务函数声明]
// =====================================================================
void Task_GUI(void *pvParameters);
void Task_LED(void *pvParameters);
void Task_Comms(void *pvParameters);
void Timer_Heartbeat_Callback(TimerHandle_t xTimer);

// =====================================================================
// MAIN 函数：只负责创建任务并启动调度器
// =====================================================================
int main(void) 
{
    // 1. 底层与时钟 (FreeRTOS 必须使用 Group_4)
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    delay_Init();
    
    // 2. 硬件外设初始化
    WS2812_Init();
    Audio_ADC_Init();        
    BT_State_Pin_Init();
    BT_EN_Control_Init();     
    Usart1_init(9600); 
    Usart3_init(115200); 
    LCD_Init();           
    tp_dev.init();        
    
    // 3. UI 界面初始化
    lv_init();             
    lv_port_disp_init();   
    lv_port_indev_init();  
    Tim3_init(83, 999);    // 启动 TIM3，专门给 LVGL 供心跳 lv_tick_inc()
    ui_init();             

    // 4. 开机自检效果
    WS2812_Fill(0, 0, 50); // 亮微弱蓝光代表启动中
    printf("\r\n>>> System Hardware Ready! Starting FreeRTOS... <<<\r\n");

    // ==========================================================
    // FreeRTOS 资源创建
    // ==========================================================
    xGuiMutex = xSemaphoreCreateMutex();

    // 创建三大任务 (堆栈大小根据实际需求调整，GUI给最大，通讯次之，灯光最小)
    xTaskCreate(Task_GUI,   "Task_GUI",   1024, NULL, 2, &Task_GUI_Handle);
    xTaskCreate(Task_LED,   "Task_LED",   256,  NULL, 3, &Task_LED_Handle);
		xTaskCreate(Task_Comms, "Task_Comms", 512,  NULL, 4, &Task_Comms_Handle);

//    // 创建心跳定时器 (周期 10000ms = 10秒，自动重载 pdTRUE)
//    Timer_Heartbeat_Handle = xTimerCreate("Heartbeat", pdMS_TO_TICKS(10000), pdTRUE, 0, Timer_Heartbeat_Callback);
//    if(Timer_Heartbeat_Handle != NULL) {
//        xTimerStart(Timer_Heartbeat_Handle, 0); 
//    }

    // 启动调度器，接管控制权
    vTaskStartScheduler();
		
    while(1); // 永远跑不到这里
}

// =====================================================================
// 1：GUI 刷新任务 (加入 5 秒开机画面逻辑)
// =====================================================================
void Task_GUI(void *pvParameters)
{
    // 🚀 第一阶段：让初始化屏幕 (ui_Screen_Init) 运行 5 秒钟
    // 5000 毫秒 / 每次循环 5 毫秒 = 1000 次循环
    for(int i = 0; i < 1000; i++) 
    {
        if (xSemaphoreTake(xGuiMutex, portMAX_DELAY) == pdTRUE) {
            lv_timer_handler(); // 注意：LVGL 8.x 推荐用 lv_timer_handler
            xSemaphoreGive(xGuiMutex);
        }
        vTaskDelay(pdMS_TO_TICKS(5)); 
    }

    // 🚀 第二阶段：5 秒时间到，执行华丽切换！
    if (xSemaphoreTake(xGuiMutex, portMAX_DELAY) == pdTRUE) {
        // 目标屏幕：ui_Screen_menu 
        // 动画：淡入淡出 (FADE_ON)，耗时 500ms
        // true：切换完成后，自动将 ui_Screen_Init 彻底销毁，释放内存！
        lv_scr_load_anim(ui_Screen_menu, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, true);
        xSemaphoreGive(xGuiMutex);
    }

    // 🚀 第三阶段：正式进入主循环，处理日常点击和滑动
    while(1)
    {
        if (xSemaphoreTake(xGuiMutex, portMAX_DELAY) == pdTRUE) {
            lv_timer_handler(); 
            xSemaphoreGive(xGuiMutex);
        }
        vTaskDelay(pdMS_TO_TICKS(5)); 
    }
}

// =====================================================================
// 2：灯光渲染引擎，根据 current_mode 的值切换不同的灯效动画
// =====================================================================
void Task_LED(void *pvParameters)
{
    uint32_t anim_interval = 100;	//动画时间
	
		TickType_t xLastWakeTime = xTaskGetTickCount();
    while(1)
    {
        switch(current_mode) {
            case Off:     WS2812_Fill(0, 0, 0);                     anim_interval = 200; break;  //关闭灯光
            case White:   WS2812_Fill(255, 255, 255);               anim_interval = 200; break;  //红色灯光
            case Red:     WS2812_Fill(255, 0, 0);                   anim_interval = 200; break;  //红色灯光
            case Yellow:  WS2812_Fill(255, 255, 0);                 anim_interval = 200; break;  //黄色灯光
            case Blue:    WS2812_Fill(0, 0, 255);                   anim_interval = 200; break;  //蓝色灯光
            case Fire:    WS2812_Fire_Step();                       anim_interval = 20;  break;
            case Breath:  WS2812_Breathing_Step(NULL);              anim_interval = 10;  break;
            case Audio:   WS2812_Audio_Sync_Step();                 anim_interval = 10;  break;
            case Meteor:  WS2812_Meteor_Step(&meteor_pos);          anim_interval = 50;  break;
            case Rainbow: WS2812_Rainbow_Step(rainbow_offset++);    anim_interval = 20;  break; 
            default: anim_interval = 200; break; 
        }
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(anim_interval));
    }
}


// =====================================================================
// 3：通讯解析处理来自蓝牙和云端的指令，并更新 current_mode 和 input_flag
// =====================================================================
void Task_Comms(void *pvParameters)
{
    vTaskDelay(pdMS_TO_TICKS(5500));
    static uint8_t last_bt_state;
    last_bt_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3);
	
    while(1)
    {
        uint8_t current_bt_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3); // 读大门状态
        
        // =============================================================
        // 场景 A：你点了按钮，系统正在“卡着秒表”等连接
        // =============================================================
        if (bt_is_waiting == 1) 
        {
            if (current_bt_state == 1) // 15秒内手机连上了！
            {
                bt_is_waiting = 0;
                if (xSemaphoreTake(xGuiMutex, portMAX_DELAY) == pdTRUE) {
                    Trigger_BT_Success(); 
                    xSemaphoreGive(xGuiMutex);
                }
            }
            else if ((xTaskGetTickCount() - bt_start_tick) > pdMS_TO_TICKS(15000)) // 15秒超时没连上
            {
                bt_is_waiting = 0;
                if (xSemaphoreTake(xGuiMutex, portMAX_DELAY) == pdTRUE) {
                    Trigger_BT_Failure(); 
                    xSemaphoreGive(xGuiMutex);
                }
            }
        }
        // =============================================================
        // 场景 B：平时闲置/挂机状态（核心修复区：手机自己乱动的情况）
        // =============================================================
        else 
        {
            // 🚀 修复 1：手机自己连上了（原本没连，突然连了）
            if (last_bt_state == 0 && current_bt_state == 1) 
            {
                if (xSemaphoreTake(xGuiMutex, portMAX_DELAY) == pdTRUE) {
                    Trigger_BT_Success(); // 直接复用成功函数，弹窗+按钮变红
                    xSemaphoreGive(xGuiMutex);
                }
                printf(">> [后台巡逻] 手机主动连上了蓝牙！\r\n");
            }
            
            // 🚀 修复 2：手机自己断开了（原本连着，突然断了，比如走远了/关蓝牙了）
            else if (last_bt_state == 1 && current_bt_state == 0) 
            {
                WS2812_BlinkRed_Twice(); 
                current_mode = 0;        
                
                if (xSemaphoreTake(xGuiMutex, portMAX_DELAY) == pdTRUE) {
                    Trigger_BT_Disconnected(); // 呼叫一个专门的断开函数
                    xSemaphoreGive(xGuiMutex);
                }
                printf(">> [后台巡逻] 手机主动断开了蓝牙！\r\n");
            }
        }
        
        // 🚀 最关键的一句：保安把当前状态记在小本本上，为下一次循环做对比
        last_bt_state = current_bt_state; 
        // =============================================================
        // 第二部分：处理蓝牙控制指令 (恢复你的完美架构！)
        // =============================================================
        if (RX_Flag == 1) // 中断里收到数据了
        {
            // 必须是 UI 处于“已授权”状态，才允许修改灯光！
            if (bt_control_enabled == 1) 
            {
                // 🚀 1. 纯逻辑切换：只改状态，绝不越权碰底层硬件！
                switch(RX_Command) {
                    // 注意：这里的 Red, Blue, Meteor 等是你之前定义的枚举宏
                    case '1': current_mode = Red;     break; // 红
                    case '2': current_mode = Yellow;  break; // 黄
                    case '3': current_mode = Blue;    break; // 蓝
                    case '4': current_mode = Fire;    break; // 火焰
                    case '5': current_mode = Meteor;  meteor_pos = 0; break; // 流星
                    case '6': current_mode = Rainbow; break; // 彩虹
                    case '7': current_mode = Audio;   break; // 音乐律动
                    case '8': current_mode = White;   break; // 白色
                    case '0': current_mode = Off;     break; // 关灯
                }
                
                // 🚀 2. 状态追踪：恢复你原本写好的追踪器逻辑
                input_source = 1; // 标记来源：1 代表当前指令来自蓝牙
                input_flag = 1;   // 标记有新指令产生
                
                // 🚀 3. 呼叫 UI 部门：蓝牙改了颜色，LCD 面板必须同步更新！
                // (防止手机上切了红色，LCD 屏幕上显示的还是蓝色)
                if (xSemaphoreTake(xGuiMutex, portMAX_DELAY) == pdTRUE) {
                    // 调用你之前写好的 LCD 灯光面板更新逻辑
                    // 比如：Sync_LCD_Light_Panel(current_mode); 
                    Sync_LCD_Light_Panel(current_mode);
                    xSemaphoreGive(xGuiMutex);
                }
            }
            // 清除标志位，等待下一个指令
            RX_Flag = 0; 
        }

        vTaskDelay(pdMS_TO_TICKS(20)); 
    }
}

// =====================================================================
// 部门 4：软件定时器回调 (10秒自动执行一次)
// =====================================================================
void Timer_Heartbeat_Callback(TimerHandle_t xTimer)
{
//    mqtt_send_heart();
//		mqtt_upload_current_state();
}

// =====================================================================
// 硬件中断：保留给 LVGL 的心跳 (千万别删)
// =====================================================================
void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
    {
        lv_tick_inc(1);  // 专门服务于 LVGL 内部动画和计时
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}