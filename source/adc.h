#ifndef _adc_h_
#define _adc_h_
#include <intrins.h> 
#include <STC15F2K60S2.H>
#define  ADC_TIMES  8     //ADC转换次数 0~64（ 返回平均值 ）
#define ADC_POWER   0x80            //ADC电源控制位
#define ADC_FLAG    0x10            //ADC完成标志
#define ADC_START   0x08            //ADC起始控制位
#define ADC_SPEEDLL 0x00            //540个时钟
#define ADC_SPEEDL  0x20            //360个时钟
#define ADC_SPEEDH  0x40            //180个时钟
#define ADC_SPEEDHH 0x60            //90个时钟
extern void	AD_init(void);      //ADC初始化  
extern WORD Get_ad_result(BYTE channel);   //ADC数据采集函数（channel: 0~7 ） P1口全部
extern WORD GetADCResult(BYTE ch);
WORD Get_ad_result0(BYTE channel);
#endif