/*ITR9606�����紫��ģ��*/
//5V����
//���ڵ�������ߵ�ƽ;���ڵ�������͵�ƽ��
//խͨ������AO��
#ifndef _THROUGH_BEAM_H
#define _THROUGH_BEAM_H

#include "stm32f4xx.h"
#include "printf.h"
#include "steering.h"


#define THROUGH_PORT 				GPIOE			//DO���Ŷ˿�
#define THROUGH_PIN   				GPIO_Pin_0  	//DO���ű��
#define THROUGH_CLK					RCC_AHB1Periph_GPIOE
#define THROUGH_EXTI_GPIO			EXTI_PortSourceGPIOE
#define THROUGH_EXTI_PIN			EXTI_PinSource0
#define THROUGH_EXTI_LINE			EXTI_Line0
#define THROUGH_EXTI_IRQ			EXTI0_IRQn

#define THROUGH_SPEED_MIN				3 //��Һ�ٶ���ֵ
#define THROUGH_SPEED_MAX				7


extern volatile int through_num;

void THROUGH_BEAM_Config(void);

#endif
