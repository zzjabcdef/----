#include "jq8900.h"

//JQ8900-16P初始化
void JQ8900_Config(void)
{
	//定义结构体
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//打开时钟
	RCC_AHB1PeriphClockCmd(JQ8900_GPIO_CLK, ENABLE);
	
	//配置和初始化引脚
	GPIO_InitStruct.GPIO_Pin =  JQ8900_GPIO_PIN1|JQ8900_GPIO_PIN2|JQ8900_GPIO_PIN3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; //必须使用上拉电阻GPIO_PuPd_UP 使电平默认为高电平1 
//	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(JQ8900_GPIO_PORT, &GPIO_InitStruct);

}


//播放第一首
void JQ8900_PlayFirstVoice(void)
{
	GPIO_ResetBits(JQ8900_GPIO_PORT, JQ8900_GPIO_PIN1);
	
	
	//1s语音
	Delay_ms(500);
	Delay_ms(500);
	
	GPIO_SetBits(JQ8900_GPIO_PORT, JQ8900_GPIO_PIN1);
	
}


//播放第二首
void JQ8900_PlaySecondVoice(void)
{
	
	GPIO_ResetBits(JQ8900_GPIO_PORT, JQ8900_GPIO_PIN2);
	//1s语音
	Delay_ms(500);
	Delay_ms(500);
	
	
	GPIO_SetBits(JQ8900_GPIO_PORT, JQ8900_GPIO_PIN2);
}

//播放第三首
void JQ8900_PlayThirdlyVoice(void)
{
	GPIO_ResetBits(JQ8900_GPIO_PORT, JQ8900_GPIO_PIN3);
	
	//2s 语音
	Delay_ms(500);
	Delay_ms(500);
	Delay_ms(500);
	Delay_ms(500);

GPIO_SetBits(JQ8900_GPIO_PORT, JQ8900_GPIO_PIN3);

	
}

