/****************

占用端口：
Led1 = P3^7;
Led2 = P3^3;
Led3 = P3^2;
Led0 = P3^6;
给高电平亮
下载器给低电平亮
****************/

#include "led.h"
#include "delay.h"
void LedPortInit()
{
	P3M0 |= 0xfc;//将P3口设置为普通IO口
	P3M1 &= 0x03;
	Led0 = 0;
	Led1 = 0;
	Led2 = 0;
	Led3 = 0;
}

void LedShanShuo()//用于报警闪烁
{
	Led0 = 0;
	Led1 = 0;
	Led2 = 0;
	Led3 = 0;
	Delay_nms(500);
	Led0 = 1;
	Led1 = 1;
	Led2 = 1;
	Led3 = 1;
	Delay_nms(500);
}
void LedShanShuo1()
{
	Led1 = 0;
	Delay_nms(500);
	Led1 = 1;
	Delay_nms(500);
}
void LedShanShuo2()
{
	Led1 = 0;
	Delay_nms(500);
	Led1 = 1;
	Delay_nms(1000);
}
void LedShanShuo3()
{
	Led1= 0;
	Delay_nms(1000);
	Led1 = 1;
	Delay_nms(500);
}
void CloseLed(BYTE i)
{
	if(i==0) Led0 = 1;
	if(i==1) Led1 = 1;
	if(i==2) Led2 = 1;
	if(i==3) Led3 = 1;
}


	
	
