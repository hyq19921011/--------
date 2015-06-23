
/**********************************************************
项目名称：电动车充电器控制器
功能：	  根据电池电量的不同合理控制充电电流，保护电池寿命。
作者：	  河南科技大学----黄永全
日期：    2015/06/04
**********************************************************/
/**************头文件**************/
#include <STC15F2K60S2.H>
#include "main.h"
#include "stdio.h"
#include "adc.h"
#include "led.h"
#include "key.h"
#include "pwm_dac.h"
#include "time.h"
#include "delay.h"
#include "usart.h"
#include "eeprom.h"

/***********系统变量定义**********/
WORD Time_Base = 0;//系统时基
SYSTime SYS_Time;  //系统时钟
BYTE SYS_STEP = 1; //系统执行的步骤
u16 AD_Time = 0;
u16 AD = 1023;    //采集回来的AD值
u16 AD0 = 1023;   //异常情况采集
BYTE DA = 10;      
//BYTE CurrentBase[6] = {0x00,0x80,0x67,0x33,0x19,0};//0\2.5\2.0\1.0\0.5 电流基准
//BYTE VoltageBase[6] = {0x00,0x9a,0xce,0xce,0xb4,0};//0\3\4\4\3.5       电压基准
//WORD CurrentInput[6] = {0x00,0x019f,0x00d1,0x006a,0,0};//0\2\1\0.5\      电流输入信号
BYTE CurrentBase[6] = {0x00,0x9a,0x80,0x4d,0x19,0};//0\3\2.5\1.5\1 电流基准
BYTE VoltageBase[6] = {0x00,0xb8,0xce,0xce,0xb8,0};//0\3.6\4\4\3.6       电压基准
WORD CurrentInput[6] = {0x00,0x0200,0x010a,0x00d1,0,0};//0\2.5\1.3\1.0\   电流输入信号
BYTE Current_Base = 0;
BYTE Voltage_Base = 0;
BYTE Change_Num = 0; //选择参数项
BYTE Wait_Flag = 0;
BYTE Save_Flag = 0;//存储数据的标志
BYTE Warrning_Flag = 0;//存储数据的标志
u16 time_wait =0;
u8  Current_Time = 0;

