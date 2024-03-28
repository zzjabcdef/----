#include "Raindrop_detection.h"
#include "printf.h"
#include "delay.h"
#include "jq8900.h"
#include "steering.h"
#include "bluetooth.h"
#include "lcd.h"
void RAINDROP_DETECTION_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
//	EXTI_InitTypeDef   EXTI_InitStructure;
//	NVIC_InitTypeDef   NVIC_InitStructure;
	
	//1.��ʱ��
	RCC_AHB1PeriphClockCmd(RAINDROP_DETECTION_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	//3.����GPIO���Ų���ʼ��
	GPIO_InitStructure.GPIO_Pin 	= RAINDROP_DETECTION_PIN;				//���ű��
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN;			//����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//�������
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP ;		//��������
	GPIO_Init(RAINDROP_DETECTION_PORT, &GPIO_InitStructure); 

	//�����ⲿ�ж�ͨ��
//	SYSCFG_EXTILineConfig(RAINDROP_DETECTION_EXTI, RAINDROP_DETECTION_EXTI_SOURCE);
//	
//	//�����ⲿ�ж�
//	EXTI_InitStructure.EXTI_Line = RAINDROP_DETECTION_EXTI_LINE;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);
//	
//	//����NVIC
//	NVIC_InitStructure.NVIC_IRQChannel = RAINDROP_DETECTION_EXTI_IRQ;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);

}

//void EXTI9_5_IRQnHandler(void)
//{
//	if(EXTI_GetITStatus(EXTI_Line6) != RESET)
//	{
//		EXTI_ClearITPendingBit(EXTI_Line6);
//		printf("no water\r\n");
//		// ��⵽ûˮ��������ʾ
//		JQ8900_PlayThirdlyVoice();
//		
//		TIM_SetCompare1(TIM12,5);
//		angle = 5;
//		printf("angle:%d\r\n",angle-4);
//	}
//}

void RAINDROP_DETECTION_ReadInputBits(void)
{
	if(GPIO_ReadInputDataBit(RAINDROP_DETECTION_PORT,RAINDROP_DETECTION_PIN) != RESET)
	{
//		printf("no water\r\n");
		// ��⵽ûˮ��������ʾ
		JQ8900_PlayThirdlyVoice();
		TIM_SetCompare1(TIM12,5);
		angle = 5;
//		printf("angle:%d\r\n",angle-4);
		POINT_COLOR=BLUE;
		LCD_ShowString(30,150,200,16,16,(u8 *)"no water");
		USART_SendString("��Һ���\r\n");

	}
	else
	{
		POINT_COLOR=BLUE;
		LCD_ShowString(30,150,200,16,16,(u8 *)"shuye ing");
	}
}
