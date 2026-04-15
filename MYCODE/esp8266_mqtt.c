#include "stm32f4xx.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "delay.h"
#include "usart.h"
#include "esp8266.h"
#include "esp8266_mqtt.h"

extern uint8_t current_mode; 
char  g_mqtt_msg[512];
uint32_t g_mqtt_tx_len;

void mqtt_send_bytes(uint8_t *buf,uint32_t len) { esp8266_send_bytes(buf,len); }

int32_t mqtt_send_heart(void) {	
	uint8_t buf[2]={0xC0,0x00};
	mqtt_send_bytes(buf,2);
	return 0;
}

// 恢复：正规的 MQTT 报文打包与二进制应答解析
int32_t mqtt_connect(char *client_id,char *user_name,char *password)
{
    uint32_t client_id_len = strlen(client_id);
    uint32_t user_name_len = strlen(user_name);
    uint32_t password_len = strlen(password);
    uint32_t data_len = 10 + (client_id_len+2) + (user_name_len+2) + (password_len+2);
    uint32_t wait = 0;
    g_mqtt_tx_len = 0;
    
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 0x10;
    do { uint8_t eb = data_len % 128; data_len /= 128; if(data_len>0) eb|=128; g_esp8266_tx_buf[g_mqtt_tx_len++]=eb; } while(data_len>0);
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 0; g_esp8266_tx_buf[g_mqtt_tx_len++] = 4;
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 'M'; g_esp8266_tx_buf[g_mqtt_tx_len++] = 'Q';
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 'T'; g_esp8266_tx_buf[g_mqtt_tx_len++] = 'T';
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 4; g_esp8266_tx_buf[g_mqtt_tx_len++] = 0xC2;
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 0; g_esp8266_tx_buf[g_mqtt_tx_len++] = 60;
    
    g_esp8266_tx_buf[g_mqtt_tx_len++] = BYTE1(client_id_len); g_esp8266_tx_buf[g_mqtt_tx_len++] = BYTE0(client_id_len);
    memcpy(&g_esp8266_tx_buf[g_mqtt_tx_len], client_id, client_id_len); g_mqtt_tx_len += client_id_len;
    
    g_esp8266_tx_buf[g_mqtt_tx_len++] = BYTE1(user_name_len); g_esp8266_tx_buf[g_mqtt_tx_len++] = BYTE0(user_name_len);
    memcpy(&g_esp8266_tx_buf[g_mqtt_tx_len], user_name, user_name_len); g_mqtt_tx_len += user_name_len;
    
    g_esp8266_tx_buf[g_mqtt_tx_len++] = BYTE1(password_len); g_esp8266_tx_buf[g_mqtt_tx_len++] = BYTE0(password_len);
    memcpy(&g_esp8266_tx_buf[g_mqtt_tx_len], password, password_len); g_mqtt_tx_len += password_len;

    memset((void *)g_esp8266_rx_buf, 0, sizeof(g_esp8266_rx_buf));
    g_esp8266_rx_cnt = 0;
    
    mqtt_send_bytes(g_esp8266_tx_buf, g_mqtt_tx_len);
    
    wait = 3000;
    while(wait--) {
        delay_ms(1);
        
        // 抓取 OneNET 返回的 0x20 0x02 (CONNACK 应答包)
        if((g_esp8266_rx_buf[0] == 0x20) && (g_esp8266_rx_buf[1] == 0x02)) {
            if(g_esp8266_rx_buf[3] == 0x00) {
                return 0; // 0x00 代表完美连上
            } 
            else {
                printf("\r\n? 收到 OneNET 拒绝报文！错误码: 0x%02X\r\n", g_esp8266_rx_buf[3]);
                if(g_esp8266_rx_buf[3] == 0x04) printf("?? 诊断：【用户名或密码错误(Token/ProductID不对)】\r\n");
                else if(g_esp8266_rx_buf[3] == 0x02) printf("?? 诊断：【非法的客户端标识符(DeviceName不对)】\r\n");
                else if(g_esp8266_rx_buf[3] == 0x05) printf("?? 诊断：【未授权访问】\r\n");
                else printf("?? 诊断：其他协议错误\r\n");
                return -1; 
            }
        }
    }
    printf("\r\n? 纯超时！OneNET没理咱们。\r\n");
    return -1;
}

