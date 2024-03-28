#include "bluetooth.h"
#include "lcd.h"
#include <string.h>
#include <stdio.h>

#define BLE_BUFSIZE 256
static volatile char ble_buf[BLE_BUFSIZE];
static volatile int ble_cnt;
static volatile char ble_bufflags;

//蓝牙初始化
void BLUETOOTH_Config(u32 baud)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//1.打开GPIO端口
	RCC_AHB1PeriphClockCmd(GPIO_CLK, ENABLE);

	//2.打开串口时钟
	RCC_APB1PeriphClockCmd(GPIO_USART2_CLK, ENABLE);

	//3.选择引脚的复用功能
	GPIO_PinAFConfig(GPIOA, GPIO_AF_TX_PIN  , GPIO_AF_USART);
	GPIO_PinAFConfig(GPIOA, GPIO_AF_RX_PIN  , GPIO_AF_USART);

	//4.配置GPIO引脚参数并初始化
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;			 			//复用模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 			//输出速度
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		 			//推挽复用
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;			 			//上拉电阻
	GPIO_InitStructure.GPIO_Pin   = GPIO_PIN	//引脚编号
	GPIO_Init(GPIO_PORT, &GPIO_InitStructure);

	//5.配置USART1的参数并初始化
	USART_InitStructure.USART_BaudRate 		= baud;										//波特率
	USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;		//数据位
	USART_InitStructure.USART_StopBits 		= USART_StopBits_1;				//停止位
	USART_InitStructure.USART_Parity 			= USART_Parity_No;				//检验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件流控
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
	USART_Init(USART, &USART_InitStructure);

	//6.配置中断参数并初始化
	NVIC_InitStructure.NVIC_IRQChannel = NVIC_IRQ;								//中断通道编号
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_PPRIORITY;				//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_SPRIORITY;							//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//打开中断通道
	NVIC_Init(&NVIC_InitStructure);

	//7.选择中断源   接收到数据则触发中断
	USART_ITConfig(USART, USART_IT_RXNE, ENABLE);

	//8.打开USART2
	USART_Cmd(USART, ENABLE);
	
}



//发送字符串的函数
void USART_SendString(char * str)
{ 
	//循环发送字符
  while( *str != '\0' )
	{
		USART_SendData(USART,*str);
		while( USART_GetFlagStatus(USART,USART_FLAG_TXE) == RESET );
		str++;
	}
}

#if USART2_BLUETOOTH
void USART2_IRQHandler(void)
{
	//判断中断是否触发
	if( USART_GetITStatus(USART, USART_IT_RXNE) == SET )
	{
		if(ble_cnt < BLE_BUFSIZE)
		{
			ble_buf[ble_cnt] = USART_ReceiveData(USART); //一次只能接收1个字节
			USART_SendData(USART1,ble_buf[ble_cnt]);      //通过USART2发送1字节
			if(ble_buf[ble_cnt] == 'q')
			{
				char str1[32];
				sprintf(str1,"angle : %d",angle-4);
				POINT_COLOR=BLUE;
				LCD_ShowString(30,130,200,16,16,(u8 *)str1);
				STEERING_ENGINE_Foreward(0);
				ble_bufflags = 1;
			} 
			if(ble_buf[ble_cnt] == 'w')
			{
				char str1[32];
				sprintf(str1,"angle : %d",angle-4);
				POINT_COLOR=BLUE;
				LCD_ShowString(30,130,200,16,16,(u8 *)str1);
				STEERING_ENGINE_Foreward(1);
				ble_bufflags = 1;
			}
			ble_cnt++;
		}
	}
}
#endif

#if USART3_BLUETOOTH
void USART3_IRQHandler(void)
{
	//判断中断是否触发
	if( USART_GetITStatus(USART, USART_IT_RXNE) == SET )
	{
		if(ble_cnt < BLE_BUFSIZE)
		{
			ble_buf[ble_cnt] = USART_ReceiveData(USART); //一次只能接收1个字节
			USART_SendData(USART1,ble_buf[ble_cnt]);      //通过USART2发送1字节
			if(ble_buf[ble_cnt] == '-')
			{
				STEERING_ENGINE_Foreward(0);
				ble_bufflags = 1;
			}
			if(ble_buf[ble_cnt] == '+')
			{
				STEERING_ENGINE_Foreward(1);
				ble_bufflags = 1;
			}
			ble_cnt++;
		}
	}
}
#endif
