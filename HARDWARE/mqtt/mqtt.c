#include "mqtt.h"

char			mqtt_post_msg[526];
uint32_t		mqtt_tx_len;
const uint8_t 	g_packet_heart_reply[2] = {0xc0,0x00};

extern char yzm[5];

volatile u32 num;


//void TIM2_IRQHandler(void)
//{
//	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
//	{
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//		if(num == 6)
//		{
//			num = 0;
//			mqtt_send_heart();
//		}
//		num++;
//		
//	}
//}


//��ʼ����ʱ�������ṩ����
void MQTT_TIM_Init(void)
{
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	/* TIM2 clock enable */
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
//	
//	/* Time base configuration */
//	TIM_TimeBaseStructure.TIM_Period = 60000-1;
//	TIM_TimeBaseStructure.TIM_Prescaler = 8400-1;
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
// 
//	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
//	
//	/* Enable the TIMx gloabal Interrupt */
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	
//	/* TIM2 Interrupts enable */
//	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
//	
//	/* TIM2 enable counter */
//	TIM_Cmd(TIM2, ENABLE);
}


//����MQTT���Ӱ�����
int Mqtt_Connect_Aliyun(void)
{
	
	int ret = 0;
	//���ӵ�Ŀ��TCP������
	ret =esp8266_connect_server("TCP",MQTT_BROKERADDRESS,1883);
	if(ret)
	{
		printf("esp8266_connect_server fail\r\n");
		return -5;
	}	
	printf("esp8266_connect_server success\r\n");
	Delay_ms(300);
	MQTT_TIM_Init();
	//�������״̬
	ret = esp8266_check_connection_status();
	if(ret)
	{
		printf("esp8266_check_connection_status fail\r\n");
		
		//���������ȵ�
		while(Esp8266_Init());
	}
	printf("esp8266_check_connection_status success\r\n");
	Delay_ms(500);
	Delay_ms(500);
	Delay_ms(500);
	Delay_ms(500);
	
	//����͸��ģʽ
	ret = esp8266_entry_transparent_transmission();
	if(ret)
	{
		printf("esp8266_entry_transparent_transmission fail\r\n");
		return -6;
	}	
	printf("esp8266_entry_transparent_transmission success\r\n");
	Delay_ms(500);
	Delay_ms(500);
	Delay_ms(500);
	Delay_ms(500);
	
	
	//MQTT�����ƶ�
	if(mqtt_connect(MQTT_CLIENTID, MQTT_USARNAME, MQTT_PASSWD))
	{
		printf("mqtt_connect fail\r\n");
		return -7;	
	
	}
	printf("mqtt_connect success\r\n");
	Delay_ms(500);
	Delay_ms(500);
	Delay_ms(500);
	Delay_ms(500);
	
	//MQTT��������
	if(mqtt_subscribe_topic(MQTT_SUBSCRIBE_TOPIC,0,1))
	{
		printf("mqtt_subscribe_topic fail\r\n");
		return -8;
	}	
	printf("mqtt_subscribe_topic success\r\n");

	return 0;
}

