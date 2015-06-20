#ifndef _time_h_
#define _time_h_
#include <STC15F2K60S2.H>
#define FOSC 11059200L

#define T0MS (65536-FOSC/12/100)      //10ms
extern void Timer0Init();
extern void Timer2Init(void);
#endif