// 恢复：正规的订阅应答解析
int32_t mqtt_subscribe_topic(char *topic,uint8_t qos,uint8_t whether) {
    uint32_t topiclen = strlen(topic);
    uint32_t data_len = 2 + (topiclen+2) + (whether?1:0);
    uint32_t wait = 0;
	g_mqtt_tx_len=0;
	
	g_esp8266_tx_buf[g_mqtt_tx_len++] = whether?0x82:0xA2;
    do { uint8_t eb = data_len % 128; data_len /= 128; if(data_len>0) eb|=128; g_esp8266_tx_buf[g_mqtt_tx_len++]=eb; } while(data_len>0);
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 0; g_esp8266_tx_buf[g_mqtt_tx_len++] = 0x01;
    g_esp8266_tx_buf[g_mqtt_tx_len++] = BYTE1(topiclen); g_esp8266_tx_buf[g_mqtt_tx_len++] = BYTE0(topiclen);
    memcpy(&g_esp8266_tx_buf[g_mqtt_tx_len],topic,topiclen); g_mqtt_tx_len += topiclen;
    if(whether) g_esp8266_tx_buf[g_mqtt_tx_len++] = qos;
    
    memset((void *)g_esp8266_rx_buf,0,sizeof(g_esp8266_rx_buf));
    g_esp8266_rx_cnt=0;
    
    mqtt_send_bytes(g_esp8266_tx_buf,g_mqtt_tx_len);
    
    wait=3000;
    while(wait--) {
        delay_ms(1);
        // ?? 核心修复：纯二进制判定
        if(g_esp8266_rx_buf[0]==0x90) return 0; // 订阅成功
    }
    return -1;
}

uint32_t mqtt_publish_data(char *topic, char *message, uint8_t qos) {
    uint32_t topicLength = strlen(topic);
    uint32_t messageLength = strlen(message);
    uint32_t data_len = (2+topicLength) + messageLength;
    g_mqtt_tx_len=0;
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 0x30;
    do { uint8_t eb = data_len % 128; data_len /= 128; if(data_len>0) eb|=128; g_esp8266_tx_buf[g_mqtt_tx_len++]=eb; } while(data_len>0);
    g_esp8266_tx_buf[g_mqtt_tx_len++] = BYTE1(topicLength); g_esp8266_tx_buf[g_mqtt_tx_len++] = BYTE0(topicLength);
    memcpy(&g_esp8266_tx_buf[g_mqtt_tx_len],topic,topicLength); g_mqtt_tx_len += topicLength;
    memcpy(&g_esp8266_tx_buf[g_mqtt_tx_len],message,messageLength); g_mqtt_tx_len += messageLength;
	mqtt_send_bytes(g_esp8266_tx_buf,g_mqtt_tx_len);
	return g_mqtt_tx_len;
}

void mqtt_report_devices_status(void) {
	memset(g_mqtt_msg, 0, sizeof(g_mqtt_msg));
	sprintf(g_mqtt_msg,"{\"id\":\"1\",\"params\":{\"WorkMode\":%d}}", current_mode);
    mqtt_publish_data(MQTT_PUBLISH_TOPIC, g_mqtt_msg, 0);
}

// 专门用来回应 OneNET 控制指令的函数
void mqtt_reply_set_command(char *msg_id) {
    char reply_msg[128];
    memset(reply_msg, 0, sizeof(reply_msg));
    
    // 按 OneJson 格式打包应答：{"id":"提取到的ID","code":200,"msg":"success"}
    sprintf(reply_msg, "{\"id\":\"%s\",\"code\":200,\"msg\":\"success\"}", msg_id);
    
    // 发送到专属的 set_reply 主题
    mqtt_publish_data(MQTT_REPLY_TOPIC, reply_msg, 0);
}

// 恢复：加入分步日志，死在哪一步一目了然
int32_t esp8266_mqtt_init(void) {
	int32_t rt;
	esp8266_init();
	esp8266_exit_transparent_transmission();
	delay_s(1); esp8266_reset();
	delay_s(2); esp8266_enable_echo(0);
	
    printf("-> 连接WiFi...");
	rt = esp8266_connect_ap(WIFI_SSID,WIFI_PASSWORD);
    if(rt) { printf("失败!\r\n"); return -4; }
    printf("成功!\r\n");
    delay_s(2);
    
    printf("-> 连接OneNET...");
	rt = esp8266_connect_server("TCP",MQTT_BROKERADDRESS,PORT);
    if(rt) { printf("失败!\r\n"); return -5; }
    printf("成功!\r\n");
    delay_s(2);
    
    printf("-> 开启透传...");
	rt = esp8266_entry_transparent_transmission();
    if(rt) { printf("失败!\r\n"); return -6; }
    printf("成功!\r\n");
    delay_s(2);
    
    printf("-> 登录MQTT鉴权...");
	rt = mqtt_connect(MQTT_CLIENTID, MQTT_USARNAME, MQTT_PASSWD);
    if(rt) { printf("失败(请检查Token)!\r\n"); return -7; }
    printf("成功!\r\n");
    delay_s(2);
    
    printf("-> 订阅主题...");
	rt = mqtt_subscribe_topic(MQTT_SUBSCRIBE_TOPIC,0,1);
    if(rt) { printf("失败!\r\n"); return -8; }
    printf("成功!\r\n");
    
	return 0;
}

