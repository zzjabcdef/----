#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "stm32f4xx.h"
#include "printf.h"
#include "delay.h"
#include "steering.h"

//按键的GPIO端口及时钟
#define		KEYBOARD_GPIO_PORT	GPIOC	
#define 	KEYBOARD_GPIO_PIN   GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_2|GPIO_Pin_4
#define		KEYBOARD_GPIO_CLK	RCC_AHB1Periph_GPIOC

extern volatile int angle;
extern volatile int Temp;

void GPIO_KEY_Init(void);

#endif
