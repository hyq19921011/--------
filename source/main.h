#ifndef _main_h_
#define _main_h_

//#define MAIN_Fosc		24000000L	//定义主时钟
//#define MAIN_Fosc		22118400L	//定义主时钟
//#define MAIN_Fosc		12000000L	//定义主时钟
#define MAIN_Fosc		11059200L	//定义主时钟
//#define MAIN_Fosc		 5529600L	//定义主时钟
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

extern WORD Time_Base;//系统时基
extern SYSTime SYS_Time;  //系统时钟
extern u16 AD;
extern BYTE DA;
extern BYTE CurrentBase[6];//0\2.5\2.0\1.0\0.5 电流基准
extern BYTE VoltageBase[6];//0\3\4\4\3.5       电压基准
extern WORD CurrentInput[6];//0\3\4\4\3.5      电流输入信号
extern BYTE KEY;
extern BYTE Time_Key;
#endif