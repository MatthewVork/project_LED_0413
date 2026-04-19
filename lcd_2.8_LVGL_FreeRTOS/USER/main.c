#include "main.h"

uint8_t current_mode = 0;  // 当前模式
uint8_t input_flag   = 0;  // 是否有新指令 (0:无, 1:有)
uint8_t input_source = 0;  // 追踪器：1:蓝牙, 2:OneNET, 3:触摸屏

volatile float global_brightness = 1.0f;

uint8_t rainbow_offset = 0; 
uint8_t meteor_pos = 0;  

// 通讯缓冲变量
uint8_t RX_Command;
uint8_t RX_Flag;

uint8_t  g_esp8266_rx_buf[ESP_BUF_SIZE]; 
uint32_t g_esp8266_rx_cnt = 0;           
uint32_t g_esp8266_rx_end = 0;  

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
//    BT_State_Pin_Init();     
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
//    xTaskCreate(Task_Comms, "Task_Comms", 512,  NULL, 4, &Task_Comms_Handle);

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
    static uint8_t last_bt_state = 1;
	
		printf("\r\n>> [Network Task] Starting ESP8266 & OneNET Init...\r\n");
    esp8266_mqtt_init(); 
    printf(">> [Network Task] Connection Success! Waiting for commands...\r\n");
    
    while(1)
    {
        // -------------------------------------------------------------
        // [源头 A] 蓝牙状态机与指令
        // -------------------------------------------------------------
        uint8_t current_bt_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3);
        if (last_bt_state == 1 && current_bt_state == 0) {
            WS2812_BlinkRed_Twice(); current_mode = 0; 
        } else if (last_bt_state == 0 && current_bt_state == 1) {
            WS2812_BlinkGreen_Twice();
        }
        last_bt_state = current_bt_state; 

        if(RX_Flag) {
            switch(RX_Command - 48) {
                case Off:     current_mode = Off;     break; 
                case White:   current_mode = White;   break; 
                case Red:     current_mode = Red;     break; 
                case Yellow:  current_mode = Yellow;  break; 
                case Blue:    current_mode = Blue;    break; 
                case Fire:    current_mode = Fire;    break; 
                case Breath:  current_mode = Breath;  break; 
                case Audio:   current_mode = Audio;   break; 
                case Meteor:  current_mode = Meteor;  break;
                case Rainbow: current_mode = Rainbow; break;
                default: break;
            }
            input_source = 1; input_flag = 1; 
            RX_Flag = 0; 
        }

        // -------------------------------------------------------------
        // [源头 B] OneNET 云端软断帧与解析
        // -------------------------------------------------------------
        if (g_esp8266_rx_cnt > 0 && g_esp8266_rx_end == 0) {
            static uint32_t rx_wait_time = 0;
            static uint32_t last_cnt = 0;
            if (g_esp8266_rx_cnt != last_cnt) {
                last_cnt = g_esp8266_rx_cnt;
                rx_wait_time = xTaskGetTickCount(); // 用 OS 的时间戳
            } else if ((xTaskGetTickCount() - rx_wait_time) >= pdMS_TO_TICKS(30)) {
                g_esp8266_rx_end = 1;  
                last_cnt = 0;          
            }
        }
                
        if (g_esp8266_rx_end == 1) { 
            char *json_start = NULL;
            for (int i = 0; i < g_esp8266_rx_cnt; i++) {
                if (g_esp8266_rx_buf[i] == '{') { json_start = (char *)&g_esp8266_rx_buf[i]; break; }
            }

            if (json_start != NULL) {
                char msg_id[20] = {0};
                char *id_p = strstr(json_start, "\"id\":"); 
                if(id_p) {
                    id_p += 5; 
                    while(*id_p == ' ' || *id_p == '\"' || *id_p == ':') id_p++; 
                    for(int i = 0; i < 19; i++) {
                        if(id_p[i] == '\"' || id_p[i] == ',' || id_p[i] == '}' || id_p[i] == ' ') break; 
                        msg_id[i] = id_p[i];
                    }
                }

                uint8_t cmd_executed = 0;
                if     (strstr(json_start, "\"WorkMode\":0")) { current_mode = Off;     cmd_executed = 1; }
                else if(strstr(json_start, "\"WorkMode\":1")) { current_mode = White;   cmd_executed = 1; }
                else if(strstr(json_start, "\"WorkMode\":2")) { current_mode = Red;     cmd_executed = 1; }
                else if(strstr(json_start, "\"WorkMode\":3")) { current_mode = Yellow;  cmd_executed = 1; }
                else if(strstr(json_start, "\"WorkMode\":4")) { current_mode = Blue;    cmd_executed = 1; }
                else if(strstr(json_start, "\"WorkMode\":5")) { current_mode = Fire;    cmd_executed = 1; }
                else if(strstr(json_start, "\"WorkMode\":6")) { current_mode = Breath;  cmd_executed = 1; }
                else if(strstr(json_start, "\"WorkMode\":7")) { current_mode = Audio;   cmd_executed = 1; }
                else if(strstr(json_start, "\"WorkMode\":8")) { current_mode = Meteor;  cmd_executed = 1; meteor_pos = 0;}
                else if(strstr(json_start, "\"WorkMode\":9")) { current_mode = Rainbow; cmd_executed = 1; }

                if(cmd_executed) {
                    input_source = 2; input_flag = 1; 
                    if(msg_id[0] != '\0') { mqtt_reply_set_command(msg_id); }
                }
            } 
            memset((void *)g_esp8266_rx_buf, 0, ESP_BUF_SIZE); 
            g_esp8266_rx_cnt = 0; g_esp8266_rx_end = 0; 
        }

        // -------------------------------------------------------------
        // [中心打印台] 统一输出情报
        // -------------------------------------------------------------
