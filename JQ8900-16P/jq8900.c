#include "jq8900.h"

//JQ8900-16P��ʼ��
void JQ8900_Config(void)
{
	//����ṹ��
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//��ʱ��
	RCC_AHB1PeriphClockCmd(JQ8900_GPIO_CLK, ENABLE);
	
	//���úͳ�ʼ������
	GPIO_InitStruct.GPIO_Pin =  JQ8900_GPIO_PIN1|JQ8900_GPIO_PIN2|JQ8900_GPIO_PIN3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; //����ʹ����������GPIO_PuPd_UP ʹ��ƽĬ��Ϊ�ߵ�ƽ1 
//	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(JQ8900_GPIO_PORT, &GPIO_InitStruct);

}


//���ŵ�һ��
void JQ8900_PlayFirstVoice(void)
{
	GPIO_ResetBits(JQ8900_GPIO_PORT, JQ8900_GPIO_PIN1);
	
	
	//1s����
	Delay_ms(500);
	Delay_ms(500);
	
	GPIO_SetBits(JQ8900_GPIO_PORT, JQ8900_GPIO_PIN1);
	
}


//���ŵڶ���
void JQ8900_PlaySecondVoice(void)
{
	
	GPIO_ResetBits(JQ8900_GPIO_PORT, JQ8900_GPIO_PIN2);
	//1s����
	Delay_ms(500);
	Delay_ms(500);
	
	
	GPIO_SetBits(JQ8900_GPIO_PORT, JQ8900_GPIO_PIN2);
}

//���ŵ�����
void JQ8900_PlayThirdlyVoice(void)
{
	GPIO_ResetBits(JQ8900_GPIO_PORT, JQ8900_GPIO_PIN3);
	
	//2s ����
	Delay_ms(500);
	Delay_ms(500);
	Delay_ms(500);
	Delay_ms(500);

GPIO_SetBits(JQ8900_GPIO_PORT, JQ8900_GPIO_PIN3);

	
}

