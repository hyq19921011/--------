
/**********************************************************
ÏîÄ¿Ãû³Æ£ºµç¶¯³µ³äµçÆ÷¿ØÖÆÆ÷
¹¦ÄÜ£º	  ¸ù¾İµç³ØµçÁ¿µÄ²»Í¬ºÏÀí¿ØÖÆ³äµçµçÁ÷£¬±£»¤µç³ØÊÙÃü¡£
×÷Õß£º	  ºÓÄÏ¿Æ¼¼´óÑ§----»ÆÓÀÈ«
ÈÕÆÚ£º    2015/06/04
**********************************************************/
/**************Í·ÎÄ¼ş**************/
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

/***********ÏµÍ³±äÁ¿¶¨Òå**********/
WORD Time_Base = 0;//ÏµÍ³Ê±»ù
SYSTime SYS_Time;  //ÏµÍ³Ê±ÖÓ
BYTE SYS_STEP = 1; //ÏµÍ³Ö´ĞĞµÄ²½Öè
u16 AD_Time = 0;
u16 AD = 1023;    //²É¼¯»ØÀ´µÄADÖµ
BYTE DA = 10;      
BYTE CurrentBase[6] = {0x00,0x80,0x67,0x33,0x19,0};//0\2.5\2.0\1.0\0.5 µçÁ÷»ù×¼
BYTE VoltageBase[6] = {0x00,0x9a,0xce,0xce,0xb4,0};//0\3\4\4\3.5       µçÑ¹»ù×¼
WORD CurrentInput[6] = {0x00,0x019f,0x00d1,0x006a,0,0};//0\2\1\0.5\      µçÁ÷ÊäÈëĞÅºÅ
BYTE Current_Base = 0;
BYTE Voltage_Base = 0;
BYTE Change_Num = 0; //Ñ¡Ôñ²ÎÊıÏî
BYTE Wait_Flag = 0;
BYTE Save_Flag = 0;//´æ´¢Êı¾İµÄ±êÖ¾
BYTE Warrning_Flag = 0;//´æ´¢Êı¾İµÄ±êÖ¾
u16 time_wait =0;
u8  Current_Time = 0;

