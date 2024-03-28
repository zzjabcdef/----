#include "steering.h"
#include "delay.h"
#include "through-beam.h"
//舵机初始化
void STEERING_ENGINE_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	//0.打开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	//1.打开时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
	
	//2.打开GPIO端口时钟  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	//3.配置GPIO引脚并初始化
	GPIO_InitStructure.GPIO_Pin 	= STEERING_ENGINE_DATA_PIN;				//引脚编号
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;			//复用模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//输出速率
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL ;		//上拉电阻
	GPIO_Init(STEERING_ENGINE_DATA_PORT, &GPIO_InitStructure); 

	//4.需要选择引脚要复用的功能
	GPIO_PinAFConfig(STEERING_ENGINE_DATA_PORT, STEERING_ENGINE_PinAF, STEERING_ENGINE_PinAF_TIM); 
	
	//5.配置定时器的基本参数 + 初始化   注意：脉冲信号的周期尽可能短一点  比如10ms
	TIM_TimeBaseStructure.TIM_Prescaler = 16800-1;  // TIM10 84MHZ / 16800 = 5000HZ  200us计数1次
	TIM_TimeBaseStructure.TIM_Period 	= 5000-1;	 // 1s
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;   //不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM14只能递增计数
	TIM_TimeBaseInit(STEERING_ENGINE_TIM, &TIM_TimeBaseStructure);

	//6.配置定时器的通道 + 初始化定时器通道
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;							//PWM模式1  CNT < CCR1 通道有效 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //使能输出比较
	TIM_OCInitStructure.TIM_Pulse = 0;														//CCR寄存器的初值  默认占空比0%
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;			//高电平有效
	TIM_OC1Init(STEERING_ENGINE_TIM, &TIM_OCInitStructure);
	
	//7.使能定时器通道对的预装载寄存器
	TIM_OC1PreloadConfig(STEERING_ENGINE_TIM, TIM_OCPreload_Enable);
	
	//8.使能自动重载预装载寄存器
	TIM_ARRPreloadConfig(STEERING_ENGINE_TIM, ENABLE);

	//9.打开定时器
	TIM_Cmd(STEERING_ENGINE_TIM, ENABLE);
	
	
}


/****************************舵机转动************************************/
/* 参数：	@flag : 用于判断舵机正转还是反转							*/
/*					1	为正转											*/
/*					0	为反转											*/
/* 返回值：	无															*/
/************************************************************************/
void STEERING_ENGINE_Foreward(u32 flag)
{
	if(flag)
	{

		if(angle <= ANGLE_MAX)
		{
			angle += ANGLE_STEP_SIZE;
			
		}
	
			TIM_SetCompare1(STEERING_ENGINE_TIM,angle);


	}
	else
	{

		if(angle > ANGLE_MIN)
		{
			angle -= ANGLE_STEP_SIZE;

		}
	
		TIM_SetCompare1(STEERING_ENGINE_TIM,angle);

	}
//	printf("angle:%d\r\n",angle);
}





