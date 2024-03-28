/*********************蓝牙5V供电*************************/

#ifndef _BLUETOOTH_H
#define _BLUETOOTH_H

#include "stm32f4xx.h"
#include "steering.h"

#define USART2_BLUETOOTH 1


/***************串口2引脚PA2 PA3*********************/
#if USART2_BLUETOOTH
#define USART				USART2
#define GPIO_CLK 			RCC_AHB1Periph_GPIOA
#define GPIO_USART2_CLK		RCC_APB1Periph_USART2
#define GPIO_AF_TX_PIN		GPIO_PinSource2
#define GPIO_AF_RX_PIN		GPIO_PinSource3
#define GPIO_PIN			GPIO_Pin_2|GPIO_Pin_3;
#define GPIO_PORT			GPIOA
#define NVIC_IRQ			USART2_IRQn
#define NVIC_PPRIORITY		(0U)
#define NVIC_SPRIORITY		(0U)
#define GPIO_AF_USART		GPIO_AF_USART2
#endif

/***************串口3引脚PB10 PB11*********************/
#if USART3_BLUETOOTH
#define USART				USART3
#define GPIO_CLK 			RCC_AHB1Periph_GPIOB
#define GPIO_USART2_CLK		RCC_APB1Periph_USART3
#define GPIO_AF_TX_PIN		GPIO_PinSource10
#define GPIO_AF_RX_PIN		GPIO_PinSource11
#define GPIO_PIN			GPIO_Pin_10|GPIO_Pin_11;
#define GPIO_PORT			GPIOB
#define NVIC_IRQ			USART3_IRQn
#define NVIC_PPRIORITY		(0U)
#define NVIC_SPRIORITY		(0U)
#define GPIO_AF_USART		GPIO_AF_USART3
#endif






void BLUETOOTH_Config(u32 baud);
void USART_SendString(char * str);

#endif
