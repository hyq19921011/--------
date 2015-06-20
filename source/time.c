#include <STC15F2K60S2.H>
//-----------------------------------------------
#include "time.h"
#include "main.h"
#include "led.h"

void Timer0Init()//定时器1初始化
{
    AUXR |= 0x00;                   //定时器0工作在12T模式下

    TMOD = 0x01;                    //设置定时器模式1(16位自动重装)
    TL0 = T0MS;                     //初始化计时值
    TH0 = T0MS >> 8;
    TR0 = 1;                        //定时器0开始计时
    ET0 = 1;                        //使能定时器0中断
   
}




