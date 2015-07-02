
#include "adc.h"

void AD_delay(unsigned int n)       //N usÑÓÊ±º¯Êı
{
   unsigned int i=0;
   for (i=0;i<n;i++)
	{
		_nop_();
	}
}


void AD_init()//10Î»
{
<<<<<<< HEAD
	P1ASF|=0x0c;       //½«P1^2¿ÚºÍ1^3×÷ÎªADC¹¦ÄÜÊ¹ÓÃ
	CLK_DIV&=~0x20;      //0000,0100 Áî ADRJ=1£¬10Î»A/D×ª»»½á¹ûµÄ¸ß2Î»´æ·ÅÔÚADC_RESµÄµÍ2Î»ÖĞ£¬µÍ8Î»´æ·ÅÔÚADC_RESLÖĞ
	ADC_CONTR = ADC_POWER | ADC_SPEEDH; //1110,0000 Áî ADC_POWER=1£¬½«ADCµçÔ´´ò¿ª£¬ SPEED1£¬SPEED0£ºÄ£Êı×ª»»Æ÷×ª»»ËÙ¶È¿ØÖÆÎ»
=======
	P1ASF|=0x20;       //½«P1^5¿Ú×÷ÎªADC¹¦ÄÜÊ¹ÓÃ
	AUXR1|=0x04;      //0000,0100 Áî ADRJ=1£¬10Î»A/D×ª»»½á¹ûµÄ¸ß2Î»´æ·ÅÔÚADC_RESµÄµÍ2Î»ÖĞ£¬µÍ8Î»´æ·ÅÔÚADC_RESLÖĞ
	ADC_CONTR = 0xE0; //1110,0000 Áî ADC_POWER=1£¬½«ADCµçÔ´´ò¿ª£¬ SPEED1£¬SPEED0£ºÄ£Êı×ª»»Æ÷×ª»»ËÙ¶È¿ØÖÆÎ»
>>>>>>> parent of 838f16f... è¯´æ˜ï¼šä¿®æ”¹è‡³æœ€æ–°ï¼ŒæŒ‰é”®æœ‰å¤§çš„æ”¹åŠ¨ï¼Œå¢åŠ ä¿¡å·æ£€æµ‹ç«¯å£ã€‚
	
	ADC_RES=0;     //¸ß°ËÎ»Êı¾İÇåÁã,STC12C5A60S2 ADÊı¾İ¼Ä´æÃûÓëSTC12C54¡Á¡ÁÏµÁĞ²»Í¬
	ADC_RESL=0;    //µÍÁ½Î»ÇåÁã
	AD_delay(2);      //Ê¹ÊäÈëµçÑ¹´ïµ½ÎÈ¶¨
}

WORD Get_ad_result(BYTE channel)
{
	BYTE i;
	WORD ad_result=0,total=0;
	AD_init();
	for(i=0;i<ADC_TIMES;i++)
	{
		ADC_CONTR |= ADC_POWER | ADC_SPEEDH | channel | ADC_START;   //0000,1xxx Áî ADC_START=1, Æô¶¯A/D ×ª»» ;Ñ¡ÔñÄ£ÄâĞÅºÅÊäÈëIOÍ¨µÀ
		AD_delay(8);  //Ê¹ÊäÈëµçÑ¹´ïµ½ÎÈ¶¨
		while( !(ADC_FLAG & ADC_CONTR) );  //0001,0000, ADC_FLAG ==1²âÊÔA/D×ª»»½áÊø·ñ
		ADC_CONTR &= ~ADC_FLAG;  //1110,0111 Áî ADC_FLAG=0,Çå³şAD×ª»»½áÊø±êÖ¾;Áî ADC_START=0, ¹Ø±ÕA/D ×ª»»,
		ad_result=ADC_RES<<2|ADC_RESL;  //¸ßÁ½Î»ÓëµÍÎ»Êı¾İ ½áºÏ  ( 0~1023)
		total += ad_result;
	}
	total=total>>3;
	return total;  // ·µ»Ø A/D ×ª»»½á¹û
}
WORD Get_ad_result0(BYTE channel)
{
	u16 ad_result = 0;
	
		ADC_CONTR |= ADC_POWER | ADC_SPEEDH | channel | ADC_START;   //0000,1xxx Áî ADC_START=1, Æô¶¯A/D ×ª»» ;Ñ¡ÔñÄ£ÄâĞÅºÅÊäÈëIOÍ¨µÀ
		AD_delay(4);  //Ê¹ÊäÈëµçÑ¹´ïµ½ÎÈ¶¨
		while( !(ADC_FLAG & ADC_CONTR) );  //0001,0000, ADC_FLAG ==1²âÊÔA/D×ª»»½áÊø·ñ
		ADC_CONTR &= ~ADC_FLAG;  //1110,0111 Áî ADC_FLAG=0,Çå³şAD×ª»»½áÊø±êÖ¾;Áî ADC_START=0, ¹Ø±ÕA/D ×ª»»,
		ad_result=ADC_RES<<2|ADC_RESL;  //¸ßÁ½Î»ÓëµÍÎ»Êı¾İ ½áºÏ  ( 0~1023)

	return ad_result;  // ·µ»Ø A/D ×ª»»½á¹û
}
