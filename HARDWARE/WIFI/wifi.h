#ifndef _WIFI_H
#define _WIFI_H

#include "stm32f4xx.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "delay.h"
#include "printf.h"
#include "usart1.h"

//WiFi����ģʽ��ѡ���Ծ�����APģʽ����������STA����ģʽ������
#define	WIFI_AP				0

//���WIFI�ȵ�궨�壬�˴������Լ���wifi������
#define WIFI_SSID 			"iPhone"
#define WIFI_PASSWORD		"A1234567"  

//��������
extern volatile uint32_t esp8266_transparent_transmission_sta;

extern uint8_t	 Tx3Buffer[512];
extern volatile uint32_t	Rx3Counter;
extern volatile uint8_t	Rx3Data;
extern volatile uint8_t	Rx3End;
extern volatile uint8_t	Rx3Buffer[512];

//��������
void WIFI_Config(u32 baud);
void WIFI_SendString(char *str);

void	esp8266_init(void);
void 	esp8266_send_bytes(uint8_t *buf,uint8_t len);
void 	esp8266_send_str(char *buf);
void 	esp8266_send_at(char *str);
int32_t	esp8266_self_test(void);
int32_t	esp8266_exit_transparent_transmission (void);
int32_t	esp8266_entry_transparent_transmission(void);
int32_t	esp8266_connect_ap(char* ssid,char* pswd);
int32_t	esp8266_connect_server(char* mode,char* ip,uint16_t port);
int32_t	esp8266_disconnect_server(void);
int32_t esp8266_find_str_in_rx_packet(char *str,uint32_t timeout);
int32_t esp8266_check_connection_status(void);
int32_t esp8266_enable_multiple_id(uint32_t b);
int32_t	esp8266_create_server(uint16_t port);
int32_t	esp8266_close_server(uint16_t port);
int32_t	esp8266_enable_echo(uint32_t b);
int32_t	esp8266_reset(void);
int32_t esp8266_check(void);
int32_t Esp8266_Init(void);

void Wifi_Init(void);
void wifisend(char *buf,int num);
void USART3_sendlenth(USART_TypeDef* USARTx, uint8_t *Data,uint8_t Len);
void USART3_sendstr(USART_TypeDef* USARTx, char *Data);

#endif
