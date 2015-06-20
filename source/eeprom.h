#ifndef _eeprom_h_
#define _eeprom_h_
#include <STC15F2K60S2.H>
#include "intrins.h"

//测试地址
#define Init_Flag   0x0000     //初始化标志
#define IAP_ADDRESS 0x0200
#define Data1       0x200//电流基准
#define Data2       0x220  //电压基准
#define Data3       0x240    //电流信号采集
extern BYTE IapReadByte(WORD addr);
extern void IapProgramByte(WORD addr, BYTE dat);
extern void IapEraseSector(WORD addr);
extern BYTE  UserData_Init();
extern BYTE  UserData_Init0();
extern void BUG();
#endif