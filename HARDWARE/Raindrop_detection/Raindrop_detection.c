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
	
	//1.打开时钟
	RCC_AHB1PeriphClockCmd(RAINDROP_DETECTION_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	//3.配置GPIO引脚并初始化
	GPIO_InitStructure.GPIO_Pin 	= RAINDROP_DETECTION_PIN;				//引脚编号
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN;			//复用模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//输出速率
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP ;		//上拉电阻
	GPIO_Init(RAINDROP_DETECTION_PORT, &GPIO_InitStructure); 

	//连接外部中断通道
//	SYSCFG_EXTILineConfig(RAINDROP_DETECTION_EXTI, RAINDROP_DETECTION_EXTI_SOURCE);
//	
//	//配置外部中断
//	EXTI_InitStructure.EXTI_Line = RAINDROP_DETECTION_EXTI_LINE;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);
//	
//	//配置NVIC
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
//		// 检测到没水，语音提示
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
		// 检测到没水，语音提示
		JQ8900_PlayThirdlyVoice();
		TIM_SetCompare1(TIM12,5);
		angle = 5;
//		printf("angle:%d\r\n",angle-4);
		POINT_COLOR=BLUE;
		LCD_ShowString(30,150,200,16,16,(u8 *)"no water");
		USART_SendString("输液完成\r\n");

	}
	else
	{
		POINT_COLOR=BLUE;
		LCD_ShowString(30,150,200,16,16,(u8 *)"shuye ing");
	}
}
