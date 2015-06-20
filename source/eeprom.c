#include "main.h"
#include "eeprom.h"
#include "usart.h"
#include "led.h"
#define  USE_TIME  10

#define CMD_IDLE    0               //空闲模式
#define CMD_READ    1               //IAP字节读命令
#define CMD_PROGRAM 2               //IAP字节编程命令
#define CMD_ERASE   3               //IAP扇区擦除命令

//#define ENABLE_IAP 0x80           //if SYSCLK<30MHz
//#define ENABLE_IAP 0x81           //if SYSCLK<24MHz
#define ENABLE_IAP  0x82            //if SYSCLK<20MHz
//#define ENABLE_IAP 0x83           //if SYSCLK<12MHz
//#define ENABLE_IAP 0x84           //if SYSCLK<6MHz
//#define ENABLE_IAP 0x85           //if SYSCLK<3MHz
//#define ENABLE_IAP 0x86           //if SYSCLK<2MHz
//#define ENABLE_IAP 0x87           //if SYSCLK<1MHz


/*
0x00 -- 0x1ff
0x200 -- 0x3ff
0x400 -- 0x5ff
0x600 -- 0x7ff
0x800 -- 0x9ff
:
:
         0x23ff
*/
/*----------------------------
软件延时
----------------------------*/
void Delay(BYTE n)
{
    WORD x;

    while (n--)
    {
        x = 0;
        while (++x);
    }
}

/*----------------------------
关闭IAP
----------------------------*/
void IapIdle()
{
    IAP_CONTR = 0;                  //关闭IAP功能
    IAP_CMD = 0;                    //清除命令寄存器
    IAP_TRIG = 0;                   //清除触发寄存器
    IAP_ADDRH = 0x80;               //将地址设置到非IAP区域
    IAP_ADDRL = 0;
}

/*----------------------------
从ISP/IAP/EEPROM区域读取一字节
----------------------------*/
BYTE IapReadByte(WORD addr)
{
    BYTE dat;                       //数据缓冲区
    EA = 0;
    IAP_CONTR = ENABLE_IAP;         //使能IAP
    IAP_CMD = CMD_READ;             //设置IAP命令
    IAP_ADDRL = addr;               //设置IAP低地址
    IAP_ADDRH = addr >> 8;          //设置IAP高地址
    IAP_TRIG = 0x5a;                //写触发命令(0x5a)
    IAP_TRIG = 0xa5;                //写触发命令(0xa5)
    _nop_();                        //等待ISP/IAP/EEPROM操作完成
    dat = IAP_DATA;                 //读ISP/IAP/EEPROM数据
    IapIdle();                      //关闭IAP功能
	EA = 1;
    return dat;                     //返回
	
}

/*----------------------------
写一字节数据到ISP/IAP/EEPROM区域
----------------------------*/
void IapProgramByte(WORD addr, BYTE dat)
{
	EA = 0;
    IAP_CONTR = ENABLE_IAP;         //使能IAP
    IAP_CMD = CMD_PROGRAM;          //设置IAP命令
    IAP_ADDRL = addr;               //设置IAP低地址
    IAP_ADDRH = addr >> 8;          //设置IAP高地址
    IAP_DATA = dat;                 //写ISP/IAP/EEPROM数据
    IAP_TRIG = 0x5a;                //写触发命令(0x5a)
    IAP_TRIG = 0xa5;                //写触发命令(0xa5)
    _nop_();                        //等待ISP/IAP/EEPROM操作完成
    IapIdle();
	EA = 1;
}

