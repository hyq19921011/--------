#ifndef _key_h_
#define _key_h_
#include <STC15F2K60S2.H>
//sbit Key_Up =   P1^2;
//sbit Key_Down=	P1^3;
//sbit Key_Choose=P1^4;
//sbit Key_Debug    =P5^4;//���԰���
//sbit Key_Lock    =P5^5;//�������˿�
sbit Key_Up =   P1^3;
sbit Key_Down=	P1^4;
sbit Key_Choose=P5^4;
sbit Key_Debug    =P5^5;//���԰���
//sbit Key_Lock    =P5^5;//�������˿�
#define UP     1
#define DOWN   2
#define CHOOSE 3
#define DEBUG  4

#define state_keyUp         0       //��ʼ״̬��δ���� 
#define state_keyDown       1       //�������� 
#define state_keyLong       2       //����  
#define state_keyTime       3       //������ʱ̬ 
  
#define return_keyUp        0x00    //��ʼ״̬  
#define return_keyPressed   0x01    //�������£���ͨ����  
#define return_keyLong      0x02    //����  
#define return_keyAuto      0x04    //�Զ����� 
  
#define key_down             0      //����  
#define key_up               0    //δ����ʱ��key��Чֵ 
#define key_longTimes       150     //10msһ��,200�μ�����,���峤�����ж�ʱ�� 
#define key_autoTimes       100      //����ʱ�䶨��,20*10=200,200���뷢һ��  

  
extern void Key_Init();
extern BYTE read_key(u8* pKeyValue) ;
#endif