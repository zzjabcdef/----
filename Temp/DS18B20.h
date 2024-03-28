#ifndef __DS18B20_H
#define __DS18B20_H

#include "stm32f4xx.h"
#include "delay.h"
#include "printf.h"

#define DS18B20_PORT_RCC    RCC_AHB1Periph_GPIOB
#define DS18B20_PORT        GPIOB
#define DS18B20_PIN         GPIO_Pin_15
 
 
#define DS18B20_OUT_SET  GPIO_SetBits(DS18B20_PORT, DS18B20_PIN);
#define DS18B20_OUT_CLR  GPIO_ResetBits(DS18B20_PORT, DS18B20_PIN);
 
int DS18B20_Init(void);
u8 DS18B20_CheckDevice(void);
void DS18B20_SendRestSingle(void);
u8 DS18B20_CheckReadySingle(void);
u8 BS18B20_WriteByte(u8 cmd);
u8 BS18B20_ReadByte(void);
u16 DS18B20_GetTemperature(void);
u8 DS18B20_ReadRomInfo(void);
u8 DS18B20_MatchROM(void);
extern int intT,decT1,decT2; 

#endif
