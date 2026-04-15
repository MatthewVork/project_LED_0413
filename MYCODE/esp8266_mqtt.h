#ifndef __ES8266_MQTT_H
#define __ES8266_MQTT_H

#include "stm32f4xx.h"

// ==========================================
// OneNET ��¼����
// ==========================================
#define MQTT_BROKERADDRESS 		"xxxx"
#define MQTT_CLIENTID 			"xxxx"
#define MQTT_USARNAME 			"xxxxx"
#define MQTT_PASSWD 			"xxxx"
#define PORT		            xxxx

// ==========================================
// OneNET ��ģ������
// ==========================================
#define MQTT_SUBSCRIBE_TOPIC 	"$sys/9e20IFjk58/stm32f4xx/thing/property/set"
#define	MQTT_PUBLISH_TOPIC 		"$sys/9e20IFjk58/stm32f4xx/thing/property/post"
#define MQTT_REPLY_TOPIC      "$sys/9e20IFjk58/stm32f4xx/thing/property/set_reply"

#define BYTE0(dwTemp)       (*( char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))

// ==========================================
// �������� (������ #endif ֮ǰ)
// ==========================================
extern int32_t mqtt_connect(char *client_id,char *user_name,char *password);
extern int32_t mqtt_subscribe_topic(char *topic,uint8_t qos,uint8_t whether);
extern uint32_t mqtt_publish_data(char *topic, char *message, uint8_t qos);
extern int32_t mqtt_send_heart(void);
extern int32_t esp8266_mqtt_init(void);
extern void mqtt_report_devices_status(void);
extern void mqtt_reply_set_command(char *msg_id);

#endif