#include "steering.h"
#include "delay.h"
#include "through-beam.h"
//�����ʼ��
void STEERING_ENGINE_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	//0.��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	//1.��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
	
	//2.��GPIO�˿�ʱ��  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	//3.����GPIO���Ų���ʼ��
	GPIO_InitStructure.GPIO_Pin 	= STEERING_ENGINE_DATA_PIN;				//���ű��
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;			//����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//�������
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL ;		//��������
	GPIO_Init(STEERING_ENGINE_DATA_PORT, &GPIO_InitStructure); 

	//4.��Ҫѡ������Ҫ���õĹ���
	GPIO_PinAFConfig(STEERING_ENGINE_DATA_PORT, STEERING_ENGINE_PinAF, STEERING_ENGINE_PinAF_TIM); 
	
	//5.���ö�ʱ���Ļ������� + ��ʼ��   ע�⣺�����źŵ����ھ����ܶ�һ��  ����10ms
	TIM_TimeBaseStructure.TIM_Prescaler = 16800-1;  // TIM10 84MHZ / 16800 = 5000HZ  200us����1��
	TIM_TimeBaseStructure.TIM_Period 	= 5000-1;	 // 1s
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;   //����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM14ֻ�ܵ�������
	TIM_TimeBaseInit(STEERING_ENGINE_TIM, &TIM_TimeBaseStructure);

	//6.���ö�ʱ����ͨ�� + ��ʼ����ʱ��ͨ��
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;							//PWMģʽ1  CNT < CCR1 ͨ����Ч 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //ʹ������Ƚ�
	TIM_OCInitStructure.TIM_Pulse = 0;														//CCR�Ĵ����ĳ�ֵ  Ĭ��ռ�ձ�0%
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;			//�ߵ�ƽ��Ч
	TIM_OC1Init(STEERING_ENGINE_TIM, &TIM_OCInitStructure);
	
	//7.ʹ�ܶ�ʱ��ͨ���Ե�Ԥװ�ؼĴ���
	TIM_OC1PreloadConfig(STEERING_ENGINE_TIM, TIM_OCPreload_Enable);
	
	//8.ʹ���Զ�����Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(STEERING_ENGINE_TIM, ENABLE);

	//9.�򿪶�ʱ��
	TIM_Cmd(STEERING_ENGINE_TIM, ENABLE);
	
	
}


/****************************���ת��************************************/
/* ������	@flag : �����ж϶����ת���Ƿ�ת							*/
/*					1	Ϊ��ת											*/
/*					0	Ϊ��ת											*/
/* ����ֵ��	��															*/
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





