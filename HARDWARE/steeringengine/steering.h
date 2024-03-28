/***************SG90 180�ȶ�� 5V����*********************/
/*��ɫΪDATA��*/
/*��ɫΪVCC��*/
/*��ɫΪGND��*/
#ifndef _STEERING_H
#define _STEERING_H

#include "stm32f4xx.h"

#define STEERING_TYPE 1		//0Ϊ360�ȶ��; 1Ϊ180�ȶ��

//DATA��	
#define STEERING_ENGINE_DATA_PORT 	GPIOB			//���DATA���Ŷ˿�
#define STEERING_ENGINE_DATA_PIN   	GPIO_Pin_14  	//���DATA���ű��
#define STEERING_ENGINE_PinAF		GPIO_PinSource14	//ѡ���õ�����
#define STEERING_ENGINE_PinAF_TIM	GPIO_AF_TIM12	//���Ÿ��õĶ�ʱ��
#define STEERING_ENGINE_TIM			TIM12			//���õĶ�ʱ��

#define		ANGLE_MAX			(10U)	//��������Ƕ�
#define		ANGLE_MIN			(5U)	//�������С�Ƕ�
#define		ANGLE_STEP_SIZE		(1U)	//����Ĳ���


extern volatile int angle;	//�洢����Ƕ�

void STEERING_ENGINE_Config(void);
void STEERING_ENGINE_Foreward(u32 flag);

#endif
