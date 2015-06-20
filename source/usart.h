#ifndef _usart_h_
#define _usart_h_

extern void InitUart();
extern BYTE SendData(BYTE dat);
extern void SendString(char *s);

#endif