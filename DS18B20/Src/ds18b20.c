#include "main.h"
#include "ds18b20.h"
#include "tim.h"
//��λDS18B20
void DS18B20_Rst(void)     
{                 
	CLR_DS18B20();    //����DQ
	HAL_Delay_us(750);    //����750us
	SET_DS18B20();;   //DQ=1 
	HAL_Delay_us(15);     //15US
}

//�ȴ�DS18B20�Ļ�Ӧ
//����1:δ��⵽ds18b20�Ĵ���
//����0:����
uint8_t DS18B20_Check(void)        
{   
	uint8_t retry=0;

	while (DS18B20_DQ_IN&&retry<200)
	{
		retry++;
		HAL_Delay_us(1);
	};   
	if(retry>=200)return 1;
	else retry=0;
	while (!DS18B20_DQ_IN&&retry<240)
	{
		retry++;
		HAL_Delay_us(1);
	};
	if(retry>=240)return 1;     
	return 0;
}
//��DS18B20��ȡһ��λ
//����ֵ:1/0
uint8_t DS18B20_Read_Bit(void)           // read one bit
{
	uint8_t data;
	CLR_DS18B20();   //����DQ 
	HAL_Delay_us(2);
	SET_DS18B20();;  //DQ=1 
	HAL_Delay_us(12);
	if(DS18B20_DQ_IN)data=1;
	else data=0;   
	HAL_Delay_us(50);  
	return data;
}
//��ds18b20��ȡһ���ֽ�
//����ֵ:��ȡ��������
uint8_t DS18B20_Read_Byte(void)    // read one byte
{        
	uint8_t i,j,dat;
	dat=0;
	for (i=1;i<=8;i++) 
	{
		j=DS18B20_Read_Bit();
		dat=(j<<7)|(dat>>1);
	}                           
	return dat;
}
//*�������� www.stm32cube.com �ṩ
//дһ���ֽڵ�DS18B20
//dat:Ҫд����ֽ�
void DS18B20_Write_Byte(uint8_t dat)     
{             
	uint8_t j;
	uint8_t testb;
	for (j=1;j<=8;j++) 
	{
		testb=dat&0x01;
		dat=dat>>1;
		if (testb) 
		{
			CLR_DS18B20(); //DS18B20_DQ_OUT=0;// Write 1
			HAL_Delay_us(2);                            
			SET_DS18B20(); //DS18B20_DQ_OUT=1;
			HAL_Delay_us(60);             
		}
		else 
		{
			CLR_DS18B20(); //DS18B20_DQ_OUT=0;// Write 0
			HAL_Delay_us(60);             
			SET_DS18B20(); //DS18B20_DQ_OUT=1;
			HAL_Delay_us(2);                          
		}
	}
}


//��ʼ�¶�ת��
void DS18B20_Start(void)// ds1820 start convert
{                                          
	DS18B20_Rst();     
	DS18B20_Check();   
	DS18B20_Write_Byte(0xcc);// skip rom
	DS18B20_Write_Byte(0x44);// convert
} 


//��ʼ��DS18B20��IO��DQͬʱ���DS�Ĵ���
//����1:������
//����0:����         
uint8_t DS18B20_Init(void)
{
	//����PA2��    
	GPIO_InitTypeDef GPIO_InitStruct;

	/* GPIO Ports Clock Enable */
	__GPIOA_CLK_ENABLE();

	/*Configure GPIO pin : PA2 */
	GPIO_InitStruct.Pin = DS18B20_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(DS18B20_GPIO_Port, &GPIO_InitStruct);

	SET_DS18B20();         //ʹPA2����ߵ�ƽ

	DS18B20_Rst();
	return DS18B20_Check();
} 


//��ds18b20�õ��¶�ֵ
//���ȣ�0.1C
//����ֵ���¶�ֵ ��-550~1250�� 
float DS18B20_Get_Temp(void)
{
    uint8_t temp;
    uint8_t TL,TH;
		int16_t tem;
    DS18B20_Start ();                    // ds1820 start convert
    DS18B20_Rst();
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0xbe);// convert	    
    TL=DS18B20_Read_Byte(); // LSB   
    TH=DS18B20_Read_Byte(); // MSB  
	    	  
    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;//�¶�Ϊ��  
    }else temp=1;//�¶�Ϊ��	  	  
    tem=TH; //��ø߰�λ
    tem<<=8;    
    tem+=TL;//��õͰ�λ
    tem=(float)tem*0.625;//ת��     
	if(temp)
	{
		return tem/10.00; //�����¶�ֵ
	}
	else 
	{
		return -tem/10.00;
	}		
}

