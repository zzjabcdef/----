/*********************************************************************************
**************************STM32F407����Ӧ�ð�*************************************
**********************************************************************************
* �ļ�����: keyboard.c                                                    		 *
* �ļ�������4*4�����������		                                                 *
*    																			 *
*  												                              	 *
**********************************************************************************
*********************************************************************************/	
#include "keyboard.h"
#include "lcd.h"
#include "Raindrop_detection.h"
#include "heater_plate.h"

__IO u8 keyflag1,keyflag2,keyflag3,keyflag4; // ȫ�ֱ���,Ĭ�ϳ�ʼֵΪ0���жϰ����Ƿ�ı䰴��״̬

volatile int angle = 5;	//�洢����Ƕ�
volatile int Temp = 20;	//�洢�¶���ֵ
volatile char str12[32] = {0};
//������ʼ��
void GPIO_KEY_Init(void)
{
	//����ṹ��
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	//��ʱ��
	RCC_AHB1PeriphClockCmd(KEYBOARD_GPIO_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	//���úͳ�ʼ������
	GPIO_InitStruct.GPIO_Pin =  KEYBOARD_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; //����ʹ����������GPIO_PuPd_UP ʹ��ƽĬ��Ϊ�ߵ�ƽ1 
//	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(KEYBOARD_GPIO_PORT, &GPIO_InitStruct);
	
	
	//�����ⲿ�ж�ͨ��
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource2);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource4);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource1);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource3);
	
	//�����ⲿ�ж�
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	//����NVIC
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	

	
}

void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
//		delay_nnms(50);
		delay_nnms(200);
		if(EXTI_GetITStatus(EXTI_Line1) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line1);
			if(Temp<32)
			{
				//����д
				Temp++;
				char str[32];
				sprintf(str,"TEMP_SET : %d",Temp);
				POINT_COLOR=RED;      //������ɫ����ɫ
				LCD_ShowString(30,40,210,24,24,(u8 *)str);
//				if(Temp > temp_now)
//				{
//					//�򿪼���Ƭ
//					HEATER_PLATE_Heating();
//				}
//				else
//				{
//					//�رռ���Ƭ
//					HEATER_PLATE_Stop();
//				}
			}
		}
	}
	

}

void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
	{
//		delay_nnms(50);
		delay_nnms(200);
		if(EXTI_GetITStatus(EXTI_Line3) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line3);
			if(Temp>20)
			{
				//����д
				Temp--;
				char str[32];
				sprintf(str,"TEMP_SET : %d",Temp);
				POINT_COLOR=RED;      //������ɫ����ɫ
				LCD_ShowString(30,40,210,24,24,(u8 *)str);
//				if(Temp > temp_now)
//				{
//					//�򿪼���Ƭ
//				HEATER_PLATE_Heating();
//				}
//				else
//				{
//					//�رռ���Ƭ
//				HEATER_PLATE_Stop();
//				}
			}
		}
	}

}

void EXTI2_IRQHandler(void)
{
//	printf("EXTI2\r\n");
	if(EXTI_GetITStatus(EXTI_Line2) != RESET)
	{
//		delay_nnms(50);
		delay_nnms(200);
		if(EXTI_GetITStatus(EXTI_Line2) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line2);
			
	
			sprintf((char *)str12,"angle : %d",angle-4);
			POINT_COLOR=BLUE;
			LCD_ShowString(30,130,200,16,16,(u8 *)str12);
			
			STEERING_ENGINE_Foreward(1);
			
		}
		
	}

}


void EXTI4_IRQHandler(void)
{
//	printf("EXTI4\r\n");
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
	{
//		delay_nnms(50);
		delay_nnms(200);
		if(EXTI_GetITStatus(EXTI_Line4) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line4);
		
			
			sprintf((char *)str12,"angle : %d",angle-4);
			POINT_COLOR=BLUE;
			LCD_ShowString(30,130,200,16,16,(u8 *)str12);
			
			STEERING_ENGINE_Foreward(0);

		}
		
	}

}

