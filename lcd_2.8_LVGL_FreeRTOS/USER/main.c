#include "main.h"

// =====================================================================
// [全局变量区]
// =====================================================================
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
// MAIN 函数：负责底层初始化和启动系统
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

    // 创建三大任务 (完全保留你原来的优先级和堆栈设置)
    xTaskCreate(Task_GUI,   "Task_GUI",   1024, NULL, 2, &Task_GUI_Handle);
    xTaskCreate(Task_LED,   "Task_LED",   256,  NULL, 3, &Task_LED_Handle);
    xTaskCreate(Task_Comms, "Task_Comms", 512,  NULL, 4, &Task_Comms_Handle);

    // 启动调度器，接管控制权
    vTaskStartScheduler();
		
    while(1); // 永远跑不到这里
}

// =====================================================================
// 1：GUI 刷新任务 (完全保留你的 5 秒开机画面逻辑)
// =====================================================================
void Task_GUI(void *pvParameters)
{
    // 🚀 第一阶段：让初始化屏幕 (ui_Screen_Init) 运行 5 秒钟
    // 5000 毫秒 / 每次循环 5 毫秒 = 1000 次循环
    for(int i = 0; i < 1000; i++) 
    {
        if (xSemaphoreTake(xGuiMutex, portMAX_DELAY) == pdTRUE) {
            lv_timer_handler(); 
            xSemaphoreGive(xGuiMutex);
        }
        vTaskDelay(pdMS_TO_TICKS(5)); 
    }

    // 🚀 第二阶段：5 秒时间到，执行华丽切换！
    if (xSemaphoreTake(xGuiMutex, portMAX_DELAY) == pdTRUE) {
        lv_scr_load_anim(ui_Screen_menu, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, true);
        xSemaphoreGive(xGuiMutex);
    }

    // 🚀 第三阶段：正式进入主循环
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
// 2：灯光渲染引擎 (完全保留你的高低帧率控制)
// =====================================================================
void Task_LED(void *pvParameters)
{
    uint32_t anim_interval = 100;	//动画时间
	
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while(1)
    {
        switch(current_mode) {
            case Off:     WS2812_Fill(0, 0, 0);                     anim_interval = 200; break;  //关闭灯光
            case White:   WS2812_Fill(255, 255, 255);               anim_interval = 200; break;  //白色灯光
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
// 3：通讯调度任务 (完美缝合：蓝牙 + 你的防乱码神技 OneNET 解析)
// =====================================================================
void Task_Comms(void *pvParameters)
{
    // 🚀 在后台默默执行 OneNET 联网！由于 delay.c 已修改，它不会卡死 GUI 动画！
    esp8266_mqtt_init();

    vTaskDelay(pdMS_TO_TICKS(500)); // 联网后稍微缓冲一下
    static uint8_t last_bt_state;
    last_bt_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3);

    TickType_t last_heartbeat_tick = xTaskGetTickCount();
	
    while(1)
    {
        uint8_t current_bt_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3); // 读大门状态
        
        // -------------------------------------------------------------
        // [控制路 1] 蓝牙物理状态监测
        // -------------------------------------------------------------
        if (bt_is_waiting == 1) 
        {
            if (current_bt_state == 1) 
            {
                bt_is_waiting = 0;
                if (xSemaphoreTake(xGuiMutex, portMAX_DELAY) == pdTRUE) {
                    Trigger_BT_Success(); xSemaphoreGive(xGuiMutex);
                }
            }
            else if ((xTaskGetTickCount() - bt_start_tick) > pdMS_TO_TICKS(15000)) 
            {
                bt_is_waiting = 0;
                if (xSemaphoreTake(xGuiMutex, portMAX_DELAY) == pdTRUE) {
                    Trigger_BT_Failure(); xSemaphoreGive(xGuiMutex);
                }
            }
        }
        else 
        {
            if (last_bt_state == 0 && current_bt_state == 1) 
            {
                if (xSemaphoreTake(xGuiMutex, portMAX_DELAY) == pdTRUE) {
                    Trigger_BT_Success(); xSemaphoreGive(xGuiMutex);
                }
                printf(">> [后台巡逻] 手机主动连上了蓝牙！\r\n");
            }
            else if (last_bt_state == 1 && current_bt_state == 0) 
            {
                WS2812_BlinkRed_Twice(); 
                current_mode = 0;        
                if (xSemaphoreTake(xGuiMutex, portMAX_DELAY) == pdTRUE) {
                    Trigger_BT_Disconnected(); xSemaphoreGive(xGuiMutex);
                }
                printf(">> [后台巡逻] 手机主动断开了蓝牙！\r\n");
            }
        }
        last_bt_state = current_bt_state; 

        // -------------------------------------------------------------
        // [控制路 2] 蓝牙串口指令处理
        // -------------------------------------------------------------
        if (RX_Flag == 1) 
        {
            if (bt_control_enabled == 1) 
            {
                switch(RX_Command) {
                    case '1': current_mode = Red;     break;
                    case '2': current_mode = Yellow;  break;
                    case '3': current_mode = Blue;    break;
                    case '4': current_mode = Fire;    break;
                    case '5': current_mode = Meteor;  meteor_pos = 0; break;
                    case '6': current_mode = Rainbow; break;
                    case '7': current_mode = Audio;   break;
                    case '8': current_mode = White;   break;
                    case '0': current_mode = Off;     break;
                }
                input_source = 1; input_flag = 1;
                
                if (xSemaphoreTake(xGuiMutex, portMAX_DELAY) == pdTRUE) {
                    Sync_LCD_Light_Panel(current_mode);
                    xSemaphoreGive(xGuiMutex);
                }
            }
            RX_Flag = 0; 
        }

        // -------------------------------------------------------------
        // [控制路 3] OneNET 云端 JSON 防爆护盾解析 (原封不动保留你的逻辑)
        // -------------------------------------------------------------
        if(g_esp8266_rx_cnt > 0) {
            vTaskDelay(pdMS_TO_TICKS(30));  // 等数据收完
            
            char *json_start = NULL;
            for (int i = 0; i < g_esp8266_rx_cnt; i++) {
                if (g_esp8266_rx_buf[i] == '{') { 
                    json_start = (char *)&g_esp8266_rx_buf[i];
                    break;
                }
            }

            if (json_start != NULL) {
                printf("\r\n>> 成功过滤乱码！纯净 JSON:\r\n%s\r\n", json_start);
                
                // 1. 精准提取流水号 ID
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

                // 2. 执行动作
                uint8_t cmd_executed = 0;
                if(strstr(json_start, "\"WorkMode\":0"))       { current_mode = Off;    cmd_executed = 1; }
                else if(strstr(json_start, "\"WorkMode\":1"))  { current_mode = Red;    cmd_executed = 1; }
                else if(strstr(json_start, "\"WorkMode\":2"))  { current_mode = Yellow; cmd_executed = 1; }
                else if(strstr(json_start, "\"WorkMode\":3"))  { current_mode = Blue;   cmd_executed = 1; }
                else if(strstr(json_start, "\"WorkMode\":4"))  { current_mode = Fire;   cmd_executed = 1; }
                else if(strstr(json_start, "\"WorkMode\":5"))  { current_mode = Meteor; cmd_executed = 1; meteor_pos = 0; }
                else if(strstr(json_start, "\"WorkMode\":6"))  { current_mode = Rainbow;cmd_executed = 1; }
                else if(strstr(json_start, "\"WorkMode\":7"))  { current_mode = Audio;  cmd_executed = 1; }
                else if(strstr(json_start, "\"WorkMode\":8"))  { current_mode = White;  cmd_executed = 1; }
                else if(strstr(json_start, "\"WorkMode\":9"))  { current_mode = Off;    cmd_executed = 1; }

                // 3. 回码与 LCD 同步
                if(cmd_executed) {
                    input_source = 2; input_flag = 1;
                    if(msg_id[0] != '\0') {
                        mqtt_reply_set_command(msg_id); 
                        printf(">> 已向 OneNET 回码(ID: %s)\r\n", msg_id);
                    }
                    if (xSemaphoreTake(xGuiMutex, portMAX_DELAY) == pdTRUE) {
                        Sync_LCD_Light_Panel(current_mode);
                        xSemaphoreGive(xGuiMutex);
                    }
                }
            } 

            memset((void *)g_esp8266_rx_buf, 0, ESP_BUF_SIZE); 
            g_esp8266_rx_cnt = 0;
            g_esp8266_rx_end = 0;
        }

        if ((xTaskGetTickCount() - last_heartbeat_tick) > pdMS_TO_TICKS(10000)) 
        {
            mqtt_send_heart(); // 发送 0xC0 0x00
            printf(">> [HeartBeat]Onenet send success!\r\n");
            
            // 重置秒表，等下一个 10 秒
            last_heartbeat_tick = xTaskGetTickCount(); 
        }

        vTaskDelay(pdMS_TO_TICKS(20)); 
    }
}

// =====================================================================
// 4：软件定时器回调
// =====================================================================
void Timer_Heartbeat_Callback(TimerHandle_t xTimer)
{
    mqtt_send_heart();
    mqtt_upload_current_state();    // 定时上传当前状态到 OneNET
}

// =====================================================================
// 5：硬件中断：保留给 LVGL 的心跳 (千万别删)
// =====================================================================
void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
    {
        lv_tick_inc(1);  // 专门服务于 LVGL 内部动画和计时
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}