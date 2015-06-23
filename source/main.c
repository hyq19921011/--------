
/**********************************************************
��Ŀ���ƣ��綯�������������
���ܣ�	  ���ݵ�ص����Ĳ�ͬ������Ƴ��������������������
���ߣ�	  ���ϿƼ���ѧ----����ȫ
���ڣ�    2015/06/04
**********************************************************/
/**************ͷ�ļ�**************/
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

/***********ϵͳ��������**********/
WORD Time_Base = 0;//ϵͳʱ��
SYSTime SYS_Time;  //ϵͳʱ��
BYTE SYS_STEP = 1; //ϵͳִ�еĲ���
u16 AD_Time = 0;
u16 AD = 1023;    //�ɼ�������ADֵ
u16 AD0 = 1023;   //�쳣����ɼ�
BYTE DA = 10;      
//BYTE CurrentBase[6] = {0x00,0x80,0x67,0x33,0x19,0};//0\2.5\2.0\1.0\0.5 ������׼
//BYTE VoltageBase[6] = {0x00,0x9a,0xce,0xce,0xb4,0};//0\3\4\4\3.5       ��ѹ��׼
//WORD CurrentInput[6] = {0x00,0x019f,0x00d1,0x006a,0,0};//0\2\1\0.5\      ���������ź�
BYTE CurrentBase[6] = {0x00,0x9a,0x80,0x4d,0x19,0};//0\3\2.5\1.5\1 ������׼
BYTE VoltageBase[6] = {0x00,0xb8,0xce,0xce,0xb8,0};//0\3.6\4\4\3.6       ��ѹ��׼
WORD CurrentInput[6] = {0x00,0x0200,0x010a,0x00d1,0,0};//0\2.5\1.3\1.0\   ���������ź�
BYTE Current_Base = 0;
BYTE Voltage_Base = 0;
BYTE Change_Num = 0; //ѡ�������
BYTE Wait_Flag = 0;
BYTE Save_Flag = 0;//�洢���ݵı�־
BYTE Warrning_Flag = 0;//�洢���ݵı�־
u16 time_wait =0;
u8  Current_Time = 0;