void user_app()//ÓÃ»§³ÌĞò
{
    u8 key_stateValue;  
    u8* pKeyValue;  
    *pKeyValue=0;  
    key_stateValue=read_key(pKeyValue); 
	if(Key_Lock==1)//Ëø¶¨¼ü£¬Îª1½â³ıËø¶¨£¬Îª0Ëø¶¨
	{
		if((*pKeyValue==4)&&(key_stateValue == return_keyPressed) )//µ÷ÊÔ°´¼ü
		{
			SYS_STEP++;
			if(SYS_STEP>5) 
			{
				SYS_STEP = 1;
			}
			if(SYS_STEP == 1){SYS_Time.GHour = 0;SYS_Time.GMintes = 0;SYS_Time.GSeconds = 0;}
			if(SYS_STEP == 2){SYS_Time.GHour = 6;SYS_Time.GMintes = 0;SYS_Time.GSeconds = 0;}//½«Ê±¼äÉèÖÃµ½µÚ¶ş½×¶ÎµÄÊ±¼ä6~9Ğ¡Ê± 
			if(SYS_STEP == 3){SYS_Time.GHour = 9;SYS_Time.GMintes = 0;SYS_Time.GSeconds = 0;}//½«Ê±¼äÉèÖÃµ½µÚ¶ş½×¶ÎµÄÊ±¼ä6~9Ğ¡Ê± 
			if(SYS_STEP == 4){SYS_Time.GHour = 12;SYS_Time.GMintes = 0;SYS_Time.GSeconds = 0;}//½«Ê±¼äÉèÖÃµ½µÚ¶ş½×¶ÎµÄÊ±¼ä6~9Ğ¡Ê± 	
		}
		if((*pKeyValue==3)&&(key_stateValue == return_keyPressed) )//²ÎÊıÑ¡Ôñ°´¼ü
		{
			Change_Num++;
			if(Change_Num>3) 
			{
				Save_Flag = 1;  //¿ªÊ¼±£´æÊı¾İ
				Change_Num = 0;
			}
				
		}
		if((key_stateValue == return_keyPressed)||(key_stateValue & return_keyAuto) )//³¤°´¼ü¼ÓÁ¬·¢
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
	}
	
	
	
}
void UserData_Claculite()//ÓÃ»§Êı¾İ·ÖÎö£¬ÅĞ¶Ï¸ÃÖ´ĞĞµÚ¼¸²½
{
	
	if( (AD<= CurrentInput[SYS_STEP])&&(SYS_STEP < 4))//Èç¹ûµçÁ÷ÊäÈëĞÅºÅµÍÓÚ2.0V²¢ÇÒ³¬¹ı10S,Ç°3²½Ö´ĞĞ£¬
	{
		if(AD <= 0x2c)//Èç¹ûµçÁ÷ÊäÈëĞÅºÅµÍÓÚ0.2VÔİÍ£¼ÆÊ±
		{
			Delay_nms(100);
			while(Get_ad_result(5)<=0x2c)
			{
				TR0 = 0;//ÔİÍ£¼ÆÊ±Æ÷
				LedShanShuo();
			}
			TR0 = 1;//´ò¿ª¼ÆÊ±Æ÷
		}
		else
		{
			Wait_Flag = 1;
			if(time_wait>=1000)//³¬¹ı10S
			{		
				SYS_STEP++;//×ªµ½ÏÂÒ»²½	
				Wait_Flag = 0;
				time_wait = 0;
				Led0 = 0;
				if(SYS_STEP == 2)SYS_Time.GHour = 6;//½«Ê±¼äÉèÖÃµ½µÚ¶ş½×¶ÎµÄÊ±¼ä6~9Ğ¡Ê± 
				if(SYS_STEP == 3)SYS_Time.GHour = 9;//½«Ê±¼äÉèÖÃµ½µÚ¶ş½×¶ÎµÄÊ±¼ä6~9Ğ¡Ê± 
				if(SYS_STEP == 4)SYS_Time.GHour = 12;//½«Ê±¼äÉèÖÃµ½µÚ¶ş½×¶ÎµÄÊ±¼ä6~9Ğ¡Ê± 
			}
			if(time_wait>=50)//¼ì²â10´Î·ÀÖ¹´íÎó¼ì²â
			{
				Led0 = 1;
				
			}
		}
		
							
	}
	else
	{
		Wait_Flag = 0;//¹Ø±ÕµÈ´ı¼ÆÊ±
		time_wait = 0;
		Led0 = 0;
	}

<<<<<<< HEAD
	if(AD0 <= 615)//Èç¹ûµçÁ÷ÊäÈëĞÅºÅµÍÓÚ3VÔİÍ£¼ÆÊ± 0x0267
	{
		Delay_nms(100);
		while(Get_ad_result(2)<=615)
		{
			TR0 = 0;//ÔİÍ£¼ÆÊ±Æ÷
			LedShanShuo();
		}
		TR0 = 1;//´ò¿ª¼ÆÊ±Æ÷
	}
=======
	
>>>>>>> parent of 838f16f... è¯´æ˜ï¼šä¿®æ”¹è‡³æœ€æ–°ï¼ŒæŒ‰é”®æœ‰å¤§çš„æ”¹åŠ¨ï¼Œå¢åŠ ä¿¡å·æ£€æµ‹ç«¯å£ã€‚
	
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
<<<<<<< HEAD
		AD0 = Get_ad_result(2);		
		AD = Get_ad_result(3);
=======
		AD = Get_ad_result(5);
>>>>>>> parent of 838f16f... è¯´æ˜ï¼šä¿®æ”¹è‡³æœ€æ–°ï¼ŒæŒ‰é”®æœ‰å¤§çš„æ”¹åŠ¨ï¼Œå¢åŠ ä¿¡å·æ£€æµ‹ç«¯å£ã€‚
		UserData_Claculite();
		if(Change_Num) Current_Time = 0;//´Ë¾äÊÇÎªÁËÔÚĞŞÕıÊÇÊı¾İÊä³ö¿Ú²»·´×ªÓÃµÄ
		switch(SYS_STEP)// Õû¸öÏµÍ³Ö´ĞĞÁ÷³Ì
		{
			case 1://µÚÒ»½×¶Î 0 ~ 6Ğ¡Ê± 
			{
				
				if(Current_Time<30)//30Ãë2.5V
				{
					PWM0_set (0,CurrentBase[SYS_STEP]);
					
				}
				else if(Current_Time<35)
					{
						PWM0_set (0,0x33);//5Ãë1V
					}
					else
					{
						Current_Time = 0;
					}
				PWM0_set (1,VoltageBase[SYS_STEP]);
				if(Change_Num==0)//±ÜÃâºÍÓÃ»§µ÷½ÚÊÇµÄÖ¸Ê¾µÆ»¥Ïà¸ÉÈÅ
				{
					Led1 = 1;
					Led2 = 0;
					Led3 = 0;
				}
				
				if(SYS_Time.GHour==6)SYS_STEP++;// Ê±¼äµ½×ªµ½ÏÂÒ»²½				
				break;
			}
			case 2://µÚÒ»½×¶Î 6 ~ 9Ğ¡Ê± 
			{
				if(Current_Time<30)//30Ãë2.5V
				{
					PWM0_set (0,CurrentBase[SYS_STEP]);
					
				}
				else if(Current_Time<35)
					{
						PWM0_set (0,0x33);//5Ãë1V
					}
					else
					{
						Current_Time = 0;
					}
				PWM0_set (1,VoltageBase[SYS_STEP]);
				if(Change_Num==0)//±ÜÃâºÍÓÃ»§µ÷½ÚÊÇµÄÖ¸Ê¾µÆ»¥Ïà¸ÉÈÅ
				{
					Led1 = 0;
					Led2 = 1;
					Led3 = 0;
				}
				if(SYS_Time.GHour==9)SYS_STEP++;
				break;
			}			
			case 3://µÚÒ»½×¶Î 9 ~ 12Ğ¡Ê± 
			{
				if(Current_Time<30)//30Ãë2.5V
				{
					PWM0_set (0,CurrentBase[SYS_STEP]);
					
				}
				else if(Current_Time<35)
					{
						PWM0_set (0,0x19);//5Ãë0.5V
					}
					else
					{
						Current_Time = 0;
					}
				PWM0_set (1,VoltageBase[SYS_STEP]);
				if(Change_Num==0)//±ÜÃâºÍÓÃ»§µ÷½ÚÊÇµÄÖ¸Ê¾µÆ»¥Ïà¸ÉÈÅ
				{
					Led1 = 0;
					Led2 = 0;
					Led3 = 1;
				}
				if(SYS_Time.GHour==12)SYS_STEP++;// Ê±¼äµ½×ªµ½ÏÂÒ»²½	
				break;
			}
			case 4://µÚÒ»½×¶Î 12 ~ 14Ğ¡Ê± 
			{
				PWM0_set (0,CurrentBase[SYS_STEP]);
				PWM0_set (1,VoltageBase[SYS_STEP]);
				if(Change_Num==0)//±ÜÃâºÍÓÃ»§µ÷½ÚÊÇµÄÖ¸Ê¾µÆ»¥Ïà¸ÉÈÅ
				{
					Led1 = 0;
					Led2 = 0;
					Led3 = 0;
				}
				if(SYS_Time.GHour==14)SYS_STEP++;// Ê±¼äµ½×ªµ½ÏÂÒ»²½	
				break;
			}
			case 5://³äÂú
			{
				PWM0_set (0,CurrentBase[0]);
				PWM0_set (1,VoltageBase[0]);
				if(Change_Num==0)//±ÜÃâºÍÓÃ»§µ÷½ÚÊÇµÄÖ¸Ê¾µÆ»¥Ïà¸ÉÈÅ
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
	TL0 = T0MS;                     //³õÊ¼»¯¼ÆÊ±Öµ
    TH0 = T0MS >> 8;
	Time_Base++;
	if(Time_Base>=100)
	{
		Time_Base = 0;
		Current_Time++;//ÓÃÓÚÊä³ö¿Ú1 30ÃëºÍ5ÃëÖØ¸´ÓÃ
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
	if(Wait_Flag)time_wait++;//Ö´ĞĞÖÜÆÚÎª10ms	
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
			IapEraseSector(0x0000);//²Á³öµÚÒ»¸öÉÈÇø
			IapEraseSector(0x0200);//²Á³öµÚ¶ş¸öÉÈÇø
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
