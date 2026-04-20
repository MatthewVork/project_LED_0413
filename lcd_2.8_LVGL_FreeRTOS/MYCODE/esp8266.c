#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"
#include "main.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "esp8266.h"
#include "usart.h"


uint8_t  g_esp8266_tx_buf[512];

volatile uint32_t g_esp8266_transparent_transmission_sta=0;

void esp8266_init(void)
{
	Usart3_init(115200);
}


void esp8266_send_at(char *str)
{
	//��ս��ջ�����?
	memset((void *)g_esp8266_rx_buf,0, sizeof g_esp8266_rx_buf);
	
	//��ս��ռ����?
	g_esp8266_rx_cnt = 0;	
	
	//����3��������
	usart3_send_str(str);
}

void esp8266_send_bytes(uint8_t *buf,uint32_t len)
{
	usart3_send_bytes(buf,len);

}

void esp8266_send_str(char *buf)
{
	usart3_send_str(buf);

}

/* ���ҽ������ݰ��е��ַ��� */
int32_t esp8266_find_str_in_rx_packet(char *str,uint32_t timeout)
{
	char *dest = str;
	char *src  = (char *)&g_esp8266_rx_buf;
	
	//�ȴ����ڽ�����ϻ�ʱ�˳�?
	while((strstr(src,dest)==NULL) && timeout)
	{		
		delay_ms(1);
		timeout--;
	}

	if(timeout) 
		return 0; 
		                    
	return -1; 
}


/* �Լ����? */
int32_t  esp8266_self_test(void)
{
	esp8266_send_at("AT\r\n");
	
	return esp8266_find_str_in_rx_packet("OK",1000);
}

/**
 * ���ܣ������ȵ�
 * ������
 *         ssid:�ȵ���
 *         pwd:�ȵ�����
 * ����ֵ��
 *         ���ӽ��?,��0���ӳɹ�,0����ʧ��
 * ˵���� 
 *         ʧ�ܵ�ԭ�������¼���(UARTͨ�ź�ESP8266���������?)
 *         1. WIFI�������벻��ȷ
 *         2. ·���������豸̫��,δ�ܸ�ESP8266����IP
 */
int32_t esp8266_connect_ap(char* ssid,char* pswd)
{
    // 1. ����Ϊ STATION ģʽ
    esp8266_send_at("AT+CWMODE_CUR=1\r\n"); 
    if(esp8266_find_str_in_rx_packet("OK",1000))
        return -1;

    // 2. ��������ָ�� (����������ͼ��ױ��ضϣ����Ǳ������ԭ��ʽ����ȷ����ɱ׼ȷ)
    esp8266_send_at("AT+CWJAP_CUR="); 
    esp8266_send_at("\"");esp8266_send_at(ssid);esp8266_send_at("\"");	
    esp8266_send_at(",");	
    esp8266_send_at("\"");esp8266_send_at(pswd);esp8266_send_at("\"");	
    esp8266_send_at("\r\n");
    
    // 3. ?? �ռ��޸������� "GOT IP" �ַ���������ץȡ���������? "CONNECT"
    // ��ȴ� 8 �룬��������ϣ������ȵ�û����������ʱ���ԹԷ���? -2
    if(esp8266_find_str_in_rx_packet("GOT IP", 8000) == 0) {
        return 0; // �ɹ��õ� IP
    }

    return -2; // ��ʱ��ʧ��
}

/* �˳�͸��ģʽ */
int32_t esp8266_exit_transparent_transmission (void)
{

	esp8266_send_at ("+++");
	

	//�˳�͸��ģʽ��������һ��ATָ��Ҫ���?1��
	delay_s(1); 

	//��¼��ǰesp8266�����ڷ�͸��ģʽ
	g_esp8266_transparent_transmission_sta = 0;

	return 0;
}

/* ����͸��ģʽ */
int32_t  esp8266_entry_transparent_transmission(void)
{
	//����͸��ģʽ
	esp8266_send_at("AT+CIPMODE=1\r\n");  
	if(esp8266_find_str_in_rx_packet("OK",5000))
		return -1;
	
	delay_s(2);
	//��������״̬
	esp8266_send_at("AT+CIPSEND\r\n");
	if(esp8266_find_str_in_rx_packet("OK",5000))
		return -2;

	//��¼��ǰesp8266������͸��ģʽ
	g_esp8266_transparent_transmission_sta = 1;
	return 0;
}

/**
 * ���ܣ�ʹ��ָ��Э��(TCP/UDP)���ӵ�������
 * ������
 *         mode:Э������ "TCP","UDP"
 *         ip:Ŀ�������IP
 *         port:Ŀ���Ƿ������˿ں�
 * ����ֵ��
 *         ���ӽ��?,��0���ӳɹ�,0����ʧ��
 * ˵���� 
 *         ʧ�ܵ�ԭ�������¼���(UARTͨ�ź�ESP8266���������?)
 *         1. Զ�̷�����IP�Ͷ˿ں�����
 *         2. δ����AP
 *         3. �������˽�ֹ����(һ�㲻�ᷢ��)
 */
