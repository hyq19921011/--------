#ifndef _key_h_
#define _key_h_
#include <STC15F2K60S2.H>
//sbit Key_Up =   P1^2;
//sbit Key_Down=	P1^3;
//sbit Key_Choose=P1^4;
//sbit Key_Debug    =P5^4;//调试按键
//sbit Key_Lock    =P5^5;//锁定定端口
sbit Key_Up =   P1^3;
sbit Key_Down=	P1^4;
sbit Key_Choose=P5^4;
sbit Key_Debug    =P5^5;//调试按键
//sbit Key_Lock    =P5^5;//锁定定端口
#define UP     1
#define DOWN   2
#define CHOOSE 3
#define DEBUG  4

#define state_keyUp         0       //初始状态，未按键 
#define state_keyDown       1       //键被按下 
#define state_keyLong       2       //长按  
#define state_keyTime       3       //按键计时态 
  
#define return_keyUp        0x00    //初始状态  
#define return_keyPressed   0x01    //键被按下，普通按键  
#define return_keyLong      0x02    //长按  
#define return_keyAuto      0x04    //自动连发 
  
#define key_down             0      //按下  
#define key_up               0    //未按下时的key有效值 
#define key_longTimes       150     //10ms一次,200次即两秒,定义长按键判定时间 
#define key_autoTimes       100      //连发时间定义,20*10=200,200毫秒发一次  

  
extern void Key_Init();
extern BYTE read_key(u8* pKeyValue) ;
#endif