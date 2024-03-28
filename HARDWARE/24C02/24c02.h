#ifndef __24C02_H
#define __24C02_H
#include "common.h"  
#include "delay.h"
////////////////////////////////////////////////////////////////////////////////// 	

//IIC_SDA��IO��������
#define SDA_IN()  {GPIOB->MODER&=~(3<<18);GPIOB->MODER|=0<<18;}	//PB9����ģʽ
#define SDA_OUT() {GPIOB->MODER&=~(3<<18);GPIOB->MODER|=1<<18;} //PB9���ģʽ
//IO��������	 
#define IIC_SCL      PBout(8) //SCL
#define IIC_SDAOUT   PBout(9) //���SDA	 
#define IIC_SDAIN    PBin(9)  //����SDA 

//IIC��غ���
void IIC_Init(void);          //��ʼ��IIC��IO��				 
void IIC_Start(void);				  //����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
u8 MCU_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void MCU_Send_Ack(void);					  //IIC����ACK�ź�
void MCU_NOAck(void);				  //IIC������ACK�ź�
void IIC_write_OneByte(u8 Senddata);
u8 IIC_Read_OneByte(u8 ack);
	
//EEPROM24c02��غ���
u8 AT24C02_ReadByte(u8 ReadAddr);							     //ָ����ַ��ȡһ���ֽ�
void AT24C02_WriteByte(u8 WriteAddr,u8 DataToWrite);		//ָ����ַд��һ���ֽ�

u32 Buf_4Byte(u8 *pBuffer,u32 Date_4Byte,u8 Byte_num,u8 mode);

void AT24C02_Write(u8 WriteAddr,u8 *pBuffer,u8 WriteNum);	//��ָ����ַ��ʼд��ָ�����ȵ�����
void AT24C02_Read(u8 ReadAddr,u8 *pBuffer,u8 ReadNum);   	//��ָ����ַ��ʼ����ָ�����ȵ�����

u8 AT24C02_Test(void);  //�������
void AT24C02_Init(void); //��ʼ��IIC
#endif
















