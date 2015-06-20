#ifndef _led_h_
#define _led_h_

#include <STC15F2K60S2.H>
#include "delay.h"
sbit Led0 = P3^7;
sbit Led3 = P3^6;
sbit Led2 = P3^3;
sbit Led1 = P3^2;
extern void LedPortInit();
extern void LedShanShuo();
extern void LedShanShuo1();
extern void LedShanShuo2();
extern void LedShanShuo3();
extern void CloseLed(BYTE i);
#endif