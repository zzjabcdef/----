/***************SG90 180度舵机 5V供电*********************/
/*黄色为DATA线*/
/*红色为VCC线*/
/*棕色为GND线*/
#ifndef _STEERING_H
#define _STEERING_H

#include "stm32f4xx.h"

#define STEERING_TYPE 1		//0为360度舵机; 1为180度舵机

//DATA线	
#define STEERING_ENGINE_DATA_PORT 	GPIOB			//舵机DATA引脚端口
#define STEERING_ENGINE_DATA_PIN   	GPIO_Pin_14  	//舵机DATA引脚编号
#define STEERING_ENGINE_PinAF		GPIO_PinSource14	//选择复用的引脚
#define STEERING_ENGINE_PinAF_TIM	GPIO_AF_TIM12	//引脚复用的定时器
#define STEERING_ENGINE_TIM			TIM12			//所用的定时器

#define		ANGLE_MAX			(10U)	//舵机的最大角度
#define		ANGLE_MIN			(5U)	//舵机的最小角度
#define		ANGLE_STEP_SIZE		(1U)	//舵机的步长


extern volatile int angle;	//存储舵机角度

void STEERING_ENGINE_Config(void);
void STEERING_ENGINE_Foreward(u32 flag);

#endif