void user_app()//�û�����
{
    u8 key_stateValue;  
    u8* pKeyValue;  
    *pKeyValue=0;  
    key_stateValue=read_key(pKeyValue); 
//	if(Key_Lock==1)//��������Ϊ1���������Ϊ0����
//	{
		if((*pKeyValue==4)&&(key_stateValue == return_keyPressed) )//���԰���
		{
			SYS_STEP++;
			if(SYS_STEP>5) 
			{
				SYS_STEP = 1;
			}
			if(SYS_STEP == 1){SYS_Time.GHour = 0;SYS_Time.GMintes = 0;SYS_Time.GSeconds = 0;}
			if(SYS_STEP == 2){SYS_Time.GHour = 6;SYS_Time.GMintes = 0;SYS_Time.GSeconds = 0;}//��ʱ�����õ��ڶ��׶ε�ʱ��6~9Сʱ 
			if(SYS_STEP == 3){SYS_Time.GHour = 9;SYS_Time.GMintes = 0;SYS_Time.GSeconds = 0;}//��ʱ�����õ��ڶ��׶ε�ʱ��6~9Сʱ 
			if(SYS_STEP == 4){SYS_Time.GHour = 12;SYS_Time.GMintes = 0;SYS_Time.GSeconds = 0;}//��ʱ�����õ��ڶ��׶ε�ʱ��6~9Сʱ 	
		}
		if((*pKeyValue==3)&&(key_stateValue == return_keyPressed) )//����ѡ�񰴼�
		{
			Change_Num++;
			if(Change_Num>3) 
			{
				Save_Flag = 1;  //��ʼ��������
				Change_Num = 0;
			}
				
		}
		if((key_stateValue == return_keyPressed)||(key_stateValue & return_keyAuto) )//������������
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
void UserData_Claculite()//�û����ݷ������жϸ�ִ�еڼ���
{
	
	if( (AD<= CurrentInput[SYS_STEP])&&(SYS_STEP < 4))//������������źŵ���2.0V���ҳ���10S,ǰ3��ִ�У�
	{
//		if(AD <= 0x2c)//������������źŵ���0.2V��ͣ��ʱ
//		{
//			Delay_nms(100);
//			while(Get_ad_result(5)<=0x2c)
//			{
//				TR0 = 0;//��ͣ��ʱ��
//				LedShanShuo();
//			}
//			TR0 = 1;//�򿪼�ʱ��
//		}
//		else
//		{
			Wait_Flag = 1;
			if(time_wait>=1000)//����10S
			{		
				SYS_STEP++;//ת����һ��	
				Wait_Flag = 0;
				time_wait = 0;
				Led0 = 0;
				if(SYS_STEP == 2)SYS_Time.GHour = 6;//��ʱ�����õ��ڶ��׶ε�ʱ��6~9Сʱ 
				if(SYS_STEP == 3)SYS_Time.GHour = 9;//��ʱ�����õ��ڶ��׶ε�ʱ��6~9Сʱ 
				if(SYS_STEP == 4)SYS_Time.GHour = 12;//��ʱ�����õ��ڶ��׶ε�ʱ��6~9Сʱ 
			}
			if(time_wait>=50)//���10�η�ֹ������
			{
				Led0 = 1;
				
			}
//		}
		
							
	}
	else
	{
		Wait_Flag = 0;//�رյȴ���ʱ
		time_wait = 0;
		Led0 = 0;
	}

	if(AD0 <= 615)//������������źŵ���3V��ͣ��ʱ 0x0267
	{
		Delay_nms(100);
		while(Get_ad_result(2)<=615)
		{
			TR0 = 0;//��ͣ��ʱ��
			LedShanShuo();
		}
		TR0 = 1;//�򿪼�ʱ��
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
		if(Change_Num) Current_Time = 0;//�˾���Ϊ������������������ڲ���ת�õ�
		switch(SYS_STEP)// ����ϵͳִ������
		{
			case 1://��һ�׶� 0 ~ 6Сʱ 
			{
				
				if(Current_Time<30)//30��2.5V
				{
					PWM0_set (0,CurrentBase[SYS_STEP]);
					
				}
				else if(Current_Time<35)
					{
						PWM0_set (0,0x33);//5��1V
					}
					else
					{
						Current_Time = 0;
					}
				PWM0_set (1,VoltageBase[SYS_STEP]);
				if(Change_Num==0)//������û������ǵ�ָʾ�ƻ������
				{
					Led1 = 1;
					Led2 = 0;
					Led3 = 0;
				}
				
				if(SYS_Time.GHour==6)SYS_STEP++;// ʱ�䵽ת����һ��				
				break;
			}
			case 2://��һ�׶� 6 ~ 9Сʱ 
			{
				if(Current_Time<30)//30��2.5V
				{
					PWM0_set (0,CurrentBase[SYS_STEP]);
					
				}
				else if(Current_Time<35)
					{
						PWM0_set (0,0x33);//5��1V
					}
					else
					{
						Current_Time = 0;
					}
				PWM0_set (1,VoltageBase[SYS_STEP]);
				if(Change_Num==0)//������û������ǵ�ָʾ�ƻ������
				{
					Led1 = 0;
					Led2 = 1;
					Led3 = 0;
				}
				if(SYS_Time.GHour==9)SYS_STEP++;
				break;
			}			
			case 3://��һ�׶� 9 ~ 12Сʱ 
			{
				if(Current_Time<30)//30��2.5V
				{
					PWM0_set (0,CurrentBase[SYS_STEP]);
					
				}
				else if(Current_Time<35)
					{
						PWM0_set (0,0x19);//5��0.5V
					}
					else
					{
						Current_Time = 0;
					}
				PWM0_set (1,VoltageBase[SYS_STEP]);
				if(Change_Num==0)//������û������ǵ�ָʾ�ƻ������
				{
					Led1 = 0;
					Led2 = 0;
					Led3 = 1;
				}
				if(SYS_Time.GHour==12)SYS_STEP++;// ʱ�䵽ת����һ��	
				break;
			}
			case 4://��һ�׶� 12 ~ 14Сʱ 
			{
				PWM0_set (0,CurrentBase[SYS_STEP]);
				PWM0_set (1,VoltageBase[SYS_STEP]);
				if(Change_Num==0)//������û������ǵ�ָʾ�ƻ������
				{
					Led1 = 0;
					Led2 = 0;
					Led3 = 0;
				}
				if(SYS_Time.GHour==14)SYS_STEP++;// ʱ�䵽ת����һ��	
				break;
			}
			case 5://����
			{
				PWM0_set (0,CurrentBase[0]);
				PWM0_set (1,VoltageBase[0]);
				if(Change_Num==0)//������û������ǵ�ָʾ�ƻ������
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
	TL0 = T0MS;                     //��ʼ����ʱֵ
    TH0 = T0MS >> 8;
	Time_Base++;
	if(Time_Base>=100)
	{
		Time_Base = 0;
		Current_Time++;//���������1 30���5���ظ���
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
	if(Wait_Flag)time_wait++;//ִ������Ϊ10ms	
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
			IapEraseSector(0x0000);//������һ������
			IapEraseSector(0x0200);//�����ڶ�������
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
