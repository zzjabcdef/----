#include "DS18B20.h"

int intT,decT1,decT2;
 u8 ROM_ID[8];
static void DS18B20_OutPut_Mode(){
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin   = DS18B20_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType =  GPIO_OType_OD; // 必须开漏
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
   
 
/*功能：DS18B20初始化*/
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
 
    /*2.检测DS18B20设备是否正常*/
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
 
/*功能：向DS18B20发送一个复位信号*/
void DS18B20_SendRestSingle(void)
{
    /*主机发送复位信号*/
    DS18B20_OutPut_Mode();
    DS18B20_OUT_CLR; //拉低总线480~960 us ，对 DS18B20 芯片进行复位
    Delay_us(750);
//	Delay_us(480);
    DS18B20_OUT_SET;
//    Delay_us(15);         //释放总线15~60 us
    Delay_us(15);         //释放总线15~60 us
}
 
/*
功能：检测DS18B20存在脉冲
返回值：
            0  DS18B20设备正常
            1  DS18B20设备响应复位信号失败
            2  DS18B20设备释放总线失败
*/
u8 DS18B20_CheckReadySingle(void)
{
    u8 cnt=0;
    /*1.检测存在脉冲*/
    DS18B20_InPut_Mode();
    while((DS18B20_IN() == 0X01) && (cnt < 240)) //等待DS18B20 拉低总线 （60~240 us 响应复位信号）
    {
        Delay_us(1);
        cnt++;
    }
    if(cnt>240) return 1;
    /*2.检测DS18B20是否释放总线*/    
    cnt=0;
    DS18B20_InPut_Mode();
    while((!DS18B20_IN()) && (cnt<240))  //判断DS18B20是否释放总线（60~240 us 响应复位信号之后会释放总线）
    {
        Delay_us(1);
        cnt++;
    }
    if(cnt>240)    return 2;
    else return 0;
}
 
/*
功能：检测DS18B20设备是否正常
返回值：
            0  DS18B20设备正常
            1  DS18B20设备响应复位信号失败
            2  DS18B20设备释放总线失败
*/
u8 DS18B20_CheckDevice(void)
{
    DS18B20_SendRestSingle();/*1.主机发送复位信号*/
    return DS18B20_CheckReadySingle();/*2.检测存在脉冲*/
}
 
/*功能：向DS18B20写一个字节数据（命令）*/
u8 BS18B20_WriteByte(u8 cmd)
{
    u8 i=0;
    u8 data = cmd;
    
    DS18B20_OutPut_Mode();
    for(i=0;i<8;i++)
    {
        DS18B20_OUT_CLR;
        Delay_us(2);     //主机拉低总线写数据时间隙2us
        if(data&0x01){    //如果该写入位为1，必须在15us之内把总线拉高，为0 保持60us即可。
            DS18B20_OUT_SET
        }else{
            DS18B20_OUT_CLR
        }
        Delay_us(60);
        DS18B20_OUT_SET;  //一位发送完成
        data >>=1;
        Delay_us(2);     //位间隙2us
    }
    return 0;
}
 
/*功能：从DS18B20读取一个字节数据*/
u8 DS18B20_ReadByte(void)
{
    u8 i,data=0;
    for(i=0;i<8;i++)
    {
        DS18B20_OutPut_Mode();//初始化为输出模式
        DS18B20_OUT_CLR;  
        Delay_us(2);  //主机拉低总线读数据时间隙2us        
        DS18B20_OUT_SET; //释放总线，准备读取位数据
        DS18B20_InPut_Mode(); //初始化为输入模式
        Delay_us(10); //等待DS18B20的数据输出
        data >>=1 ;  //高位补0，默认以0为准
        
        if(DS18B20_IN()){
            data |=0x80; //eB这里注意下,看看对不对
        }
        
        
        Delay_us(60); //延时确保DS18B20采样周期已经过去（非常重要）
//        DS18B20_OUT_SET;  //释放总线准备读取下一位位数据
    }
    return data;
}
 
/*
函数功能: 读取一次DS18B20的温度数据
返 回 值: 读取的温度数据
考虑的情况:  总线上只是接了一个DS18B20的情况
*/
u16 DS18B20_GetTemperature(void)
{
        u16 temp=0;
        u8 temp_H,temp_L;
        
        DS18B20_CheckDevice();   //发送复位脉冲、检测存在脉冲
	
        BS18B20_WriteByte(0xCC); //跳过ROM序列检测

        BS18B20_WriteByte(0x44); //启动一次温度转换
        
	
        //等待温度转换完成
        while(DS18B20_ReadByte()!=0xFF){}
        DS18B20_CheckDevice();   //发送复位脉冲、检测存在脉冲
        BS18B20_WriteByte(0xCC); //跳过ROM序列检测
        BS18B20_WriteByte(0xBE); //读取温度
        temp_L=DS18B20_ReadByte(); //读取的温度低位数据
        temp_H=DS18B20_ReadByte(); //读取的温度高位数据
        if(temp_H>7){
         temp_H =~temp_H;
         temp_L =~temp_L;
        };           
		
        temp=temp_L+(temp_H<<8);   //合成温度
        intT = temp>>4 ;          /*合成实际温度整数部分****精度相对上面的更高*/
        decT1 = (temp_L&0x0F)*10/16 ;         /*合成实际温度小数部分*/
        decT2 = (temp_L&0x0F)*100/16%10 ;
		
		temp *= 0.0625;
		
        return temp;
}
 
 
/*功能：从DS18B20读取ROM信息 (ROM_ID= 28-92-AF-AC-17-13-1-F1)*/
u8 DS18B20_ReadRomInfo(void)
{
    u8 i=0;
    BS18B20_WriteByte(0x33);  /*4.启动读取ROM*/
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
 
/*功能：匹配 DS18B20  ROM信息*/
u8 DS18B20_MatchROM(void)
{
    u8 i=0;
    BS18B20_WriteByte(0x55);  /*4.匹配64位 ROM 信息*/
    for(i=0;i<8;i++)
    {
        BS18B20_WriteByte(ROM_ID[i]);
    }
    return 0;
}
