#include "wifi.h"

//����ȫ�ֱ���
volatile uint32_t esp8266_transparent_transmission_sta=0;

uint8_t	 Tx3Buffer[512];
volatile uint32_t	Rx3Counter	= 0;
volatile uint8_t	Rx3Data 	= 0;
volatile uint8_t	Rx3End 		= 0;
volatile uint8_t	Rx3Buffer[512]={0};


#define  BLE_BUFFERSIZE  256					//���ջ������Ĵ�С
static __IO char  wifi_buffer[BLE_BUFFERSIZE];  //��ΪWIFIģ��Ľ��ջ�����
static __IO int  wifi_cnt=0;					//��ΪWIFIģ��Ľ��ռ�����
static __IO uint8_t wifi_recvflag = 0;			//��ΪWIFIģ��Ľ��ձ�־λ

/******************************WIFI��ʼ��*********************************/
/*							  WIFI�ĳ�ʼ�� 						    	 */
/*************8*******������������һ����115200������**********************/
void WIFI_Config(u32 baud)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//1.��GPIO�˿�   
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

	//2.�򿪴���ʱ��  USART3 -- APB1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	//3.ѡ�����ŵĸ��ù���
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10  , GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11  , GPIO_AF_USART3);

	//4.����GPIO���Ų�������ʼ��
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;			 			//����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 			//����ٶ�
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		 			//���츴��
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;			 			//��������
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10|GPIO_Pin_11;	//���ű��
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	

	//5.���� USART3 �Ĳ�������ʼ��
	USART_InitStructure.USART_BaudRate 		= baud;										//������
	USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;		//����λ
	USART_InitStructure.USART_StopBits 		= USART_StopBits_1;				//ֹͣλ
	USART_InitStructure.USART_Parity 			= USART_Parity_No;				//����λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ
	USART_Init(USART3, &USART_InitStructure);

	//6.�����жϲ�������ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;								//�ж�ͨ�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;				//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;							//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//���ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);

	//7.ѡ���ж�Դ   ���յ������򴥷��ж�
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	//8.�� USART3
	USART_Cmd(USART3, ENABLE);
	
}

//�����ַ����ĺ���
void WIFI_SendString(char *str)
{ 
	//ѭ�������ַ�
	while( *str != '\0' )
	{
		USART_SendData(USART3,*str++);
		while( USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET ); //�ȴ��������ݼĴ���Ϊ��  ��ʾ�������		
	}
}
/******************************WIFI��ʼ��******************************************************************/
/******************************����4�ж�*******************************************************************/
void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		/* Read one byte from the receive data register */
		Rx3Data = USART_ReceiveData(USART3);
//printf("data:%c",Rx3Data);
		Rx3Buffer[Rx3Counter++] = Rx3Data;
//		uart1SendChar(Rx3Data);
		if(Rx3Counter >= sizeof( Rx3Buffer))
		{
			Rx3Counter = 0;
			Rx3End=1;
		}
	}
	
	//�����־λ
	USART_ClearITPendingBit(USART3, USART_IT_RXNE);
}
/******************************����4�ж�*******************************************************************/

void SEND_USART1(void)
{
	printf("wifi_buffer:%s\r\n",wifi_buffer);
	memset((char *)wifi_buffer,0,sizeof(wifi_buffer));
	wifi_cnt = 0;
}

/*****************  ����һ���ֽ� **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
	//USART_ClearFlag(pUSARTx,USART_FLAG_TXE);
}

/*****************  ����ָ�����ȵ��ֽ� **********************/
void Usart_SendBytes(USART_TypeDef * pUSARTx, uint8_t *buf,uint32_t len)
{
	uint8_t *p = buf;
	
	while(len--)
	{
		USART_SendData(pUSARTx,*p);
		
		p++;
		
		//�ȴ����ݷ��ͳɹ�
		while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE)==RESET);
		USART_ClearFlag(pUSARTx, USART_FLAG_TXE);
	}
}

