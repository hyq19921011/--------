
#include "adc.h"

void AD_delay(unsigned int n)       //N us��ʱ����
{
   unsigned int i=0;
   for (i=0;i<n;i++)
	{
		_nop_();
	}
}


void AD_init()//10λ
{
<<<<<<< HEAD
	P1ASF|=0x0c;       //��P1^2�ں�1^3��ΪADC����ʹ��
	CLK_DIV&=~0x20;      //0000,0100 �� ADRJ=1��10λA/Dת������ĸ�2λ�����ADC_RES�ĵ�2λ�У���8λ�����ADC_RESL��
	ADC_CONTR = ADC_POWER | ADC_SPEEDH; //1110,0000 �� ADC_POWER=1����ADC��Դ�򿪣� SPEED1��SPEED0��ģ��ת����ת���ٶȿ���λ
=======
	P1ASF|=0x20;       //��P1^5����ΪADC����ʹ��
	AUXR1|=0x04;      //0000,0100 �� ADRJ=1��10λA/Dת������ĸ�2λ�����ADC_RES�ĵ�2λ�У���8λ�����ADC_RESL��
	ADC_CONTR = 0xE0; //1110,0000 �� ADC_POWER=1����ADC��Դ�򿪣� SPEED1��SPEED0��ģ��ת����ת���ٶȿ���λ
>>>>>>> parent of 838f16f... 说明：修改至最新，按键有大的改动，增加信号检测端口。
	
	ADC_RES=0;     //�߰�λ��������,STC12C5A60S2 AD���ݼĴ�����STC12C54����ϵ�в�ͬ
	ADC_RESL=0;    //����λ����
	AD_delay(2);      //ʹ�����ѹ�ﵽ�ȶ�
}

WORD Get_ad_result(BYTE channel)
{
	BYTE i;
	WORD ad_result=0,total=0;
	AD_init();
	for(i=0;i<ADC_TIMES;i++)
	{
		ADC_CONTR |= ADC_POWER | ADC_SPEEDH | channel | ADC_START;   //0000,1xxx �� ADC_START=1, ����A/D ת�� ;ѡ��ģ���ź�����IOͨ��
		AD_delay(8);  //ʹ�����ѹ�ﵽ�ȶ�
		while( !(ADC_FLAG & ADC_CONTR) );  //0001,0000, ADC_FLAG ==1����A/Dת��������
		ADC_CONTR &= ~ADC_FLAG;  //1110,0111 �� ADC_FLAG=0,���ADת��������־;�� ADC_START=0, �ر�A/D ת��,
		ad_result=ADC_RES<<2|ADC_RESL;  //����λ���λ���� ���  ( 0~1023)
		total += ad_result;
	}
	total=total>>3;
	return total;  // ���� A/D ת�����
}
WORD Get_ad_result0(BYTE channel)
{
	u16 ad_result = 0;
	
		ADC_CONTR |= ADC_POWER | ADC_SPEEDH | channel | ADC_START;   //0000,1xxx �� ADC_START=1, ����A/D ת�� ;ѡ��ģ���ź�����IOͨ��
		AD_delay(4);  //ʹ�����ѹ�ﵽ�ȶ�
		while( !(ADC_FLAG & ADC_CONTR) );  //0001,0000, ADC_FLAG ==1����A/Dת��������
		ADC_CONTR &= ~ADC_FLAG;  //1110,0111 �� ADC_FLAG=0,���ADת��������־;�� ADC_START=0, �ر�A/D ת��,
		ad_result=ADC_RES<<2|ADC_RESL;  //����λ���λ���� ���  ( 0~1023)

	return ad_result;  // ���� A/D ת�����
}
