#include "DS18B20.h"

int intT,decT1,decT2;
 u8 ROM_ID[8];
static void DS18B20_OutPut_Mode(){
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin   = DS18B20_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType =  GPIO_OType_OD; // ���뿪©
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);    
}
static void DS18B20_InPut_Mode(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin   = DS18B20_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);      
}
static uint8_t DS18B20_IN(void){
    return GPIO_ReadInputDataBit(DS18B20_PORT, DS18B20_PIN);
}
 

static void OLED_ShowString(int a,int b, char * str)
{
	printf("%s\r\n",str);
 
 
}
   
 
/*���ܣ�DS18B20��ʼ��*/
int DS18B20_Init(void)
{
    
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_AHB1PeriphClockCmd(DS18B20_PORT_RCC, ENABLE );
    GPIO_InitStructure.GPIO_Pin   = DS18B20_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType =  GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
 
    /*2.���DS18B20�豸�Ƿ�����*/
    switch(DS18B20_CheckDevice())
    {
        case 0:    {
            OLED_ShowString(1,7," OK!");
        }return 0;
        case 1:    {
            OLED_ShowString(1,7,"error1");
        }return 1;
        case 2:    {
           OLED_ShowString(1,7,"error2");
        }return 2;
    }
    return -1;
}
 
/*���ܣ���DS18B20����һ����λ�ź�*/
void DS18B20_SendRestSingle(void)
{
    /*�������͸�λ�ź�*/
    DS18B20_OutPut_Mode();
    DS18B20_OUT_CLR; //��������480~960 us ���� DS18B20 оƬ���и�λ
    Delay_us(750);
//	Delay_us(480);
    DS18B20_OUT_SET;
//    Delay_us(15);         //�ͷ�����15~60 us
    Delay_us(15);         //�ͷ�����15~60 us
}
 
/*
���ܣ����DS18B20��������
����ֵ��
            0  DS18B20�豸����
            1  DS18B20�豸��Ӧ��λ�ź�ʧ��
            2  DS18B20�豸�ͷ�����ʧ��
*/
u8 DS18B20_CheckReadySingle(void)
{
    u8 cnt=0;
    /*1.����������*/
    DS18B20_InPut_Mode();
    while((DS18B20_IN() == 0X01) && (cnt < 240)) //�ȴ�DS18B20 �������� ��60~240 us ��Ӧ��λ�źţ�
    {
        Delay_us(1);
        cnt++;
    }
    if(cnt>240) return 1;
    /*2.���DS18B20�Ƿ��ͷ�����*/    
    cnt=0;
    DS18B20_InPut_Mode();
    while((!DS18B20_IN()) && (cnt<240))  //�ж�DS18B20�Ƿ��ͷ����ߣ�60~240 us ��Ӧ��λ�ź�֮����ͷ����ߣ�
    {
        Delay_us(1);
        cnt++;
    }
    if(cnt>240)    return 2;
    else return 0;
}
 
/*
���ܣ����DS18B20�豸�Ƿ�����
����ֵ��
            0  DS18B20�豸����
            1  DS18B20�豸��Ӧ��λ�ź�ʧ��
            2  DS18B20�豸�ͷ�����ʧ��
*/
u8 DS18B20_CheckDevice(void)
{
    DS18B20_SendRestSingle();/*1.�������͸�λ�ź�*/
    return DS18B20_CheckReadySingle();/*2.����������*/
}
 
/*���ܣ���DS18B20дһ���ֽ����ݣ����*/
u8 BS18B20_WriteByte(u8 cmd)
{
    u8 i=0;
    u8 data = cmd;
    
    DS18B20_OutPut_Mode();
    for(i=0;i<8;i++)
    {
        DS18B20_OUT_CLR;
        Delay_us(2);     //������������д����ʱ��϶2us
        if(data&0x01){    //�����д��λΪ1��������15us֮�ڰ��������ߣ�Ϊ0 ����60us���ɡ�
            DS18B20_OUT_SET
        }else{
            DS18B20_OUT_CLR
        }
        Delay_us(60);
        DS18B20_OUT_SET;  //һλ�������
        data >>=1;
        Delay_us(2);     //λ��϶2us
    }
    return 0;
}
 
