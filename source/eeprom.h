#ifndef _eeprom_h_
#define _eeprom_h_
#include <STC15F2K60S2.H>
#include "intrins.h"

//���Ե�ַ
#define Init_Flag   0x0000     //��ʼ����־
#define IAP_ADDRESS 0x0200
#define Data1       0x200//������׼
#define Data2       0x220  //��ѹ��׼
#define Data3       0x240    //�����źŲɼ�
extern BYTE IapReadByte(WORD addr);
extern void IapProgramByte(WORD addr, BYTE dat);
extern void IapEraseSector(WORD addr);
extern BYTE  UserData_Init();
extern BYTE  UserData_Init0();
extern void BUG();
#endif