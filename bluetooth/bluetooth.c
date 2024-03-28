#include "bluetooth.h"
#include "lcd.h"
#include <string.h>
#include <stdio.h>

#define BLE_BUFSIZE 256
static volatile char ble_buf[BLE_BUFSIZE];
static volatile int ble_cnt;
static volatile char ble_bufflags;

//������ʼ��
void BLUETOOTH_Config(u32 baud)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//1.��GPIO�˿�
	RCC_AHB1PeriphClockCmd(GPIO_CLK, ENABLE);

	//2.�򿪴���ʱ��
	RCC_APB1PeriphClockCmd(GPIO_USART2_CLK, ENABLE);

	//3.ѡ�����ŵĸ��ù���
	GPIO_PinAFConfig(GPIOA, GPIO_AF_TX_PIN  , GPIO_AF_USART);
	GPIO_PinAFConfig(GPIOA, GPIO_AF_RX_PIN  , GPIO_AF_USART);

	//4.����GPIO���Ų�������ʼ��
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;			 			//����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 			//����ٶ�
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		 			//���츴��
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;			 			//��������
	GPIO_InitStructure.GPIO_Pin   = GPIO_PIN	//���ű��
	GPIO_Init(GPIO_PORT, &GPIO_InitStructure);

	//5.����USART1�Ĳ�������ʼ��
	USART_InitStructure.USART_BaudRate 		= baud;										//������
	USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;		//����λ
	USART_InitStructure.USART_StopBits 		= USART_StopBits_1;				//ֹͣλ
	USART_InitStructure.USART_Parity 			= USART_Parity_No;				//����λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ
	USART_Init(USART, &USART_InitStructure);

	//6.�����жϲ�������ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = NVIC_IRQ;								//�ж�ͨ�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_PPRIORITY;				//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_SPRIORITY;							//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//���ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);

	//7.ѡ���ж�Դ   ���յ������򴥷��ж�
	USART_ITConfig(USART, USART_IT_RXNE, ENABLE);

	//8.��USART2
	USART_Cmd(USART, ENABLE);
	
}



//�����ַ����ĺ���
void USART_SendString(char * str)
{ 
	//ѭ�������ַ�
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
	//�ж��ж��Ƿ񴥷�
	if( USART_GetITStatus(USART, USART_IT_RXNE) == SET )
	{
		if(ble_cnt < BLE_BUFSIZE)
		{
			ble_buf[ble_cnt] = USART_ReceiveData(USART); //һ��ֻ�ܽ���1���ֽ�
			USART_SendData(USART1,ble_buf[ble_cnt]);      //ͨ��USART2����1�ֽ�
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
	//�ж��ж��Ƿ񴥷�
	if( USART_GetITStatus(USART, USART_IT_RXNE) == SET )
	{
		if(ble_cnt < BLE_BUFSIZE)
		{
			ble_buf[ble_cnt] = USART_ReceiveData(USART); //һ��ֻ�ܽ���1���ֽ�
			USART_SendData(USART1,ble_buf[ble_cnt]);      //ͨ��USART2����1�ֽ�
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
