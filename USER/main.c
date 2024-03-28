/********************************************************************************************
**************************STM32F407核心应用板************************************************
*********************************************************************************************
* 文件名称: 智能输液系统main.c                                                              *
* 文件简述：																				*
*			@usart1.c : 测试串口源文件   				                                	*
*			@lcd.c : 桔恩迪电子3.2寸tft液晶电阻屏源代码										*
*			@keyboard.c : 键盘及其外部中断源文件											*
*			@delay.c : 延时函数源文件														*
*			@steering.c : 舵机源文件														*
*			@wifi.c : DSP8266-01SWIFI模块源文件												*
*			@mqtt.c : MQTT协议及阿里云物联网平台的数据传输源文件，需搭配出cJSON.c使用		*
*			@bluetooth.c : 蓝牙源文件														*
*			@through-beam.c : 对射光电传感器源文件											*
*			@DS18B20.c ：DS18B20温度传感器源文件											*
*			@printf.c : 将printf重定向到USART1串口输出，用于调试							*
*			@jq8900.c : JQ8900-16P语音播报模块源文件										*
*			@Raindrop_detection.c : 雨滴模块源文件											*
*			@heater_plate.c : 加热片源文件													*
*																							*
*********************************************************************************************
*********************************************************************************************/
/***********************************头文件***************************************************/
#include "stm32f4xx.h"  //必须添加
#include "keyboard.h"
#include "printf.h"
#include "usart1.h"
#include "delay.h"
#include "steering.h"
#include "bluetooth.h"
#include "through-beam.h"
#include "DS18B20.h"
#include "lcd.h"
#include "Raindrop_detection.h"
#include "heater_plate.h"
#include "jq8900.h"
/***********************************头文件***************************************************/

volatile int temp_now=0;

//程序入口
int main()
{
/*********************************硬件初始化*************************************************/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init();
	uart1_init(115200);
	JQ8900_Config();
	HEATER_PLATE_Config();//加热片初始化
	BLUETOOTH_Config(9600);//蓝牙初始化
	LCD_Init();					//LCD初始化 
	DS18B20_Init();//温度初始化
	DS18B20_ReadRomInfo();
	if(DS18B20_CheckDevice()==0)
		printf("dev OK\r\n");
	else
		printf("dev fail\r\n");
	THROUGH_BEAM_Config();//对射初始化
	STEERING_ENGINE_Config();//舵机初始化
	GPIO_KEY_Init();//按键初始化
	RAINDROP_DETECTION_Config();//雨滴初始化
/*********************************硬件初始化*************************************************/
	//2.发送测试指令
//	USART_SendString("AT\r\n");	
	
	
	printf("Init OK\r\n");

	char temp_str[12]=" ";
	char speed_str[12]=" ";
	char str[32];
	sprintf(str,"TEMP_SET : %d",Temp);
	POINT_COLOR=RED;      //画笔颜色：红色
	LCD_ShowString(30,40,210,24,24,(u8 *)str); 
	POINT_COLOR=BLUE;
	LCD_ShowString(30,70,200,16,16,(u8 *)"TEMP_NOW : ");
	LCD_ShowString(30,90,200,16,16,(u8 *)"SPEED : ");	
	char str1[32];
	LCD_ShowString(30,130,200,16,16,(u8 *)"angle : ");
	while(1)
	{	
		RAINDROP_DETECTION_ReadInputBits();
		Delay_ms(500);
//		Delay_ms(500);
		//测温
		temp_now = DS18B20_GetTemperature();
		if(Temp > temp_now)
		{
			//打开加热片
			HEATER_PLATE_Heating();
		}
		else
		{
			//关闭加热片
			HEATER_PLATE_Stop();
		}
			
		//界面显示
		LCD_Clear(WHITE);
		sprintf(str,"TEMP_SET : %d",Temp);
		POINT_COLOR=RED;      //画笔颜色：红色
		LCD_ShowString(30,40,210,24,24,(u8 *)str); 
		
		sprintf(temp_str,"TEMP_NOW : %d ",temp_now);
		POINT_COLOR=BLUE;
		LCD_ShowString(30,70,200,16,16,(u8 *)temp_str);
		
		sprintf(speed_str,"SPEED : %d ",through_num);
		POINT_COLOR=BLUE;
		LCD_ShowString(30,90,200,16,16,(u8 *)speed_str);
		
		sprintf(str1,"angle : %d",angle-4);
		POINT_COLOR=BLUE;
		LCD_ShowString(30,130,200,16,16,(u8 *)str1);
		
		//判断滴液速度
		if(through_num >= THROUGH_SPEED_MAX)
		{
			LCD_ShowString(30,110,200,16,16,(u8 *)"so fast\r\n");
		}
		else if(through_num <= THROUGH_SPEED_MIN)
		{
			LCD_ShowString(30,110,200,16,16,(u8 *)"so slow\r\n");
		}
		through_num=0;
		 
	}
	
	
}

