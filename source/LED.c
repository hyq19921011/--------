/****************

ռ�ö˿ڣ�
Led1 = P3^7;
Led2 = P3^3;
Led3 = P3^2;
Led0 = P3^6;
���ߵ�ƽ��
���������͵�ƽ��
****************/

#include "led.h"
#include "delay.h"
void LedPortInit()
{
	P3M0 |= 0xfc;//��P3������Ϊ��ͨIO��
	P3M1 &= 0x03;
	Led0 = 0;
	Led1 = 0;
	Led2 = 0;
	Led3 = 0;
}

void LedShanShuo()//���ڱ�����˸
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


	
	
