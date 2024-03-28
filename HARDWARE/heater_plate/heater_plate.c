#include "heater_plate.h"
#include "printf.h"

void HEATER_PLATE_Config(void)
{
	//定义结构体
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//打开时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	//配置和初始化引脚
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; //必须使用上拉电阻GPIO_PuPd_UP 使电平默认为高电平1 
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_SetBits(GPIOC,GPIO_Pin_7);

}


void HEATER_PLATE_Heating(void)
{
//	printf("heating\r\n");
	GPIO_ResetBits(GPIOC,GPIO_Pin_7);

}

void HEATER_PLATE_Stop(void)
{
//	printf("stop heating\r\n");
	GPIO_SetBits(GPIOC,GPIO_Pin_7);

}