//MQTT���ӷ������Ĵ������
int32_t mqtt_connect(char *client_id,char *user_name,char *password)
{
	uint8_t encodedByte = 0;
    uint32_t client_id_len = strlen(client_id);
    uint32_t user_name_len = strlen(user_name);
    uint32_t password_len  = strlen(password);
    uint32_t data_len;
    uint32_t cnt =2;
    uint32_t wait=0;
    mqtt_tx_len  =0;
	
    //�ɱ䱨ͷ+Payload  ÿ���ֶΰ��������ֽڵĳ��ȱ�ʶ
    data_len = 10 + (client_id_len+2) + (user_name_len+2) + (password_len+2);

    //�̶���ͷ
    //���Ʊ�������
    Tx3Buffer[mqtt_tx_len++] = 0x10;			//MQTT Message Type CONNECT
    //ʣ�೤��(�������̶�ͷ��)
    do
    {
        encodedByte = data_len % 128;
        data_len = data_len / 128;
        //if there are more data to encode, set the top bit of this byte
        if( data_len > 0 )
            encodedByte = encodedByte | 128;
        Tx3Buffer[mqtt_tx_len++] = encodedByte;
    } while( data_len > 0 );

    //�ɱ䱨ͷ
    //Э����
    Tx3Buffer[mqtt_tx_len++] = 0;			// Protocol Name Length MSB
    Tx3Buffer[mqtt_tx_len++] = 4;			// Protocol Name Length LSB
    Tx3Buffer[mqtt_tx_len++] = 'M';			// ASCII Code for M
    Tx3Buffer[mqtt_tx_len++] = 'Q';			// ASCII Code for Q
    Tx3Buffer[mqtt_tx_len++] = 'T';			// ASCII Code for T
    Tx3Buffer[mqtt_tx_len++] = 'T';			// ASCII Code for T
    //Э�鼶��
    Tx3Buffer[mqtt_tx_len++] = 4;			// MQTT Protocol version = 4
    //���ӱ�־
    Tx3Buffer[mqtt_tx_len++] = 0xc2;		// conn flags
    Tx3Buffer[mqtt_tx_len++] = 0;			// Keep-alive Time Length MSB
    Tx3Buffer[mqtt_tx_len++] = 60;			// Keep-alive Time Length LSB  60S������

    Tx3Buffer[mqtt_tx_len++] = BYTE1(client_id_len);// Client ID length MSB
    Tx3Buffer[mqtt_tx_len++] = BYTE0(client_id_len);// Client ID length LSB
    memcpy(&Tx3Buffer[mqtt_tx_len],client_id,client_id_len);
    mqtt_tx_len += client_id_len;

    if(user_name_len > 0)
    {
        Tx3Buffer[mqtt_tx_len++] = BYTE1(user_name_len);		//user_name length MSB
        Tx3Buffer[mqtt_tx_len++] = BYTE0(user_name_len);		//user_name length LSB
        memcpy(&Tx3Buffer[mqtt_tx_len],user_name,user_name_len);
        mqtt_tx_len += user_name_len;
    }

    if(password_len > 0)
    {
        Tx3Buffer[mqtt_tx_len++] = BYTE1(password_len);			//password length MSB
        Tx3Buffer[mqtt_tx_len++] = BYTE0(password_len);			//password length LSB
        memcpy(&Tx3Buffer[mqtt_tx_len],password,password_len);
        mqtt_tx_len += password_len;
    }

    while(cnt--)
    {
        memset((void *)Rx3Buffer,0,sizeof(Rx3Buffer));
		Rx3Counter=0;
		
        mqtt_send_bytes(Tx3Buffer,mqtt_tx_len);
		
		//�ȴ�3sʱ��
        wait=3000;
		
        while(wait--)
        {
				Delay_ms(1);

			//�������ȷ�Ϲ̶���ͷ
            if((Rx3Buffer[0]==0x20) && (Rx3Buffer[1]==0x02))
            {
				if(Rx3Buffer[3] == 0x00)
				{
					printf("�����ѱ��������˽��ܣ�����ȷ�ϳɹ�\r\n");
					//���ӳɹ�
					return 0;
				}
				else
				{
					switch(Rx3Buffer[3])
					{
						case 1:printf("�����Ѿܾ�����֧�ֵ�Э��汾\r\n");
						break;
						case 2:printf("�����Ѿܾ������ϸ�Ŀͻ��˱�ʶ��\r\n");
						break;		
						case 3:printf("�����Ѿܾ�������˲�����\r\n");
						break;		
						case 4:printf("�����Ѿܾ�����Ч���û�������\r\n");
						break;	
						case 5:printf("�����Ѿܾ���δ��Ȩ\r\n");
						break;
						default:printf("δ֪��Ӧ\r\n");
						break;
					}
					return 0;
				} 
            }  
        }
    }
	
    return -1;
}

/**
  * @brief  MQTT����/ȡ���������ݴ������
  * @param  topic  		����
  * @param  qos    		��Ϣ�ȼ�
  * @param  whether: 	����/ȡ�����������
  * @retval 0���ɹ���
  * 		1��ʧ�ܣ�
  */
