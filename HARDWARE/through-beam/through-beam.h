/*ITR9606对射光电传感模块*/
//5V供电
//有遮挡，输出高电平;无遮挡，输出低电平。
//窄通道无需AO线
#ifndef _THROUGH_BEAM_H
#define _THROUGH_BEAM_H

#include "stm32f4xx.h"
#include "printf.h"
#include "steering.h"


#define THROUGH_PORT 				GPIOE			//DO引脚端口
#define THROUGH_PIN   				GPIO_Pin_0  	//DO引脚编号
#define THROUGH_CLK					RCC_AHB1Periph_GPIOE
#define THROUGH_EXTI_GPIO			EXTI_PortSourceGPIOE
#define THROUGH_EXTI_PIN			EXTI_PinSource0
#define THROUGH_EXTI_LINE			EXTI_Line0
#define THROUGH_EXTI_IRQ			EXTI0_IRQn

#define THROUGH_SPEED_MIN				3 //滴液速度阈值
#define THROUGH_SPEED_MAX				7


extern volatile int through_num;

void THROUGH_BEAM_Config(void);

#endif