/*���ܣ���DS18B20��ȡһ���ֽ�����*/
u8 DS18B20_ReadByte(void)
{
    u8 i,data=0;
    for(i=0;i<8;i++)
    {
        DS18B20_OutPut_Mode();//��ʼ��Ϊ���ģʽ
        DS18B20_OUT_CLR;  
        Delay_us(2);  //�����������߶�����ʱ��϶2us        
        DS18B20_OUT_SET; //�ͷ����ߣ�׼����ȡλ����
        DS18B20_InPut_Mode(); //��ʼ��Ϊ����ģʽ
        Delay_us(10); //�ȴ�DS18B20���������
        data >>=1 ;  //��λ��0��Ĭ����0Ϊ׼
        
        if(DS18B20_IN()){
            data |=0x80; //eB����ע����,�����Բ���
        }
        
        
        Delay_us(60); //��ʱȷ��DS18B20���������Ѿ���ȥ���ǳ���Ҫ��
//        DS18B20_OUT_SET;  //�ͷ�����׼����ȡ��һλλ����
    }
    return data;
}
 
/*
��������: ��ȡһ��DS18B20���¶�����
�� �� ֵ: ��ȡ���¶�����
���ǵ����:  ������ֻ�ǽ���һ��DS18B20�����
*/
u16 DS18B20_GetTemperature(void)
{
        u16 temp=0;
        u8 temp_H,temp_L;
        
        DS18B20_CheckDevice();   //���͸�λ���塢����������
	
        BS18B20_WriteByte(0xCC); //����ROM���м��

        BS18B20_WriteByte(0x44); //����һ���¶�ת��
        
	
        //�ȴ��¶�ת�����
        while(DS18B20_ReadByte()!=0xFF){}
        DS18B20_CheckDevice();   //���͸�λ���塢����������
        BS18B20_WriteByte(0xCC); //����ROM���м��
        BS18B20_WriteByte(0xBE); //��ȡ�¶�
        temp_L=DS18B20_ReadByte(); //��ȡ���¶ȵ�λ����
        temp_H=DS18B20_ReadByte(); //��ȡ���¶ȸ�λ����
        if(temp_H>7){
         temp_H =~temp_H;
         temp_L =~temp_L;
        };           
		
        temp=temp_L+(temp_H<<8);   //�ϳ��¶�
        intT = temp>>4 ;          /*�ϳ�ʵ���¶���������****�����������ĸ���*/
        decT1 = (temp_L&0x0F)*10/16 ;         /*�ϳ�ʵ���¶�С������*/
        decT2 = (temp_L&0x0F)*100/16%10 ;
		
		temp *= 0.0625;
		
        return temp;
}
 
 
/*���ܣ���DS18B20��ȡROM��Ϣ (ROM_ID= 28-92-AF-AC-17-13-1-F1)*/
u8 DS18B20_ReadRomInfo(void)
{
    u8 i=0;
    BS18B20_WriteByte(0x33);  /*4.������ȡROM*/
    for(i=0;i<8;i++)
    {
        ROM_ID[i]=DS18B20_ReadByte();
    }
    printf("ROM_ID= ");
    for(i=0;i<8;i++)
    {
        printf("%X",ROM_ID[i]);
        if(i==7) {
            printf("\r\n");
        }
        else{
            printf("-");
        }            
    }
    return 0;
}
 
/*���ܣ�ƥ�� DS18B20  ROM��Ϣ*/
u8 DS18B20_MatchROM(void)
{
    u8 i=0;
    BS18B20_WriteByte(0x55);  /*4.ƥ��64λ ROM ��Ϣ*/
    for(i=0;i<8;i++)
    {
        BS18B20_WriteByte(ROM_ID[i]);
    }
    return 0;
}
