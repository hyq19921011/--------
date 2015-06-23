#include "key.h"
#include "usart.h"
#include "delay.h"
void Key_Init()
{
	    P1M0 &= 0xcf;//��ʼ��P14�� P15Ϊ˫���
		P1M1 &= 0xcf;
		
}

/***************************
�������ƣ�Key_Scan()
���������NULL
���ز���������ɨ��ֵ
����˵����ɨ�谴����ֵ�����ء�
****************************/
unsigned char Key_Scan()
{
 if(Key_Up == 0) //��
 {
		return 1;
    
 }
 if(Key_Down == 0) //��
 {
		return 2;   
 }
 
 if(Key_Choose == 0)  //ѡ��
 {   	
		return 3;
  
 }
 if(Key_Debug == 0)  //ѡ��
 {   	
		return 4;
  
 }
 return 0;    //û�м����� ��������ֵû��������
}

BYTE read_key(u8* pKeyValue)               
{  
    static u8  s_u8keyState=0;        //δ������ͨ�̰�������������  
    static u16 s_u16keyTimeCounts=0;  //δ�ڼ�ʱ״̬�ļ�����
    static u8  s_u8LastKey = key_up ; //��ʷ����
  
    u8 keyTemp=0;               //����Ӧio�ĵ�ƽ  
    u8 key_return=0;            //��������ֵ 
    keyTemp=Key_Scan();  //��ȡ����key��Ӧ��io��  
	Delay_nms(1);
    switch(s_u8keyState)           //�����⵽������ǰ��״̬�����޶��� 
    {  
        case state_keyUp:   //�����ǰ�ǳ�ʼ̬  
        {  
            if(keyTemp!=key_up) //�����������
            {  
                s_u8keyState=state_keyDown; //���¼���״̬����ͨ������  
            }  
        }  
        break;  
          
        case state_keyDown: //�����ǰ�Ǳ����ŵ�  
        {  
            if(key_up!=keyTemp) //������ڻ��Ǳ�����  
            {  
                s_u8keyState=state_keyTime; //ת������ʱ̬  
                s_u16keyTimeCounts=0;  
                s_u8LastKey = keyTemp;     //�����ֵ  
            }  
            else  
            {  
                s_u8keyState=state_keyUp; //��û�����ţ��ص���ʼ̬��˵���Ǹ���  
            }  
        }  
        break;  
          
        case state_keyTime:  //�����ǰ�Ѿ�ת������ʱ̬  
        {  //��������ֶ���������Ȼ���뵽������飬���һ��ν���  
            if(key_up==keyTemp) //���δ���� 
            {  
                s_u8keyState=state_keyUp;   
                key_return=return_keyPressed;    //����2��һ����������ͨ���� 
                //�������������飬˵���Ѿ���2������10MS���жϣ������Ѿ����� 
                //��ô��ʱ��⵽�������ͷţ�˵����һ����ͨ�Ķ̰�  
            }  
            else  //�ڼ�ʱ״̬����⵽����������  
            {  
                if(++s_u16keyTimeCounts>key_longTimes) //ʱ��ﵽ2��  
                {  
                    s_u8keyState=state_keyLong;  //���������״̬  
                    s_u16keyTimeCounts=0;        //���������㣬���ڽ����������¼���  
                    key_return=return_keyLong;   //����state_keyLong  
                }  
           
            }  
        }  
        break;  
          
        case state_keyLong:  //�ڳ���״̬������� ,ÿ0.2�뷢һ�� 
        {  
            if(key_up==keyTemp)   
            {  
               s_u8keyState=state_keyUp;   
            }  
            else //����ʱ�䳬��2��ʱ 
            {  
                if(++s_u16keyTimeCounts>key_autoTimes)//10*key_autoTimes
                {  
                    s_u16keyTimeCounts=0;  
                    key_return=return_keyAuto;  //ÿ0.2�뷵��ֵ�ĵڶ�λ��λ  
                }//������ʱ������ų���  
            }  
            key_return |= return_keyLong;  //0x02????,0x04|0x02????  
        }  
        break;  
          
        default:  
        break;  
    }  
    *pKeyValue = s_u8LastKey ; //���ؼ�ֵ 
    return key_return;  
}