int32_t mqtt_subscribe_topic(char *topic,uint8_t qos,uint8_t whether)
{
	uint8_t encodedByte=0;
    uint32_t cnt=2;
    uint32_t wait=0;
	
    uint32_t topiclen = strlen(topic);
    uint32_t data_len = 2 + (topiclen+2) + (whether?1:0);//�ɱ䱨ͷ�ĳ��ȣ�2�ֽڣ�������Ч�غɵĳ���
	
	mqtt_tx_len=0;
	
    //�̶���ͷ
    //���Ʊ�������
    if(whether) 
		Tx3Buffer[mqtt_tx_len++] = 0x82; 	//��Ϣ���ͺͱ�־����
    else	
		Tx3Buffer[mqtt_tx_len++] = 0xA2; 	//ȡ������

    //ʣ�೤��
    do
    {
        encodedByte = data_len % 128;
        data_len 	= data_len / 128;
        //if there are more data to encode, set the top bit of this byte
        if ( data_len > 0 )
            encodedByte = encodedByte | 128;
        Tx3Buffer[mqtt_tx_len++] = encodedByte;
    } while ( data_len > 0 );

    //�ɱ䱨ͷ
    Tx3Buffer[mqtt_tx_len++] = 0;				//��Ϣ��ʶ�� MSB
    Tx3Buffer[mqtt_tx_len++] = 0x01;			//��Ϣ��ʶ�� LSB
	
    //��Ч�غ�
    Tx3Buffer[mqtt_tx_len++] = BYTE1(topiclen);	//���ⳤ�� MSB
    Tx3Buffer[mqtt_tx_len++] = BYTE0(topiclen);	//���ⳤ�� LSB
    memcpy(&Tx3Buffer[mqtt_tx_len],topic,topiclen);
	
    mqtt_tx_len += topiclen;

    if(whether)
    {
        Tx3Buffer[mqtt_tx_len++] = qos;			//QoS����
    }

    while(cnt--)
    {
		Rx3Counter=0;
        memset((void *)Rx3Buffer,0,sizeof(Rx3Buffer));
        mqtt_send_bytes(Tx3Buffer,mqtt_tx_len);
		
        wait=3000;								//�ȴ�3sʱ��
        while(wait--)
        {
			Delay_ms(1);
			
			//��鶩��ȷ�ϱ�ͷ
            if(Rx3Buffer[0]==0x90)
            {
				printf("��������ȷ�ϳɹ�\r\n");
				
				//��ȡʣ�೤��
				if(Rx3Buffer[1]==3)
				{
					printf("Success - Maximum QoS 0 is %02X\r\n",Rx3Buffer[2]);
					printf("Success - Maximum QoS 2 is %02X\r\n",Rx3Buffer[3]);		
					printf("Failure is %02X\r\n",Rx3Buffer[4]);	
				}
				//��ȡʣ�೤��
				if(Rx3Buffer[1]==2)
				{
					printf("Success - Maximum QoS 0 is %02X\r\n",Rx3Buffer[2]);
					printf("Success - Maximum QoS 2 is %02X\r\n",Rx3Buffer[3]);			
				}				
				
				//��ȡʣ�೤��
				if(Rx3Buffer[1]==1)
				{
					printf("Success - Maximum QoS 0 is %02X\r\n",Rx3Buffer[2]);		
				}
				
				//���ĳɹ�
                return 0;
            }
        }
    }
	
    if(cnt) 
		return 0;//���ĳɹ�
	
    return -1;
}

/**
  * @brief  MQTT����/ȡ���������ݴ������
  * @param  topic  		����
  * @param  message  	��Ϣ
  * @param  qos    		��Ϣ�ȼ�
  * @retval 0���ɹ���
  * 		1��ʧ�ܣ�
  */
