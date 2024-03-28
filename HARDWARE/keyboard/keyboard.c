/*********************************************************************************
**************************STM32F407核心应用板*************************************
**********************************************************************************
* 文件名称: keyboard.c                                                    		 *
* 文件简述：4*4按键键盘输出		                                                 *
*    																			 *
*  												                              	 *
**********************************************************************************
*********************************************************************************/	
#include "keyboard.h"
#include "lcd.h"
#include "Raindrop_detection.h"
#include "heater_plate.h"

__IO u8 keyflag1,keyflag2,keyflag3,keyflag4; // 全局变量,默认初始值为0，判断按键是否改变按键状态

volatile int angle = 5;	//存储舵机角度
volatile int Temp = 20;	//存储温度阈值
volatile char str12[32] = {0};
//按键初始化
void GPIO_KEY_Init(void)
{
	//定义结构体
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	//打开时钟
	RCC_AHB1PeriphClockCmd(KEYBOARD_GPIO_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	//配置和初始化引脚
	GPIO_InitStruct.GPIO_Pin =  KEYBOARD_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; //必须使用上拉电阻GPIO_PuPd_UP 使电平默认为高电平1 
//	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(KEYBOARD_GPIO_PORT, &GPIO_InitStruct);
	
	
	//连接外部中断通道
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource2);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource4);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource1);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource3);
	
	//配置外部中断
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
	
	//配置NVIC
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
				//在这写
				Temp++;
				char str[32];
				sprintf(str,"TEMP_SET : %d",Temp);
				POINT_COLOR=RED;      //画笔颜色：红色
				LCD_ShowString(30,40,210,24,24,(u8 *)str);
//				if(Temp > temp_now)
//				{
//					//打开加热片
//					HEATER_PLATE_Heating();
//				}
//				else
//				{
//					//关闭加热片
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
				//在这写
				Temp--;
				char str[32];
				sprintf(str,"TEMP_SET : %d",Temp);
				POINT_COLOR=RED;      //画笔颜色：红色
				LCD_ShowString(30,40,210,24,24,(u8 *)str);
//				if(Temp > temp_now)
//				{
//					//打开加热片
//				HEATER_PLATE_Heating();
//				}
//				else
//				{
//					//关闭加热片
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

