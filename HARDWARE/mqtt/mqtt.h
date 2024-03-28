#ifndef __MQTT_H
#define __MQTT_H

//����ͷ�ļ�
#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include "usart1.h"
#include "wifi.h"
#include "cjson.h"

//�궨��
//�˴��ǰ����Ʒ������Ĺ���ʵ����½����------------ע���޸�Ϊ�Լ����Ʒ����豸��Ϣ��������
#define MQTT_BROKERADDRESS		"iot-06z00d9yx7e4ka9.mqtt.iothub.aliyuncs.com"
#define MQTT_CLIENTID			"k0wb4sjctWJ.shuye|securemode=2,signmethod=hmacsha256,timestamp=1710642177674|"
#define MQTT_USARNAME			"shuye&k0wb4sjctWJ"
#define MQTT_PASSWD				"93b42eef8710ad767ff1aa56ad66684d6060b8167c6683e09150e54713db4374"
#define	MQTT_PUBLISH_TOPIC		"/sys/k0wb4sjctWJ/shuye/thing/event/property/post"
#define MQTT_SUBSCRIBE_TOPIC	"/sys/k0wb4sjctWJ/shuye/thing/service/property/set"


#define BYTE0(dwTemp)       (*( char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))
	
#define CONNECT_MQTT_LED(x)	PCout(13)=(x)?0:1

//��������


//��������
int Mqtt_Connect_Aliyun(void);											//����MQTT���Ӱ�����
int32_t mqtt_connect(char *client_id,char *user_name,char *password);	//MQTT���ӷ�����
int32_t mqtt_subscribe_topic(char *topic,uint8_t qos,uint8_t whether);	//MQTT��Ϣ����
uint32_t mqtt_publish_data(char *topic, char *message, uint8_t qos);	//MQTT��Ϣ����
int32_t mqtt_send_heart(void);											//MQTT����������
void mqtt_report_devices_status(int temp,int speed);					//�豸״̬�ϱ�
void mqtt_disconnect(void);												//MQTT�������Ͽ�
void mqtt_send_bytes(uint8_t *buf,uint32_t len);						//MQTT��������
void mqtt_msg_handle(void);												//����MQTT�·�����
uint32_t  mqtt_cjson_parse(char *pbuf);									//����MQTT�·�����
void MQTT_TIM_Init(void);

#endif
