#include "wifi.h"

//定义全局变量
volatile uint32_t esp8266_transparent_transmission_sta=0;

uint8_t	 Tx3Buffer[512];
volatile uint32_t	Rx3Counter	= 0;
volatile uint8_t	Rx3Data 	= 0;
volatile uint8_t	Rx3End 		= 0;
volatile uint8_t	Rx3Buffer[512]={0};


#define  BLE_BUFFERSIZE  256					//接收缓冲区的大小
static __IO char  wifi_buffer[BLE_BUFFERSIZE];  //作为WIFI模块的接收缓冲区
static __IO int  wifi_cnt=0;					//作为WIFI模块的接收计数器
static __IO uint8_t wifi_recvflag = 0;			//作为WIFI模块的接收标志位

/******************************WIFI初始化*********************************/
/*							  WIFI的初始化 						    	 */
/*************8*******！！！波特率一定是115200！！！**********************/
void WIFI_Config(u32 baud)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//1.打开GPIO端口   
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

	//2.打开串口时钟  USART3 -- APB1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	//3.选择引脚的复用功能
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10  , GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11  , GPIO_AF_USART3);

	//4.配置GPIO引脚参数并初始化
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;			 			//复用模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 			//输出速度
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		 			//推挽复用
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;			 			//上拉电阻
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10|GPIO_Pin_11;	//引脚编号
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	

	//5.配置 USART3 的参数并初始化
	USART_InitStructure.USART_BaudRate 		= baud;										//波特率
	USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;		//数据位
	USART_InitStructure.USART_StopBits 		= USART_StopBits_1;				//停止位
	USART_InitStructure.USART_Parity 			= USART_Parity_No;				//检验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件流控
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
	USART_Init(USART3, &USART_InitStructure);

	//6.配置中断参数并初始化
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;								//中断通道编号
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;				//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;							//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//打开中断通道
	NVIC_Init(&NVIC_InitStructure);

	//7.选择中断源   接收到数据则触发中断
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	//8.打开 USART3
	USART_Cmd(USART3, ENABLE);
	
}

//发送字符串的函数
void WIFI_SendString(char *str)
{ 
	//循环发送字符
	while( *str != '\0' )
	{
		USART_SendData(USART3,*str++);
		while( USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET ); //等待发送数据寄存器为空  表示发送完成		
	}
}
/******************************WIFI初始化******************************************************************/
/******************************串口4中断*******************************************************************/
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
	
	//清除标志位
	USART_ClearITPendingBit(USART3, USART_IT_RXNE);
}
/******************************串口4中断*******************************************************************/

void SEND_USART1(void)
{
	printf("wifi_buffer:%s\r\n",wifi_buffer);
	memset((char *)wifi_buffer,0,sizeof(wifi_buffer));
	wifi_cnt = 0;
}

/*****************  发送一个字节 **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
	//USART_ClearFlag(pUSARTx,USART_FLAG_TXE);
}

/*****************  发送指定长度的字节 **********************/
void Usart_SendBytes(USART_TypeDef * pUSARTx, uint8_t *buf,uint32_t len)
{
	uint8_t *p = buf;
	
	while(len--)
	{
		USART_SendData(pUSARTx,*p);
		
		p++;
		
		//等待数据发送成功
		while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE)==RESET);
		USART_ClearFlag(pUSARTx, USART_FLAG_TXE);
	}
}

/****************** 发送8位的数组 ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
	uint8_t i;

	for(i=0; i<num; i++)
	{
		/* 发送一个字节数据到USART */
		Usart_SendByte(pUSARTx,array[i]);	
	}
	/* 等待发送完成 */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
	//USART_ClearFlag(pUSARTx,USART_FLAG_TC);
}

/*****************  发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
	do 
	{
		Usart_SendByte( pUSARTx, *(str + k) );
		k++;
	} while(*(str + k)!='\0');

	/* 等待发送完成 */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
	USART_ClearFlag(pUSARTx,USART_FLAG_TC);
}

/*****************  发送一个16位数 **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;

	/* 取出高八位 */
	temp_h = (ch&0XFF00)>>8;
	/* 取出低八位 */
	temp_l = ch&0XFF;

	/* 发送高八位 */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);

	/* 发送低八位 */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}


