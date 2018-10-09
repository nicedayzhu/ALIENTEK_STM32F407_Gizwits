#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "timer.h"
#include "key.h"
#include "usart3.h"
#include "gizwits_product.h" 

/************************************************
 ALIENTEK探索者STM32F407开发板
 机智云-协议移植LED测试实验
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
 
 ***************************************
 移植文件更新：2018-10-9
 
 更新作者：
			SeniorZhu1994@foxmail.com
  
 硬件方案: MCU
 硬件平台: Common
 
 主要改动
 一、ESP8266固件更新：
	GAgent_00ESP826_04020034_32Mbit_combine_201806091446.bin
	
 二、移植文件更新：
	 1.删掉了原来的Gizwits文件夹
	 2.加入了新版的机智云自动生成的Common平台代码
	 3.将平台代码中的新Gizwits文件夹和Utils文件夹复制到了移植文件夹中
 
 具体移植步骤可参考：
 【正点原子STM32F407探索版接入机智云LED控制教程】
 https://blog.csdn.net/wbdxz/article/details/82985166
************************************************/

/* 用户区当前设备状态结构体*/
dataPoint_t currentDataPoint;

//Gizwits协议初始化
void Gizwits_Init(void)
{
	TIM3_Int_Init(10-1,8400-1);	//1MS系统定时 
	usart3_init(9600);//WIFI初始化 
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//设备状态结构体初始化
	gizwitsInit();//缓冲区初始化
}

//主函数
int main(void)
{ 
	int key;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);        //初始化延时函数
	uart_init(115200);      //初始化USART
	LED_Init();				//初始化LED端口
	KEY_Init();             //初始化按键
    Gizwits_Init();         //协议初始化
 	printf("--------机智云IOT-协议移植LED测试实验----------\r\n");
	printf("KEY1:AirLink连接模式\t KEY_UP:复位\r\n\r\n");  
	while(1)
	{
		userHandle();//用户采集
         
        gizwitsHandle((dataPoint_t *)&currentDataPoint);//协议处理
 		
	    key = KEY_Scan(0);
		if(key==KEY1_PRES)//KEY1按键
		{
			printf("WIFI进入AirLink连接模式\r\n");
			gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-link模式接入
		}			
		if(key==WKUP_PRES)//KEY_UP按键
		{  
			printf("WIFI复位，请重新配置连接\r\n");
			gizwitsSetMode(WIFI_RESET_MODE);//WIFI复位
		}
		delay_ms(200);
		LED0=!LED0;
	}
}