void user_app()//用户程序
{
    u8 key_stateValue;  
    u8* pKeyValue;  
    *pKeyValue=0;  
    key_stateValue=read_key(pKeyValue); 
//	if(Key_Lock==1)//锁定键，为1解除锁定，为0锁定
//	{
		if((*pKeyValue==4)&&(key_stateValue == return_keyPressed) )//调试按键
		{
			SYS_STEP++;
			if(SYS_STEP>5) 
			{
				SYS_STEP = 1;
			}
			if(SYS_STEP == 1){SYS_Time.GHour = 0;SYS_Time.GMintes = 0;SYS_Time.GSeconds = 0;}
			if(SYS_STEP == 2){SYS_Time.GHour = 6;SYS_Time.GMintes = 0;SYS_Time.GSeconds = 0;}//将时间设置到第二阶段的时间6~9小时 
			if(SYS_STEP == 3){SYS_Time.GHour = 9;SYS_Time.GMintes = 0;SYS_Time.GSeconds = 0;}//将时间设置到第二阶段的时间6~9小时 
			if(SYS_STEP == 4){SYS_Time.GHour = 12;SYS_Time.GMintes = 0;SYS_Time.GSeconds = 0;}//将时间设置到第二阶段的时间6~9小时 	
		}
		if((*pKeyValue==3)&&(key_stateValue == return_keyPressed) )//参数选择按键
		{
			Change_Num++;
			if(Change_Num>3) 
			{
				Save_Flag = 1;  //开始保存数据
				Change_Num = 0;
			}
				
		}
		if((key_stateValue == return_keyPressed)||(key_stateValue & return_keyAuto) )//长按键加连发
		{
			if((*pKeyValue==UP)&&(Change_Num!=0))
			{
				switch(Change_Num)
				{
					case 1:CurrentBase[SYS_STEP]++;
		
						   break;
					case 2:VoltageBase[SYS_STEP]++;
						
						   break;
					case 3:CurrentInput[SYS_STEP]++;
						   
							break;

				}
			}
			if((*pKeyValue==DOWN)&&(Change_Num!=0))
			{
				switch(Change_Num)
				{
					case 1:CurrentBase[SYS_STEP]--;break;
					case 2:VoltageBase[SYS_STEP]--;break;
					case 3:CurrentInput[SYS_STEP]--;break;
							
						
				}
			}

		}
//	}
	
	
	
}
void UserData_Claculite()//用户数据分析，判断该执行第几步
{
	
	if( (AD<= CurrentInput[SYS_STEP])&&(SYS_STEP < 4))//如果电流输入信号低于2.0V并且超过10S,前3步执行，
	{
//		if(AD <= 0x2c)//如果电流输入信号低于0.2V暂停计时
//		{
//			Delay_nms(100);
//			while(Get_ad_result(5)<=0x2c)
//			{
//				TR0 = 0;//暂停计时器
//				LedShanShuo();
//			}
//			TR0 = 1;//打开计时器
//		}
//		else
//		{
			Wait_Flag = 1;
			if(time_wait>=1000)//超过10S
			{		
				SYS_STEP++;//转到下一步	
				Wait_Flag = 0;
				time_wait = 0;
				Led0 = 0;
				if(SYS_STEP == 2)SYS_Time.GHour = 6;//将时间设置到第二阶段的时间6~9小时 
				if(SYS_STEP == 3)SYS_Time.GHour = 9;//将时间设置到第二阶段的时间6~9小时 
				if(SYS_STEP == 4)SYS_Time.GHour = 12;//将时间设置到第二阶段的时间6~9小时 
			}
			if(time_wait>=50)//检测10次防止错误检测
			{
				Led0 = 1;
				
			}
//		}
		
							
	}
	else
	{
		Wait_Flag = 0;//关闭等待计时
		time_wait = 0;
		Led0 = 0;
	}

	if(AD0 <= 615)//如果电流输入信号低于3V暂停计时 0x0267
	{
		Delay_nms(100);
		while(Get_ad_result(2)<=615)
		{
			TR0 = 0;//暂停计时器
			LedShanShuo();
		}
		TR0 = 1;//打开计时器
	}
	
}
void main()
{
    Key_Init();
	InitUart();
	LedPortInit();
	Timer0Init();
	AD_init();
	PWMn_init();
    while(UserData_Init0()) LedShanShuo();
	EA = 1;
	Delay_nms(1000);
	while(1)
	{
//		EA = 1;
//		TR0 = 1;
		AD0 = Get_ad_result(2);		
		AD = Get_ad_result(3);
		UserData_Claculite();
		if(Change_Num) Current_Time = 0;//此句是为了在修正是数据输出口不反转用的
		switch(SYS_STEP)// 整个系统执行流程
		{
			case 1://第一阶段 0 ~ 6小时 
			{
				
				if(Current_Time<30)//30秒2.5V
				{
					PWM0_set (0,CurrentBase[SYS_STEP]);
					
				}
				else if(Current_Time<35)
					{
						PWM0_set (0,0x33);//5秒1V
					}
					else
					{
						Current_Time = 0;
					}
				PWM0_set (1,VoltageBase[SYS_STEP]);
				if(Change_Num==0)//避免和用户调节是的指示灯互相干扰
				{
					Led1 = 1;
					Led2 = 0;
					Led3 = 0;
				}
				
				if(SYS_Time.GHour==6)SYS_STEP++;// 时间到转到下一步				
				break;
			}
			case 2://第一阶段 6 ~ 9小时 
			{
				if(Current_Time<30)//30秒2.5V
				{
					PWM0_set (0,CurrentBase[SYS_STEP]);
					
				}
				else if(Current_Time<35)
					{
						PWM0_set (0,0x33);//5秒1V
					}
					else
					{
						Current_Time = 0;
					}
				PWM0_set (1,VoltageBase[SYS_STEP]);
				if(Change_Num==0)//避免和用户调节是的指示灯互相干扰
				{
					Led1 = 0;
					Led2 = 1;
					Led3 = 0;
				}
				if(SYS_Time.GHour==9)SYS_STEP++;
				break;
			}			
			case 3://第一阶段 9 ~ 12小时 
			{
				if(Current_Time<30)//30秒2.5V
				{
					PWM0_set (0,CurrentBase[SYS_STEP]);
					
				}
				else if(Current_Time<35)
					{
						PWM0_set (0,0x19);//5秒0.5V
					}
					else
					{
						Current_Time = 0;
					}
				PWM0_set (1,VoltageBase[SYS_STEP]);
				if(Change_Num==0)//避免和用户调节是的指示灯互相干扰
				{
					Led1 = 0;
					Led2 = 0;
					Led3 = 1;
				}
				if(SYS_Time.GHour==12)SYS_STEP++;// 时间到转到下一步	
				break;
			}
			case 4://第一阶段 12 ~ 14小时 
			{
				PWM0_set (0,CurrentBase[SYS_STEP]);
				PWM0_set (1,VoltageBase[SYS_STEP]);
				if(Change_Num==0)//避免和用户调节是的指示灯互相干扰
				{
					Led1 = 0;
					Led2 = 0;
					Led3 = 0;
				}
				if(SYS_Time.GHour==14)SYS_STEP++;// 时间到转到下一步	
				break;
			}
			case 5://充满
			{
				PWM0_set (0,CurrentBase[0]);
				PWM0_set (1,VoltageBase[0]);
				if(Change_Num==0)//避免和用户调节是的指示灯互相干扰
				{
					Led1 = 0;
					Led2 = 0;
					Led3 = 0;
				}
//				SYS_STEP = 1;
				break;
			}
		}
		switch(Change_Num)
		{
			case 1:LedShanShuo1();break;
			case 2:LedShanShuo2();break;
			case 3:LedShanShuo3();break;
			case 0:  break;
		}
		if(Save_Flag == 1)
		{
			Save_Flag = 0;
			UserData_Init0();
			LedShanShuo();
		}
 		
		
	}

}
//-----------------------------------------------