uint32_t mqtt_publish_data(char *topic, char *message, uint8_t qos)
{
//static 
	uint16_t id=0;	
    uint32_t topicLength = strlen(topic);
    uint32_t messageLength = strlen(message);

    uint32_t data_len;
	uint8_t encodedByte;

    mqtt_tx_len=0;
    //��Ч�غɵĳ����������㣺�ù̶���ͷ�е�ʣ�೤���ֶε�ֵ��ȥ�ɱ䱨ͷ�ĳ���
    //QOSΪ0ʱû�б�ʶ��
    //���ݳ���             ������   ���ı�ʶ��   ��Ч�غ�
    if(qos)	data_len = (2+topicLength) + 2 + messageLength;
    else	data_len = (2+topicLength) + messageLength;

    //�̶���ͷ
    //���Ʊ�������
    Tx3Buffer[mqtt_tx_len++] = 0x30;				// MQTT Message Type PUBLISH

    //ʣ�೤��
    do
    {
        encodedByte = data_len % 128;
        data_len = data_len / 128;
        // if there are more data to encode, set the top bit of this byte
        if ( data_len > 0 )
            encodedByte = encodedByte | 128;
        Tx3Buffer[mqtt_tx_len++] = encodedByte;
    } while ( data_len > 0 );

    Tx3Buffer[mqtt_tx_len++] = BYTE1(topicLength);	//���ⳤ��MSB
    Tx3Buffer[mqtt_tx_len++] = BYTE0(topicLength);	//���ⳤ��LSB
	
    memcpy(&Tx3Buffer[mqtt_tx_len],topic,topicLength);	//��������
	
    mqtt_tx_len += topicLength;

    //���ı�ʶ��
    if(qos)
    {
        Tx3Buffer[mqtt_tx_len++] = BYTE1(id);
        Tx3Buffer[mqtt_tx_len++] = BYTE0(id);
        id++;
    }
	
    memcpy(&Tx3Buffer[mqtt_tx_len],message,messageLength);
	
    mqtt_tx_len += messageLength;
	

	mqtt_send_bytes(Tx3Buffer,mqtt_tx_len);
	
	//Qos�ȼ����õ���00����˰�����������ƽ̨��û�з�����Ӧ��Ϣ��;
	return mqtt_tx_len;
}
/*
{
	"method":"thing.service.property.set",
	"id":"554823095",
	"params":{"temperature":12},
	"version":"1.0.0"
}
*/


//�豸״̬�ϱ�
void mqtt_report_devices_status(int temp,int speed)
{
    uint8_t led1_state 	= GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_3) ? 0:1;
    uint8_t led2_state 	= GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_4) ? 0:1;
    uint8_t NO_state	= GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_5) ? 0:1;

    //�ѿ�������ص�״̬��������sprintf������ŵ�һ��������ٰѸ���������MQTTЭ��������Ϣ����
    //����ʵ��ƽ̨���ݶ�Ӧ���豸��Ϣ������������Ϣ��
	sprintf(mqtt_post_msg,"{\"method\":\"thing.service.property.set\",\"id\":\"594360699\",\"params\":{\"VehSpeed\":%d,\"temperature\":%d},\"version\":\"1.0.0\"}",speed,temp);

	//�ϱ���Ϣ��ƽ̨������
	mqtt_publish_data(MQTT_PUBLISH_TOPIC,mqtt_post_msg,0);
	printf("messge publish to aliyun server OK\r\n");
}

//MQTT��������
void mqtt_send_bytes(uint8_t *buf,uint32_t len)
{
    esp8266_send_bytes(buf,len);
}

//����������
int32_t mqtt_send_heart(void)
{	
	uint8_t buf[2]={0xC0,0x00};
    uint32_t cnt=2;
    uint32_t wait=0;	
	
#if 0	
	mqtt_send_bytes(buf,2);
	return 0;
#else
    while(cnt--)
    {	
		mqtt_send_bytes(buf,2);
		memset((void *)Rx3Buffer,0,sizeof(Rx3Buffer));
		Rx3Counter=0;	
		
		wait=3000;//�ȴ�3sʱ��
		
		while(wait--)
		{
				Delay_ms(1);

			//���������Ӧ�̶���ͷ
			if((Rx3Buffer[0]==0xD0) && (Rx3Buffer[1]==0x00)) 
			{
				printf("������Ӧȷ��ʧ�ܣ�����������\r\n");
				
				return 0;
			}
		}
	}
	printf("������Ӧȷ�ϳɹ������������ߡ�\r\n");
//	while(esp8266_connect_server("TCP",MQTT_BROKERADDRESS,1883));
	return -1;
#endif	

}

//MQTT�������Ͽ�
void mqtt_disconnect(void)
{
	uint8_t buf[2]={0xe0,0x00};
	
    mqtt_send_bytes(buf,2);
	
	esp8266_disconnect_server();
}


