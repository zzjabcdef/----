#include "through-beam.h"
#include "lcd.h"
#include "delay.h"

volatile int through_num = 0;

void THROUGH_BEAM_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	
	RCC_AHB1PeriphClockCmd(THROUGH_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	GPIO_InitStructure.GPIO_Pin =  THROUGH_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(THROUGH_PORT, &GPIO_InitStructure);

	
	//�����ⲿ�ж�ͨ��
	SYSCFG_EXTILineConfig(THROUGH_EXTI_GPIO, THROUGH_EXTI_PIN);
	
	//�����ⲿ�ж�
	EXTI_InitStructure.EXTI_Line = THROUGH_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

		
	//����NVIC
	NVIC_InitStructure.NVIC_IRQChannel = THROUGH_EXTI_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

//ʹ�ö�ʱ���ж�ÿ���жϲ鿴DO���Ŵ����˶��ٴ��ⲿ�жϣ�
//���ܵõ���Һ��Ƶ�ʣ������ʣ�
//ͨ����Ϊѡ����ֵ���жϵ�Һ�ǿ컹����

void EXTI0_IRQHandler(void)
{

	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		delay_nnms(500);
		if(EXTI_GetITStatus(EXTI_Line0) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line0);
		
			through_num++;

		}
	}
}