int32_t esp8266_connect_server(char* mode,char* ip,uint16_t port)
{

#if 0	
	//ʹ��MQTT���ݵ�ip��ַ������������ʹ�����·�����������ջ���?
	//AT+CIPSTART="TCP","a10tC4OAAPc.iot-as-mqtt.cn-shanghai.aliyuncs.com",1883�����ַ���ռ���ڴ������?
	
	char buf[128]={0};
	
	//���ӷ�����
	sprintf((char*)buf,"AT+CIPSTART=\"%s\",\"%s\",%d\r\n",mode,ip,port);
	
	esp8266_send_at(buf);
#else
	
	char buf[16]={0};
	esp8266_send_at("AT+CIPSTART=");
	esp8266_send_at("\"");	esp8266_send_at(mode);	esp8266_send_at("\"");
	esp8266_send_at(",");
	esp8266_send_at("\"");	esp8266_send_at(ip);	esp8266_send_at("\"");	
	esp8266_send_at(",");
	sprintf(buf,"%d",port);
	esp8266_send_at(buf);	
	esp8266_send_at("\r\n");
	
#endif
	
	if(esp8266_find_str_in_rx_packet("CONNECT",5000))
		if(esp8266_find_str_in_rx_packet("OK",5000))
			return -1;
	return 0;
}

/* �Ͽ������� */
int32_t esp8266_disconnect_server(void)
{
	esp8266_send_at("AT+CIPCLOSE\r\n");
		
	if(esp8266_find_str_in_rx_packet("CLOSED",5000))
		if(esp8266_find_str_in_rx_packet("OK",5000))
			return -1;
	
	return 0;	
}


/* ʹ�ܶ����� */
int32_t esp8266_enable_multiple_id(uint32_t b)
{

	char buf[32]={0};
	
	sprintf(buf,"AT+CIPMUX=%d\r\n", b);
	esp8266_send_at(buf);
	
	if(esp8266_find_str_in_rx_packet("OK",5000))
		return -1;
	
	return 0;
}

/* ���������� */
int32_t esp8266_create_server(uint16_t port)
{
	char buf[32]={0};
	
	sprintf(buf,"AT+CIPSERVER=1,%d\r\n", port);
	esp8266_send_at(buf);
	
	if(esp8266_find_str_in_rx_packet("OK",5000))
		return -1;
	
	return 0;
}

/* �رշ����� */
int32_t esp8266_close_server(uint16_t port)
{
	char buf[32]={0};
	
	sprintf(buf,"AT+CIPSERVER=0,%d\r\n", port);
	esp8266_send_at(buf);
	
	if(esp8266_find_str_in_rx_packet("OK",5000))
		return -1;
	
	return 0;
}

/* ���Դ򿪻�ر�? */
int32_t esp8266_enable_echo(uint32_t b)
{
	if(b)
		esp8266_send_at("ATE1\r\n"); 
	else
		esp8266_send_at("ATE0\r\n"); 
	
	if(esp8266_find_str_in_rx_packet("OK",5000))
		return -1;

	return 0;
}

/* ��λ */
int32_t esp8266_reset(void)
{
	esp8266_send_at("AT+RST\r\n");
	
	if(esp8266_find_str_in_rx_packet("OK",10000))
		return -1;

	return 0;
}


// 🚀 终极安全版：获取网络真实时间 (NTP)
int32_t esp8266_get_network_time(uint16_t *year, uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *min, uint8_t *sec)
{
    // 配置时区(�?8�?)�? NTP 服务�?
    esp8266_send_at("AT+CIPSNTPCFG=1,8,\"ntp1.aliyun.com\"\r\n");
    delay_ms(500);

    for (int i = 0; i < 5; i++) {
        esp8266_send_at("AT+CIPSNTPTIME?\r\n");
        
        if (esp8266_find_str_in_rx_packet("OK", 2000) == 0) {
            char *time_ptr = strstr((char *)g_esp8266_rx_buf, "+CIPSNTPTIME:");
            
            if (time_ptr != NULL) {
                char week[4] = {0}, month_str[4] = {0};
                int p_year, p_day, p_hour, p_min, p_sec;
                
                // 利用 sscanf �?动提取报�? (预期格式: +CIPSNTPTIME:Mon Apr 20 18:07:00 2026)
                int matched = sscanf(time_ptr, "+CIPSNTPTIME:%3s %3s %d %d:%d:%d %d", 
                                     week, month_str, &p_day, &p_hour, &p_min, &p_sec, &p_year);
                                     
                if (matched == 7 && p_year != 1970) {
                    *year = p_year; *day = p_day; *hour = p_hour; *min = p_min; *sec = p_sec;
                    
                    // �?换英文月�?
                    const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
                    for(int m = 0; m < 12; m++) {
                        if(strcmp(month_str, months[m]) == 0) {
                            *month = m + 1;
                            break;
                        }
                    }
                    return 0; // 解析成功�?
                }
            }
        }
        delay_ms(1000); // 没拿到，�?1秒再�?
    }
    return -1; // 彻底失败
}