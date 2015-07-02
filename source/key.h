#ifndef _key_h_
#define _key_h_
#include <STC15F2K60S2.H>
<<<<<<< HEAD
//sbit Key_Up =   P1^2;
//sbit Key_Down=	P1^3;
//sbit Key_Choose=P1^4;
//sbit Key_Debug    =P5^4;//µ÷ÊÔ°´¼ü
//sbit Key_Lock    =P5^5;//Ëø¶¨¶¨¶Ë¿Ú
sbit Key_Up =   P1^4;
sbit Key_Down=	P1^5;
sbit Key_Choose=P5^4;
sbit Key_Debug    =P5^5;//µ÷ÊÔ°´¼ü
//sbit Key_Lock    =P5^5;//Ëø¶¨¶¨¶Ë¿Ú
=======
sbit Key_Up =   P1^2;
sbit Key_Down=	P1^3;
sbit Key_Choose=P1^4;
sbit Key_Debug    =P5^4;//µ÷ÊÔ°´¼ü
sbit Key_Lock    =P5^5;//Ëø¶¨¶¨¶Ë¿Ú
>>>>>>> parent of 838f16f... è¯´æ˜ï¼šä¿®æ”¹è‡³æœ€æ–°ï¼ŒæŒ‰é”®æœ‰å¤§çš„æ”¹åŠ¨ï¼Œå¢åŠ ä¿¡å·æ£€æµ‹ç«¯å£ã€‚
#define UP     1
#define DOWN   2
#define CHOOSE 3
#define DEBUG  4

#define state_keyUp         0       //³õÊ¼×´Ì¬£¬Î´°´¼ü 
#define state_keyDown       1       //¼ü±»°´ÏÂ 
#define state_keyLong       2       //³¤°´  
#define state_keyTime       3       //°´¼ü¼ÆÊ±Ì¬ 
  
#define return_keyUp        0x00    //³õÊ¼×´Ì¬  
#define return_keyPressed   0x01    //¼ü±»°´ÏÂ£¬ÆÕÍ¨°´¼ü  
#define return_keyLong      0x02    //³¤°´  
#define return_keyAuto      0x04    //×Ô¶¯Á¬·¢ 
  
#define key_down             0      //°´ÏÂ  
#define key_up               0    //Î´°´ÏÂÊ±µÄkeyÓĞĞ§Öµ 
#define key_longTimes       150     //10msÒ»´Î,200´Î¼´Á½Ãë,¶¨Òå³¤°´¼üÅĞ¶¨Ê±¼ä 
#define key_autoTimes       100      //Á¬·¢Ê±¼ä¶¨Òå,20*10=200,200ºÁÃë·¢Ò»´Î  

  
extern void Key_Init();
extern BYTE read_key(u8* pKeyValue) ;
#endif