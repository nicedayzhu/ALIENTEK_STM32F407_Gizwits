#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "timer.h"
#include "key.h"
#include "usart3.h"
#include "gizwits_product.h" 

/************************************************
 ALIENTEK̽����STM32F407������
 ������-Э����ֲLED����ʵ��
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
 
 ***************************************
 ��ֲ�ļ����£�2018-10-9
 
 �������ߣ�
			SeniorZhu1994@foxmail.com
  
 Ӳ������: MCU
 Ӳ��ƽ̨: Common
 
 ��Ҫ�Ķ�
 һ��ESP8266�̼����£�
	GAgent_00ESP826_04020034_32Mbit_combine_201806091446.bin
	
 ������ֲ�ļ����£�
	 1.ɾ����ԭ����Gizwits�ļ���
	 2.�������°�Ļ������Զ����ɵ�Commonƽ̨����
	 3.��ƽ̨�����е���Gizwits�ļ��к�Utils�ļ��и��Ƶ�����ֲ�ļ�����
 
 ������ֲ����ɲο���
 ������ԭ��STM32F407̽������������LED���ƽ̡̳�
 https://blog.csdn.net/wbdxz/article/details/82985166
************************************************/

/* �û�����ǰ�豸״̬�ṹ��*/
dataPoint_t currentDataPoint;

//GizwitsЭ���ʼ��
void Gizwits_Init(void)
{
	TIM3_Int_Init(10-1,8400-1);	//1MSϵͳ��ʱ 
	usart3_init(9600);//WIFI��ʼ�� 
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//�豸״̬�ṹ���ʼ��
	gizwitsInit();//��������ʼ��
}

//������
int main(void)
{ 
	int key;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);        //��ʼ����ʱ����
	uart_init(115200);      //��ʼ��USART
	LED_Init();				//��ʼ��LED�˿�
	KEY_Init();             //��ʼ������
    Gizwits_Init();         //Э���ʼ��
 	printf("--------������IOT-Э����ֲLED����ʵ��----------\r\n");
	printf("KEY1:AirLink����ģʽ\t KEY_UP:��λ\r\n\r\n");  
	while(1)
	{
		userHandle();//�û��ɼ�
         
        gizwitsHandle((dataPoint_t *)&currentDataPoint);//Э�鴦��
 		
	    key = KEY_Scan(0);
		if(key==KEY1_PRES)//KEY1����
		{
			printf("WIFI����AirLink����ģʽ\r\n");
			gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-linkģʽ����
		}			
		if(key==WKUP_PRES)//KEY_UP����
		{  
			printf("WIFI��λ����������������\r\n");
			gizwitsSetMode(WIFI_RESET_MODE);//WIFI��λ
		}
		delay_ms(200);
		LED0=!LED0;
	}
}
