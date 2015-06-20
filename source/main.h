#ifndef _main_h_
#define _main_h_

//#define MAIN_Fosc		24000000L	//������ʱ��
//#define MAIN_Fosc		22118400L	//������ʱ��
//#define MAIN_Fosc		12000000L	//������ʱ��
#define MAIN_Fosc		11059200L	//������ʱ��
//#define MAIN_Fosc		 5529600L	//������ʱ��
#include <STC15F2K60S2.H>
enum
	{
		Step1 = 1,
		Step2 ,
		Step3 ,
		Step4 ,
		Step5 
	};
typedef struct systime
{
	BYTE GHour;
	BYTE GMintes;
	BYTE GSeconds;
}SYSTime;

extern WORD Time_Base;//ϵͳʱ��
extern SYSTime SYS_Time;  //ϵͳʱ��
extern u16 AD;
extern BYTE DA;
extern BYTE CurrentBase[6];//0\2.5\2.0\1.0\0.5 ������׼
extern BYTE VoltageBase[6];//0\3\4\4\3.5       ��ѹ��׼
extern WORD CurrentInput[6];//0\3\4\4\3.5      ���������ź�
extern BYTE KEY;
extern BYTE Time_Key;
#endif