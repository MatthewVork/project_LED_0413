#include "main.h"
// =====================================================================
// [1] 系统精确延时
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
// [7] 主函数逻辑
// =====================================================================
int main(void) {
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  SystemInit();
  DWT_Init();
  
  WS2812_Init();
  Audio_ADC_Init();
  Bluetooth_P4_Init(9600); 
  BT_State_Pin_Init();     
  
  uint8_t last_bt_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3); // 开机读取初始状态
  uint8_t current_mode = 0;   
  uint8_t rainbow_offset = 0; 
  uint8_t meteor_pos = 0;  

  uint32_t last_print_time = DWT->CYCCNT;
  char print_buf[64];

  WS2812_Fill(10, 10, 10); 
  
  while(1) {
      // -------------------------------------------------------------
      // 每 0.5 秒打印一次，直接看最真实的裸数据！
      if (DWT->CYCCNT - last_print_time >= SystemCoreClock / 2) { 
          last_print_time = DWT->CYCCNT; 
          
          // 不循环了！不相减了！直接读一针见血的原始数据！
          uint16_t real_volume = Get_Audio_Raw();
          
          sprintf(print_buf, "Raw Volume: %d\r\n", real_volume);
          USART1_SendString(print_buf);
      }
      
      // -------------------------------------------------------------
      // 监测蓝牙状态引脚 (PA3)
      // -------------------------------------------------------------
      uint8_t current_bt_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3);
      if (last_bt_state == 1 && current_bt_state == 0) {
          WS2812_BlinkRed_Twice(); 
          current_mode = 0; 
      }
      else if (last_bt_state == 0 && current_bt_state == 1) {
          WS2812_BlinkGreen_Twice();
      }
      last_bt_state = current_bt_state; 

      // -------------------------------------------------------------
      // 处理蓝牙指令
      // -------------------------------------------------------------
      if(RX_Flag) {
          switch(RX_Command)
          {
              case '1': current_mode = 1; WS2812_Fill(255, 0, 0); break; 
              case '2': current_mode = 2; WS2812_Fill(0, 255, 0); break; 
              case '3': current_mode = 3; WS2812_Fill(0, 0, 255); break; 
              case '4': current_mode = 4; break; 
              case '5': current_mode = 5; meteor_pos = 0;   break; 
              case '6': current_mode = 6; break; 
              case '7': current_mode = 7; break; 
              case '8': current_mode = 8; break; // 发送 '8' 进入声控律动模式
              case '0': current_mode = 0; WS2812_Fill(0, 0, 0);   break; 
          }
          RX_Flag = 0; 
      }
      
      // -------------------------------------------------------------
      // 动画状态机执行
      // -------------------------------------------------------------
      switch(current_mode) {
          case 4: WS2812_Rainbow_Step(rainbow_offset++); Delay_ms(20); break;
          case 5: WS2812_Meteor_Step(&meteor_pos); Delay_ms(30); break;
          case 6: WS2812_Breathing_Step(NULL); Delay_ms(10); break;
          case 7: WS2812_Fire_Step(); Delay_ms(50); break;
          case 8: WS2812_Audio_Sync_Step(); /* 函数内已有延时，无需额外Delay */ break;
          default: Delay_ms(20); break;
      }
  }
}