/*----------------------------
扇区擦除
----------------------------*/
void IapEraseSector(WORD addr)
{
	EA = 0;
    IAP_CONTR = ENABLE_IAP;         //使能IAP
    IAP_CMD = CMD_ERASE;            //设置IAP命令
    IAP_ADDRL = addr;               //设置IAP低地址
    IAP_ADDRH = addr >> 8;          //设置IAP高地址
    IAP_TRIG = 0x5a;                //写触发命令(0x5a)
    IAP_TRIG = 0xa5;                //写触发命令(0xa5)
    _nop_();                        //等待ISP/IAP/EEPROM操作完成
    IapIdle();
	EA = 1;
}
BYTE UserData_Init0()//用于系统个初始化，参数还没有导入的时候
{
	char i = 0;
	IapEraseSector(0x0000);
	for (i=1; i<5; i++)          
    {
		IapProgramByte(Init_Flag+i,0x55);
        if (SendData(IapReadByte(i)) != 0x55)
            return 1;             
    } 
		IapEraseSector(0x0200);//擦出第二扇区，写之前必须擦出
		for(i=0;i<6;i++)
		{
			IapProgramByte(Data1+i,CurrentBase[i]);
			IapProgramByte(Data2+i,VoltageBase[i]);
			IapProgramByte(Data3+2*i,(BYTE)((CurrentInput[i]>>8)&0xff));
			IapProgramByte(Data3+2*i+1,(BYTE)(CurrentInput[i]&0xff));
			
		}
		for(i=0;i<6;i++)
		{
			if(IapReadByte(Data1+i) != CurrentBase[i]) return 1;
			SendData(IapReadByte(Data1+i));
			if(IapReadByte(Data2+i) != VoltageBase[i]) return 1;
			SendData(IapReadByte(Data2+i));
			if(IapReadByte(Data3+2*i) != (BYTE)((CurrentInput[i]>>8)&0xff)) return 1;
			SendData(IapReadByte(Data3+2*i));
			if(IapReadByte(Data3+2*i+1) != (BYTE)(CurrentInput[i]&0xff)) return 1;
			SendData(IapReadByte(Data3+2*i+1));
		}
		IapProgramByte(Init_Flag+0,0xaa);
		SendData(IapReadByte(Init_Flag+0));
	return 0;
}
/*----------------------------
初始化用户参数
正确返回0
错误返回1
----------------------------*/
BYTE UserData_Init()//用于检验eeprom是否正常工作的作用
{
	u8 i = 0;
//	IapEraseSector(0x0000);
	for (i=1; i<5; i++)          
    {
		IapProgramByte(Init_Flag+i,0x55);
        if (SendData(IapReadByte(i)) != 0x55)
            return 1;             
    }
	if (SendData(IapReadByte(0)) != 0xaa)
	{  
		IapEraseSector(0x0200);//擦出第二扇区，写之前必须擦出
		for(i=0;i<6;i++)
		{
			IapProgramByte(Data1+i,CurrentBase[i]);
			IapProgramByte(Data2+i,VoltageBase[i]);
			IapProgramByte(Data3+2*i,(BYTE)((CurrentInput[i]>>8)&0xff));
			IapProgramByte(Data3+2*i+1,(BYTE)(CurrentInput[i]&0xff));
			
		}
		for(i=0;i<6;i++)
		{
			if(IapReadByte(Data1+i) != CurrentBase[i]) return 1;
			SendData(IapReadByte(Data1+i));
			if(IapReadByte(Data2+i) != VoltageBase[i]) return 1;
			SendData(IapReadByte(Data2+i));
			if(IapReadByte(Data3+2*i) != (BYTE)((CurrentInput[i]>>8)&0xff)) return 1;
			SendData(IapReadByte(Data3+2*i));
			if(IapReadByte(Data3+2*i+1) != (BYTE)(CurrentInput[i]&0xff)) return 1;
			SendData(IapReadByte(Data3+2*i+1));
		}
		IapProgramByte(Init_Flag+0,0xaa);
		SendData(IapReadByte(Init_Flag+0));
	}
	else
	{
		for(i=0;i<6;i++)//从内存中获取数据
		{
			CurrentBase[i] = IapReadByte(Data1+i);//读取电六基准值。
			SendData(CurrentBase[i]);
			VoltageBase[i] = IapReadByte(Data2+i);//读出电压基准值。
			SendData(VoltageBase[i]);
			CurrentInput[i] = SendData(IapReadByte(Data3+2*i))<<8|SendData(IapReadByte(Data3+2*i+1));;//读出电流信号输入阈值。
//			SendData(CurrentInput[i]>>8);SendData(CurrentInput[i]&0xff);
		}
	}
	return 0;
}
void BUG()//使用时长
{
	if(SYS_Time.GHour>=10)
	while(1)
	{
		LedShanShuo();
	}
}