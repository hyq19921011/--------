/*******
占用端口P10、P11
*******/

#include "pwm_dac.h"
	/*
	#define PCACLK0     0x00        //Fosc/12
	#define PCACLK1     0x02        //Fosc/2
	#define PCACLK2     0x04        //???0????
	#define PCACLK3     0x06        //ECI????
	#define PCACLK4     0x08        //Fosc/1
	#define PCACLK5     0x0a        //Fosc/4
	#define PCACLK6     0x0c        //Fosc/6
	#define PCACLK7     0x0e        //Fosc/8
	*/
void PWMn_init()//PWM初始化
{
	 P1M1 &= ~0x03,  P1M0 |=  0x03; 
	CCON = 0;                       //Initial PCA control register
                                    //PCA timer stop running
                                    //Clear CF flag
                                    //Clear all module interrupt flag
    CL = 0;                         //Reset PCA base timer
    CH = 0;
    CMOD=0x02; //Fosc/2
	CCAPM0=0x42; //PWM0??PCA?????PWM??(0100 0010)
	CCAPM1=0x42; //PWM1??PCA?????PWM??(0100 0010)
	CR=1; //??PCA???
}

void PWM0_set (BYTE chanel,BYTE zkb)
{

	if(chanel==0)
	{		
 		CCAP0H=CCAP0L=255-zkb;//2.55*zkb; 

	}
	if(chanel==1)
	{	 	
		CCAP1H=CCAP1L=255-zkb;//2.55*zkb; 
 		 
	}
	
}

void Set_Speed(int pwm)//pwm??? ??0 ??;??0??; 
{
	if(pwm>=0)
	{
		PWM0_set(0,0);//??
		PWM0_set(1,pwm);
	}
	if(pwm<=0)
	{
		PWM0_set(1,0);//??
		PWM0_set(0,-pwm);
		
	}
}