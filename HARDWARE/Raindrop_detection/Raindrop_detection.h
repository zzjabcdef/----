/*雨滴模块*/
//5V供电
//有水时DO输出低电平，无水时DO输出高电平

#ifndef _RAINDROP_DETECTION_H
#define _RAINDROP_DETECTION_H

#include "stm32f4xx.h"

#define RAINDROP_DETECTION_PORT 		GPIOC
#define RAINDROP_DETECTION_PIN  		GPIO_Pin_6
#define RAINDROP_DETECTION_CLK			RCC_AHB1Periph_GPIOC
#define RAINDROP_DETECTION_EXTI			EXTI_PortSourceGPIOC
#define RAINDROP_DETECTION_EXTI_SOURCE	EXTI_PinSource6
#define RAINDROP_DETECTION_EXTI_LINE	EXTI_Line6
#define RAINDROP_DETECTION_EXTI_IRQ		EXTI9_5_IRQn


extern volatile int temp_now;

void RAINDROP_DETECTION_Config(void);
void RAINDROP_DETECTION_ReadInputBits(void);

#endif
