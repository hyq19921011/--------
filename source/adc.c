
#include "adc.h"

void AD_delay(unsigned int n)       //N us延时函数
{
   unsigned int i=0;
   for (i=0;i<n;i++)
	{
		_nop_();
	}
}


void AD_init()//10位
{
	P1ASF|=0x20;       //将P1^5口作为ADC功能使用
	AUXR1|=0x04;      //0000,0100 令 ADRJ=1，10位A/D转换结果的高2位存放在ADC_RES的低2位中，低8位存放在ADC_RESL中
	ADC_CONTR = 0xE0; //1110,0000 令 ADC_POWER=1，将ADC电源打开， SPEED1，SPEED0：模数转换器转换速度控制位
	
	ADC_RES=0;     //高八位数据清零,STC12C5A60S2 AD数据寄存名与STC12C54××系列不同
	ADC_RESL=0;    //低两位清零
	AD_delay(2);      //使输入电压达到稳定
	Get_ad_result0(5);
	Get_ad_result0(5);
	Get_ad_result0(5);
}

WORD Get_ad_result(BYTE channel)
{
	BYTE i;
	WORD ad_result=0,total=0;
	for(i=0;i<ADC_TIMES;i++)
	{
		ADC_CONTR |= ADC_POWER | ADC_SPEEDLL | channel | ADC_START;   //0000,1xxx 令 ADC_START=1, 启动A/D 转换 ;选择模拟信号输入IO通道
		AD_delay(8);  //使输入电压达到稳定
		while( !(ADC_FLAG & ADC_CONTR) );  //0001,0000, ADC_FLAG ==1测试A/D转换结束否
		ADC_CONTR &= ~ADC_FLAG;  //1110,0111 令 ADC_FLAG=0,清楚AD转换结束标志;令 ADC_START=0, 关闭A/D 转换,
		ad_result=ADC_RES<<2|ADC_RESL;  //高两位与低位数据 结合  ( 0~1023)
		total += ad_result;
	}
	total=total>>3;
	return total;  // 返回 A/D 转换结果
}
WORD Get_ad_result0(BYTE channel)
{
	u16 ad_result = 0;
	
		ADC_CONTR |= ADC_POWER | ADC_SPEEDLL | channel | ADC_START;   //0000,1xxx 令 ADC_START=1, 启动A/D 转换 ;选择模拟信号输入IO通道
		AD_delay(4);  //使输入电压达到稳定
		while( !(ADC_FLAG & ADC_CONTR) );  //0001,0000, ADC_FLAG ==1测试A/D转换结束否
		ADC_CONTR &= ~ADC_FLAG;  //1110,0111 令 ADC_FLAG=0,清楚AD转换结束标志;令 ADC_START=0, 关闭A/D 转换,
		ad_result=ADC_RES<<2|ADC_RESL;  //高两位与低位数据 结合  ( 0~1023)

	return ad_result;  // 返回 A/D 转换结果
}