/* Timer0 interrupt routine */
void tm0_isr() interrupt 1 using 1
{
	TL0 = T0MS;                     //初始化计时值
    TH0 = T0MS >> 8;
	Time_Base++;
	if(Time_Base>=100)
	{
		Time_Base = 0;
		Current_Time++;//用于输出口1 30秒和5秒重复用
		SYS_Time.GSeconds++;
		if(SYS_Time.GSeconds>59) 
		{
			SYS_Time.GSeconds=0;
			SYS_Time.GMintes++;
		}
		if(SYS_Time.GMintes>59) 
		{
			SYS_Time.GMintes=0;
			SYS_Time.GHour++;
		}
		if(SYS_Time.GHour>23) 
		{
			SYS_Time.GHour=0;
		}

	}
	if(Wait_Flag)time_wait++;//执行周期为10ms	
	user_app();                      
}
/*----------------------------
UART interrupt service routine
----------------------------*/
void Uart_Isr() interrupt 4 using 1
{
    if (RI)
    {
        RI = 0;             //Clear receive interrupt flag
        DA = SBUF; 
//		SendData(DA);
//		PWM0_set (0,DA);
//		PWM0_set (1,DA); 
		if(DA == 0xaa)
		{
			IapEraseSector(0x0000);//擦出第一个扇区
			IapEraseSector(0x0200);//擦出第二个扇区
		}
		if(DA == 1)
		{
			SendData(CurrentBase[SYS_STEP]);
			SendData(VoltageBase[SYS_STEP]);
			SendData(CurrentInput[SYS_STEP]>>8);
			SendData(CurrentInput[SYS_STEP]&0xff);
		}
		if(DA == 2)
		{
			SendData(2);
			SendData(AD0>>8);
			SendData(AD0);
			SendData(3);
			SendData(AD>>8);
			SendData(AD);
		
		}
		if(DA == 3)
		{
			SendData(SYS_Time.GHour);
			SendData(SYS_Time.GMintes);
			SendData(SYS_Time.GSeconds);
		}
		if(DA == 0x12)
		{
			SYS_Time.GHour = 5;
			SYS_Time.GMintes = 59;
			SYS_Time.GSeconds = 40;
			SendData(SYS_Time.GHour);
			SendData(SYS_Time.GMintes);
			SendData(SYS_Time.GSeconds);
		}
		if(DA == 0x23)
		{
			SYS_Time.GHour = 8;
			SYS_Time.GMintes = 59;
			SYS_Time.GSeconds = 40;
			SendData(SYS_Time.GHour);
			SendData(SYS_Time.GMintes);
			SendData(SYS_Time.GSeconds);
		}
		if(DA == 0x34)
		{
			SYS_Time.GHour = 11;
			SYS_Time.GMintes = 59;
			SYS_Time.GSeconds = 40;
			SendData(SYS_Time.GHour);
			SendData(SYS_Time.GMintes);
			SendData(SYS_Time.GSeconds);
		}
		if(DA == 0x45)
		{
			SYS_Time.GHour = 13;
			SYS_Time.GMintes = 59;
			SYS_Time.GSeconds = 40;
			SendData(SYS_Time.GHour);
			SendData(SYS_Time.GMintes);
			SendData(SYS_Time.GSeconds);
		}
    }
}
 WORD Get_SYSTime()
{
	return Time_Base;
}
