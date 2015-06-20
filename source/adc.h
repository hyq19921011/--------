#ifndef _adc_h_
#define _adc_h_
#include <intrins.h> 
#include <STC15F2K60S2.H>
#define  ADC_TIMES  8     //ADCת������ 0~64�� ����ƽ��ֵ ��
#define ADC_POWER   0x80            //ADC��Դ����λ
#define ADC_FLAG    0x10            //ADC��ɱ�־
#define ADC_START   0x08            //ADC��ʼ����λ
#define ADC_SPEEDLL 0x00            //540��ʱ��
#define ADC_SPEEDL  0x20            //360��ʱ��
#define ADC_SPEEDH  0x40            //180��ʱ��
#define ADC_SPEEDHH 0x60            //90��ʱ��
extern void	AD_init(void);      //ADC��ʼ��  
extern WORD Get_ad_result(BYTE channel);   //ADC���ݲɼ�������channel: 0~7 �� P1��ȫ��
extern WORD GetADCResult(BYTE ch);
WORD Get_ad_result0(BYTE channel);
#endif