#include <STC15F2K60S2.H>

/*----------------------------
��ʼ������
----------------------------*/
void InitUart()
{
    SCON = 0x5a;                //���ô���Ϊ8λ�ɱ䲨����
    T2L = (65536-11059200/4/115200);                 //���ò�������װֵ
    T2H = (65536-11059200/4/115200)>> 8;                 //115200 bps(65536-11432000/4/115200)
    AUXR = 0x14;                //T2Ϊ1Tģʽ, ��������ʱ��2
    AUXR |= 0x01;               //ѡ��ʱ��2Ϊ����1�Ĳ����ʷ�����
    ES = 1;                   //ʹ�ܴ���1�ж�
}

/*----------------------------
���ʹ�������
----------------------------*/
BYTE SendData(BYTE dat)
{
    while (!TI);                    //�ȴ�ǰһ�����ݷ������
    TI = 0;                         //������ͱ�־
    SBUF = dat;                     //���͵�ǰ����
    
    return dat;
}
void SendString(char *s)
{
    while (*s)              //Check the end of the string
    {
        SendData(*s++);     //Send current char and increment string ptr
    }
}