/*
����mqttЭ�鷢����Ϣ���ݰ� = 0x30+ʣ�೤��+01+00+Topic������+Json���ݣ�����ͨ��������������ƽ̨��������			
0x30 0xE2 0x01 0x00 /thing/service/property/set{"method":"thing.service.property.set","id":"1597870845","params":{"NO":1,"led1":1,"led2":1},"version":"1.0.0"}		
����cJSONʱ����ȫΪ�ַ�����������0x00����������0x00�ᵼ��ֱ�ӽ���cJSON�ġ������Ҫ���в���'{'��ͷ��Json����				
*/
//���������·�����
void mqtt_msg_handle(void)
{
	uint8_t i;
	
	//�ȴ����ݽ������
		Delay_ms(100);			
	
	//���˴��ɲ����������������յ��������·��������Ƿ�����С�"method"�������ǣ���������ַ����а���\0�Ļ���strstr������ҵ����ͻ᷵��null����
//	if( ! esp8266_find_str_in_rx_packet("method",5000));
	{
 		for(i=0;i<Rx3Counter;i++)
		{	
			//����'{'
			if(Rx3Buffer[i] == '{')
			{
				//�����ɹ������˳�
				if( ! mqtt_cjson_parse((char *)&Rx3Buffer[i]))
					break;
			}
		}
		//���ڽ�������λ����0,���������
		Rx3Counter = 0;			
		Rx3End=0;
	}
}

//����MQTT�·�����
/*{
    "method":"thing.service.property.set",
    "id":"1597870845",
    "params":{
        "NO":1,
        "led1":1,
        "led2":1
    },
    "version":"1.0.0"
}
{
	"code":200,"data":{},
	"id":"1710478496012",
	"message":"success",
	"method":"thing.event.property.post",
	"version":"1.0"
}
*/

#if 1
uint32_t mqtt_cjson_parse(char *pbuf)
{
	cJSON *Json=NULL, *Method=NULL, *Id=NULL, *Params=NULL, *Item=NULL;

	char *psrt = pbuf;

	//�������ݰ�
	Json = cJSON_Parse(psrt);
	if(Json == NULL)												//���Json���ݰ��Ƿ�����﷨�ϵĴ��󣬷���NULL��ʾ���ݰ���Ч
	{
		cJSON_Delete(Json);

		//��ӡ���ݰ��﷨�����λ��
		printf("Error before: [%s] \r\n", cJSON_GetErrorPtr());
		return 1;
	}
	else
	{
		//ƥ���Ӷ��� method
		if((Method = cJSON_GetObjectItem(Json,"method")) != NULL)
		{
			printf("---------------------------------method----------------------------\r\n");
			printf("%s: %s \r\n",Method->string,Method->valuestring);
		}
		//ƥ���Ӷ��� id
		if((Id = cJSON_GetObjectItem(Json,"id")) != NULL)
		{
			printf("-----------------------------------id------------------------------\r\n");
			printf("%s: %s \r\n",Id->string,Id->valuestring);
		}

		//ƥ���Ӷ��� params
		if((Params = cJSON_GetObjectItem(Json,"params")) != NULL)
		{
			printf("---------------------------------params----------------------------\r\n");

			if((Item = cJSON_GetObjectItem(Params,"NO")) != NULL)//ƥ���Ӷ���2�еĳ�Ա "NO"
			{
				printf("%s: %d \r\n",Item->string,Item->valueint);

				//���Ƽ̵���
				PAout(5) = Item->valueint;
			}
			if((Item = cJSON_GetObjectItem(Params,"led1")) != NULL)//ƥ���Ӷ���2�еĳ�Ա "led1"
			{
				printf("%s: %d \r\n",Item->string,Item->valueint);
				
				//����1��
				PBout(3) = !Item->valueint;
			}
			if((Item = cJSON_GetObjectItem(Params,"led2")) != NULL)//ƥ���Ӷ���2�еĳ�Ա "led2"
			{
				printf("%s: %d \r\n",Item->string,Item->valueint);
				
				//����2��
				PBout(4) = !Item->valueint;
			}
		}
	}

	//�ͷ�cJSON_Parse()����������ڴ�ռ�
	cJSON_Delete(Json);
	Json=NULL;
	
	return 0;
}
#endif
