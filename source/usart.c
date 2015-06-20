#include <STC15F2K60S2.H>

/*----------------------------
初始化串口
----------------------------*/
void InitUart()
{
    SCON = 0x5a;                //设置串口为8位可变波特率
    T2L = (65536-11059200/4/115200);                 //设置波特率重装值
    T2H = (65536-11059200/4/115200)>> 8;                 //115200 bps(65536-11432000/4/115200)
    AUXR = 0x14;                //T2为1T模式, 并启动定时器2
    AUXR |= 0x01;               //选择定时器2为串口1的波特率发生器
    ES = 1;                   //使能串口1中断
}

/*----------------------------
发送串口数据
----------------------------*/
BYTE SendData(BYTE dat)
{
    while (!TI);                    //等待前一个数据发送完成
    TI = 0;                         //清除发送标志
    SBUF = dat;                     //发送当前数据
    
    return dat;
}
void SendString(char *s)
{
    while (*s)              //Check the end of the string
    {
        SendData(*s++);     //Send current char and increment string ptr
    }
}