//        if (input_flag == 1) 
//        {
//            if      (input_source == 1) printf("[指令来源] >>> 蓝牙端 (Bluetooth)\r\n");
//            else if (input_source == 2) printf("[指令来源] >>> 云平台 (OneNET)\r\n");
//            else if (input_source == 3) printf("[指令来源] >>> 触摸屏 (LVGL UI)\r\n");

//            printf("[执行动作] >>> ");
//            switch(current_mode) {
//                case Off:       printf("关闭氛围灯 (OFF)\r\n"); break;
//                case White:     printf("切换为: 白色常亮\r\n"); break;
//                case Red:       printf("切换为: 红色常亮\r\n"); break;
//                case Yellow:    printf("切换为: 黄色常亮\r\n"); break;
//                case Blue:      printf("切换为: 蓝色常亮\r\n"); break;
//                case Fire:      printf("切换为: 火焰 (Fire)\r\n"); break;
//                case Breath:    printf("切换为: 呼吸灯 (Breathing)\r\n"); break;
//                case Audio:     printf("切换为: 音乐律动 (Audio Sync)\r\n"); break;
//                case Meteor:    printf("切换为: 流星雨 (Meteor)\r\n"); break;
//                case Rainbow:   printf("切换为: 彩虹渐变 (Rainbow)\r\n"); break;
//                default:printf("未知模式\r\n"); break;
//            }
//						
//						if(input_source == 1 || input_source == 3) 
//						{
//							 mqtt_upload_current_state();
//							 printf(">> [Sync] State synchronized to OneNET!\r\n");
//            }

//            input_flag = 0; input_source = 0;
//        }

        vTaskDelay(pdMS_TO_TICKS(10)); // 防止通讯任务饿死低优先级任务
    }
}

// =====================================================================
// 部门 4：软件定时器回调 (10秒自动执行一次)
// =====================================================================
void Timer_Heartbeat_Callback(TimerHandle_t xTimer)
{
    mqtt_send_heart();
		mqtt_upload_current_state();
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