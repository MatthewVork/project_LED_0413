#include "bemfa.h"

// void Bemfa_Cloud_Init(void) {
//    // 1. 测试指令
//    WiFi_SendString("AT\r\n");
//    Delay_ms(500);
//    
//    // 2. 设置为 Station 模式
//    WiFi_SendString("AT+CWMODE=1\r\n");
//    Delay_ms(500);
//    
//    // 3. 连接 WiFi (请替换为你的 WiFi 名和密码)
//    WiFi_SendString("AT+CWJAP=\"你的WiFi名\",\"你的密码\"\r\n");
//    Delay_ms(5000); 
//    
//    // 4. 连接巴法云 TCP 服务器 (地址: bemfa.com, 端口: 8344)
//    WiFi_SendString("AT+CIPSTART=\"TCP\",\"bemfa.com\",8344\r\n");
//    Delay_ms(2000);
//    
//    // 5. 开启透传模式
//    WiFi_SendString("AT+CIPMODE=1\r\n");
//    Delay_ms(500);
//    WiFi_SendString("AT+CIPSEND\r\n");
//    Delay_ms(500);
//    
//    // 6. 订阅主题 (请替换为你的私钥和主题名)
//    // 格式: cmd=1&uid=私钥&topic=主题名\r\n
//    WiFi_SendString("cmd=1&uid=你的私钥xxxxxxx&topic=light002\r\n");
//}