/*******************************AT指令*********************************************************************/
/* 配置WiFi连接热点 */
int32_t Esp8266_Init(void)
{
	int32_t ret;
	//退出透传模式，才能输入AT指令
	ret=esp8266_exit_transparent_transmission();
	if(ret)
	{
		printf("esp8266_exit_transparent_transmission fail\r\n");
		return -1;
	}	
	printf("esp8266_exit_transparent_transmission success\r\n");
	Delay_ms(500);
	Delay_ms(500);
	//复位模块
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
	
	//检查ESP8266是否正常
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
	
	//关闭回显
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

	//连接热点
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

//发送AT指令
void esp8266_send_at(char *str)
{

	//清空接收缓冲区
	memset((void *)Rx3Buffer,0, sizeof Rx3Buffer);

	//清空接收计数值
	Rx3Counter = 0;	

	//串口3发送数据
	Usart_SendString(USART3, str);
}

//发送字节
void esp8266_send_bytes(uint8_t *buf,uint8_t len)
{
	Usart_SendBytes(USART3, buf,len);
}

//发送字符串
void esp8266_send_str(char *buf)
{
	Usart_SendString(USART3, buf);
}

/* 查找接收数据包中的字符串 */
int32_t esp8266_find_str_in_rx_packet(char *str,uint32_t timeout)
{
	char *dest = str;
	char *src  = (char *)&Rx3Buffer;
	
	//等待串口接收完毕或超时退出，strstr()寻找相应字符串，如果未找到则返回 Null;
	while((strstr(src,dest)==NULL) && timeout) //while(找到了 ！=  NULL && timeout == 0),退出循环；
	{		
			Delay_ms(1);
	
		timeout--;
	}

	if(timeout) 
		return 0;	//查找到了相关数据
		                    
	return -1; 
}

/* 检查ESP8266是否正常 */
int32_t esp8266_check(void)
{
	esp8266_send_at("AT\r\n");
	
	if(esp8266_find_str_in_rx_packet("OK",10000))
		return -1;

	return 0;
}

/* 复位 */
int32_t esp8266_reset(void)
{
	esp8266_send_at("AT+RST\r\n");
	
	if(esp8266_find_str_in_rx_packet("OK",10000))
		return -1;

	return 0;
}

/* 回显打开或关闭 */
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

/* 退出透传模式 */
int32_t esp8266_exit_transparent_transmission (void)
{
	esp8266_send_at ("+++");
	//退出透传模式，发送下一条AT指令要间隔1秒
	Delay_ms(500);
	Delay_ms(500);

	//记录当前esp8266工作在非透传模式
	esp8266_transparent_transmission_sta = 0;

	return 0;
}

/* 进入透传模式 */
int32_t esp8266_entry_transparent_transmission(void)
{
	//进入透传模式
	esp8266_send_at("AT+CIPMODE=1\r\n");  
	if(esp8266_find_str_in_rx_packet("OK",5000))
		return -1;
	Delay_ms(500);
	Delay_ms(500);
	Delay_ms(500);
	Delay_ms(500);

	//开启发送状态
	esp8266_send_at("AT+CIPSEND\r\n");
	if(esp8266_find_str_in_rx_packet("OK",5000))
		return -2;
	
	Delay_ms(500);
	Delay_ms(500);
	Delay_ms(500);
	Delay_ms(500);
	//记录当前esp8266工作在透传模式
	esp8266_transparent_transmission_sta = 1;
	return 0;
}

/* 检测连接状态 */
int32_t esp8266_check_connection_status(void)
{
	esp8266_send_at("AT+CIPSTATUS\r\n");
	
	if(esp8266_find_str_in_rx_packet("STATUS",10000))
		if(esp8266_find_str_in_rx_packet("OK",10000))
			return -1;

	return 0;
}

/**
 * 功能：连接热点
 * 参数：
 *         ssid:热点名
 *         pwd:热点密码
 * 返回值：
 *         连接结果,非0连接成功,0连接失败
 * 说明： 
 *         失败的原因有以下几种(UART通信和ESP8266正常情况下)
 *         1. WIFI名和密码不正确
 *         2. 路由器连接设备太多,未能给ESP8266分配IP
 */
int32_t esp8266_connect_ap(char* ssid,char* pswd)
{
#if 0	
	//不建议使用以下sprintf，占用过多的栈
	char buf[128]={0};
	
	//设置为STATION模式	
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
	//设置为Station模式	
	esp8266_send_at("AT+CWMODE=1\r\n"); 
	if(esp8266_find_str_in_rx_packet("OK",1000))
		return -1;
	
//	esp8266_send_at("AT+CIPMUX=0\r\n");
//	if(esp8266_find_str_in_rx_packet("OK",1000))
//		return -2;

	//连接热点
	esp8266_send_at("AT+CWJAP="); 
	esp8266_send_at("\"");esp8266_send_at(ssid);esp8266_send_at("\"");	
	esp8266_send_at(",");	
	esp8266_send_at("\"");esp8266_send_at(pswd);esp8266_send_at("\"");	
	esp8266_send_at("\r\n");
	//连接热点，务必等待该条指令返回WIFI GOT IP，表示连接成功后，再发送下面的指令；
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
 * 功能：使用指定协议(TCP/UDP)连接到服务器
 * 参数：
 *         mode:协议类型 "TCP","UDP"
 *         ip:目标服务器IP
 *         port:目标是服务器端口号
 * 返回值：
 *         连接结果,非0连接成功,0连接失败
 * 说明： 
 *         失败的原因有以下几种(UART通信和ESP8266正常情况下)
 *         1. 远程服务器IP和端口号有误
 *         2. 未连接AP
 *         3. 服务器端禁止添加(一般不会发生)
 */
int32_t esp8266_connect_server(char* mode,char* ip,uint16_t port)
{

#if 0	
	//使用MQTT传递的ip地址过长，不建议使用以下方法，否则导致栈溢出
	//AT+CIPSTART="TCP","a10tC4OAAPc.iot-as-mqtt.cn-shanghai.aliyuncs.com",1883，该字符串占用内存过多了
	
	char buf[128]={0};
	
	//连接服务器
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

/* 断开服务器 */
int32_t esp8266_disconnect_server(void)
{
	esp8266_send_at("AT+CIPCLOSE\r\n");
		
	if(esp8266_find_str_in_rx_packet("CLOSED",2000))
		if(esp8266_find_str_in_rx_packet("OK",2000))
			return -1;

	return 0;	
}

/* 使能多链接 */
int32_t esp8266_enable_multiple_id(uint32_t b)
{
	char buf[32]={0};
	
	sprintf(buf,"AT+CIPMUX=%d\r\n", b);
	esp8266_send_at(buf);
	
	if(esp8266_find_str_in_rx_packet("OK",5000))
		return -1;
	
	return 0;
}

/* 创建服务器 */
int32_t esp8266_create_server(uint16_t port)
{
	char buf[32]={0};
	
	sprintf(buf,"AT+CIPSERVER=1,%d\r\n", port);
	esp8266_send_at(buf);
	
	if(esp8266_find_str_in_rx_packet("OK",5000))
		return -1;
	
	return 0;
}

/* 关闭服务器 */
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
以下为简单框架设置Esp8266_WiFi模块为AP模式进行工作，即设置AP局域网
*/
void Wifi_Init(void)
{
	USART3_sendstr(USART3,"AT+CWMODE=2\r\n");//设置为 softAP+station 共存模式
	Delay_ms(500);
	
	
	USART3_sendstr(USART3,"AT+RST\r\n");//重启
	Delay_ms(500);
	Delay_ms(500);
	Delay_ms(500);
	Delay_ms(500);
	
	USART3_sendstr(USART3,"AT+CIPAP=\"192.168.1.1\"\r\n");//设置IP:192.168.1.1
	Delay_ms(500);

	USART3_sendstr(USART3,"AT+CWSAP=\"CZJ\",\"12345678\",5,0\r\n");//设置wifi名称是CZJ，密码12345678，最多5个人同时连接，连接时无需密码；
	Delay_ms(500);
	
	USART3_sendstr(USART3,"AT+CIPMUX=1\r\n");//启动多连接
	Delay_ms(500);
	
	USART3_sendstr(USART3,"AT+CIPSERVER=1,8080\r\n");//设置端口8080
	Delay_ms(500);
	
	printf("wifi_init end\n");//串口1输出提示；
}
//wifi模块发送语句---每次固定发送num个字节
void wifisend(char *buf,int num)
{
	//每次wifi模块发送数据的时候，都事先发送一个固定前缀
	char sendfront[32];//定义前缀的缓冲区
	sprintf(sendfront,"AT+CIPSEND=0,%d\r\n",num);//组合字符串
	
	USART3_sendstr(USART3,sendfront);
	Delay_ms(5);
	USART3_sendstr(USART3,buf);
}
//发送len长度的字符串
void USART3_sendlenth(USART_TypeDef* USARTx, uint8_t *Data,uint8_t Len)
{ 
	while(Len--){				                          //判断是否到达字符串结束符
	    USART_SendData(USARTx, *Data++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET); //等待发送完
	}
}
//发送一个完整的字符串
void USART3_sendstr(USART_TypeDef* USARTx, char *Data)
{ 
	//循环发送1个字节，直到准备发送的字节是'\0',也就是字符串末尾，停止发送
	while(*Data!=0){				                        
		USART_SendData(USARTx, *Data);
		Data++;
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
	}
}