/****************** ����8λ������ ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
	uint8_t i;

	for(i=0; i<num; i++)
	{
		/* ����һ���ֽ����ݵ�USART */
		Usart_SendByte(pUSARTx,array[i]);	
	}
	/* �ȴ�������� */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
	//USART_ClearFlag(pUSARTx,USART_FLAG_TC);
}

/*****************  �����ַ��� **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
	do 
	{
		Usart_SendByte( pUSARTx, *(str + k) );
		k++;
	} while(*(str + k)!='\0');

	/* �ȴ�������� */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
	USART_ClearFlag(pUSARTx,USART_FLAG_TC);
}

/*****************  ����һ��16λ�� **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;

	/* ȡ���߰�λ */
	temp_h = (ch&0XFF00)>>8;
	/* ȡ���Ͱ�λ */
	temp_l = ch&0XFF;

	/* ���͸߰�λ */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);

	/* ���͵Ͱ�λ */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}


/*******************************ATָ��*********************************************************************/
/* ����WiFi�����ȵ� */
int32_t Esp8266_Init(void)
{
	int32_t ret;
	//�˳�͸��ģʽ����������ATָ��
	ret=esp8266_exit_transparent_transmission();
	if(ret)
	{
		printf("esp8266_exit_transparent_transmission fail\r\n");
		return -1;
	}	
	printf("esp8266_exit_transparent_transmission success\r\n");
	Delay_ms(500);
	Delay_ms(500);
	//��λģ��
	ret = esp8266_reset();
	if(ret)
	{
		printf("esp8266_reset fail\r\n");
		return -2;
	}
	printf("esp8266_reset success\r\n");
	Delay_ms(500);
	Delay_ms(500);
	Delay_ms(500);
	Delay_ms(500);
	
	//���ESP8266�Ƿ�����
	ret=esp8266_check();
	if(ret)
	{
		printf("esp8266_check fail\r\n");
		return -3;
	}
	printf("esp8266_check success\r\n");
	Delay_ms(500);
	Delay_ms(500);
	Delay_ms(500);
	Delay_ms(500);
	
	//�رջ���
	ret=esp8266_enable_echo(0);
	if(ret)
	{
		printf("esp8266_enable_echo(0) fail\r\n");
		return -4;
	}	
	printf("esp8266_enable_echo(0)success\r\n");
	Delay_ms(500);
	Delay_ms(500);
	Delay_ms(500);
	Delay_ms(500);

	//�����ȵ�
	ret = esp8266_connect_ap(WIFI_SSID,WIFI_PASSWORD);
	if(ret)
	{
		printf("esp8266_connect_ap fail\r\n");
		return -5;
	}	
	printf("esp8266_connect_ap success\r\n");
	Delay_ms(500);
	Delay_ms(500);
	Delay_ms(500);
	Delay_ms(500);
	return 0;
}

//����ATָ��
void esp8266_send_at(char *str)
{

	//��ս��ջ�����
	memset((void *)Rx3Buffer,0, sizeof Rx3Buffer);

	//��ս��ռ���ֵ
	Rx3Counter = 0;	

	//����3��������
	Usart_SendString(USART3, str);
}

//�����ֽ�
void esp8266_send_bytes(uint8_t *buf,uint8_t len)
{
	Usart_SendBytes(USART3, buf,len);
}

//�����ַ���
void esp8266_send_str(char *buf)
{
	Usart_SendString(USART3, buf);
}

/* ���ҽ������ݰ��е��ַ��� */
int32_t esp8266_find_str_in_rx_packet(char *str,uint32_t timeout)
{
	char *dest = str;
	char *src  = (char *)&Rx3Buffer;
	
	//�ȴ����ڽ�����ϻ�ʱ�˳���strstr()Ѱ����Ӧ�ַ��������δ�ҵ��򷵻� Null;
	while((strstr(src,dest)==NULL) && timeout) //while(�ҵ��� ��=  NULL && timeout == 0),�˳�ѭ����
	{		
			Delay_ms(1);
	
		timeout--;
	}

	if(timeout) 
		return 0;	//���ҵ����������
		                    
	return -1; 
}

