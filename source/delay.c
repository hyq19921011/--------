#include "delay.h"
#include "main.h"
void Delay_nms(unsigned int count)
{
	unsigned int j = 0;
	for(count;count>0;count--)
		for(j = 56;j>0;j--);
}
//========================================================================
// ??: void  delay_ms(unsigned char ms)
// ??: ?????
// ??: ms,????ms?, ?????1~255ms. ???????.
// ??: none.
// ??: VER1.0
// ??: 2013-4-1
// ??: 
//========================================================================
void  delay_ms(unsigned int ms)
{
     unsigned int i;
	 do{
	      i = MAIN_Fosc / 13000;
		  while(--i)	;
     }while(--ms);
}