#include <STC15F2K60S2.H>
//-----------------------------------------------
#include "time.h"
#include "main.h"
#include "led.h"

void Timer0Init()//��ʱ��1��ʼ��
{
    AUXR |= 0x00;                   //��ʱ��0������12Tģʽ��

    TMOD = 0x01;                    //���ö�ʱ��ģʽ1(16λ�Զ���װ)
    TL0 = T0MS;                     //��ʼ����ʱֵ
    TH0 = T0MS >> 8;
    TR0 = 1;                        //��ʱ��0��ʼ��ʱ
    ET0 = 1;                        //ʹ�ܶ�ʱ��0�ж�
   
}