/* ���ESP8266�Ƿ����� */
int32_t esp8266_check(void)
{
	esp8266_send_at("AT\r\n");
	
	if(esp8266_find_str_in_rx_packet("OK",10000))
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

/* ���Դ򿪻�ر� */
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

/* �˳�͸��ģʽ */
int32_t esp8266_exit_transparent_transmission (void)
{
	esp8266_send_at ("+++");
	//�˳�͸��ģʽ��������һ��ATָ��Ҫ���1��
	Delay_ms(500);
	Delay_ms(500);

	//��¼��ǰesp8266�����ڷ�͸��ģʽ
	esp8266_transparent_transmission_sta = 0;

	return 0;
}

/* ����͸��ģʽ */
int32_t esp8266_entry_transparent_transmission(void)
{
	//����͸��ģʽ
	esp8266_send_at("AT+CIPMODE=1\r\n");  
	if(esp8266_find_str_in_rx_packet("OK",5000))
		return -1;
	Delay_ms(500);
	Delay_ms(500);
	Delay_ms(500);
	Delay_ms(500);

	//��������״̬
	esp8266_send_at("AT+CIPSEND\r\n");
	if(esp8266_find_str_in_rx_packet("OK",5000))
		return -2;
	
	Delay_ms(500);
	Delay_ms(500);
	Delay_ms(500);
	Delay_ms(500);
	//��¼��ǰesp8266������͸��ģʽ
	esp8266_transparent_transmission_sta = 1;
	return 0;
}

/* �������״̬ */
int32_t esp8266_check_connection_status(void)
{
	esp8266_send_at("AT+CIPSTATUS\r\n");
	
	if(esp8266_find_str_in_rx_packet("STATUS",10000))
		if(esp8266_find_str_in_rx_packet("OK",10000))
			return -1;

	return 0;
}

/**
 * ���ܣ������ȵ�
 * ������
 *         ssid:�ȵ���
 *         pwd:�ȵ�����
 * ����ֵ��
 *         ���ӽ��,��0���ӳɹ�,0����ʧ��
 * ˵���� 
 *         ʧ�ܵ�ԭ�������¼���(UARTͨ�ź�ESP8266���������)
 *         1. WIFI�������벻��ȷ
 *         2. ·���������豸̫��,δ�ܸ�ESP8266����IP
 */
int32_t esp8266_connect_ap(char* ssid,char* pswd)
{
#if 0	
	//������ʹ������sprintf��ռ�ù����ջ
	char buf[128]={0};
	
	//����ΪSTATIONģʽ	
	esp8266_send_at("AT+CWMODE_CUR=1\r\n"); 
	if(esp8266_find_str_in_rx_packet("OK",10000))
		return -1;
	esp8266_send_at("AT+CIPMUX=0\r\n");
	if(esp8266_find_str_in_rx_packet("OK",1000))
		return -2;
	sprintf(buf,"AT+CWJAP_CUR=\"%s\",\"%s\"\r\n",ssid,pswd);
	esp8266_send_at(buf); 
	if(esp8266_find_str_in_rx_packet("OK",5000))
		if(esp8266_find_str_in_rx_packet("CONNECT",5000))
			return -2;
#else
	//����ΪStationģʽ	
	esp8266_send_at("AT+CWMODE=1\r\n"); 
	if(esp8266_find_str_in_rx_packet("OK",1000))
		return -1;
	
//	esp8266_send_at("AT+CIPMUX=0\r\n");
//	if(esp8266_find_str_in_rx_packet("OK",1000))
//		return -2;

	//�����ȵ�
	esp8266_send_at("AT+CWJAP="); 
	esp8266_send_at("\"");esp8266_send_at(ssid);esp8266_send_at("\"");	
	esp8266_send_at(",");	
	esp8266_send_at("\"");esp8266_send_at(pswd);esp8266_send_at("\"");	
	esp8266_send_at("\r\n");
	//�����ȵ㣬��صȴ�����ָ���WIFI GOT IP����ʾ���ӳɹ����ٷ��������ָ�
//	while(esp8266_find_str_in_rx_packet("CONNECTED",5000));
	if(esp8266_find_str_in_rx_packet("CONNECTED",5000))
		esp8266_connect_ap(ssid,pswd);
		Delay_ms(500);
	Delay_ms(500);
	Delay_ms(500);
	Delay_ms(500);
	Delay_ms(500);
	Delay_ms(500);



#endif
	return 0;
}

/**
 * ���ܣ�ʹ��ָ��Э��(TCP/UDP)���ӵ�������
 * ������
 *         mode:Э������ "TCP","UDP"
 *         ip:Ŀ�������IP
 *         port:Ŀ���Ƿ������˿ں�
 * ����ֵ��
 *         ���ӽ��,��0���ӳɹ�,0����ʧ��
 * ˵���� 
 *         ʧ�ܵ�ԭ�������¼���(UARTͨ�ź�ESP8266���������)
 *         1. Զ�̷�����IP�Ͷ˿ں�����
 *         2. δ����AP
 *         3. �������˽�ֹ���(һ�㲻�ᷢ��)
 */
int32_t esp8266_connect_server(char* mode,char* ip,uint16_t port)
{

#if 0	
	//ʹ��MQTT���ݵ�ip��ַ������������ʹ�����·�����������ջ���
	//AT+CIPSTART="TCP","a10tC4OAAPc.iot-as-mqtt.cn-shanghai.aliyuncs.com",1883�����ַ���ռ���ڴ������
	
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
		
	if(esp8266_find_str_in_rx_packet("CLOSED",2000))
		if(esp8266_find_str_in_rx_packet("OK",2000))
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


/*
����Ϊ�򵥿������Esp8266_WiFiģ��ΪAPģʽ���й�����������AP������
*/
void Wifi_Init(void)
{
	USART3_sendstr(USART3,"AT+CWMODE=2\r\n");//����Ϊ softAP+station ����ģʽ
	Delay_ms(500);
	
	
	USART3_sendstr(USART3,"AT+RST\r\n");//����
	Delay_ms(500);
	Delay_ms(500);
	Delay_ms(500);
	Delay_ms(500);
	
	USART3_sendstr(USART3,"AT+CIPAP=\"192.168.1.1\"\r\n");//����IP:192.168.1.1
	Delay_ms(500);

	USART3_sendstr(USART3,"AT+CWSAP=\"CZJ\",\"12345678\",5,0\r\n");//����wifi������CZJ������12345678�����5����ͬʱ���ӣ�����ʱ�������룻
	Delay_ms(500);
	
	USART3_sendstr(USART3,"AT+CIPMUX=1\r\n");//����������
	Delay_ms(500);
	
	USART3_sendstr(USART3,"AT+CIPSERVER=1,8080\r\n");//���ö˿�8080
	Delay_ms(500);
	
	printf("wifi_init end\n");//����1�����ʾ��
}
//wifiģ�鷢�����---ÿ�ι̶�����num���ֽ�
void wifisend(char *buf,int num)
{
	//ÿ��wifiģ�鷢�����ݵ�ʱ�򣬶����ȷ���һ���̶�ǰ׺
	char sendfront[32];//����ǰ׺�Ļ�����
	sprintf(sendfront,"AT+CIPSEND=0,%d\r\n",num);//����ַ���
	
	USART3_sendstr(USART3,sendfront);
	Delay_ms(5);
	USART3_sendstr(USART3,buf);
}
//����len���ȵ��ַ���
void USART3_sendlenth(USART_TypeDef* USARTx, uint8_t *Data,uint8_t Len)
{ 
	while(Len--){				                          //�ж��Ƿ񵽴��ַ���������
	    USART_SendData(USARTx, *Data++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET); //�ȴ�������
	}
}
//����һ���������ַ���
void USART3_sendstr(USART_TypeDef* USARTx, char *Data)
{ 
	//ѭ������1���ֽڣ�ֱ��׼�����͵��ֽ���'\0',Ҳ�����ַ���ĩβ��ֹͣ����
	while(*Data!=0){				                        
		USART_SendData(USARTx, *Data);
		Data++;
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
	}
}
