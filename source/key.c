#include "key.h"
#include "usart.h"
#include "delay.h"
void Key_Init()
{
	    P1M0 &= 0xcf;//初始化P14、 P15为双向口
		P1M1 &= 0xcf;
		
}

/***************************
函数名称：Key_Scan()
输入参数：NULL
返回参数：按键扫描值
功能说明：扫描按键数值并返回。
****************************/
unsigned char Key_Scan()
{
 if(Key_Up == 0) //加
 {
		return 1;
    
 }
 if(Key_Down == 0) //减
 {
		return 2;   
 }
 
 if(Key_Choose == 0)  //选择
 {   	
		return 3;
  
 }
 if(Key_Debug == 0)  //选择
 {   	
		return 4;
  
 }
 return 0;    //没有键按下 随便给的数值没特殊意义
}

BYTE read_key(u8* pKeyValue)               
{  
    static u8  s_u8keyState=0;        //未按，普通短按，长按，连发  
    static u16 s_u16keyTimeCounts=0;  //未在计时状态的计数器
    static u8  s_u8LastKey = key_up ; //历史数据
  
    u8 keyTemp=0;               //键对应io的电平  
    u8 key_return=0;            //函数返回值 
    keyTemp=Key_Scan();  //提取所有key对应的io口  
	Delay_nms(1);
    switch(s_u8keyState)           //这里检测到的是先前的状态，即无动作 
    {  
        case state_keyUp:   //如果先前是初始态  
        {  
            if(keyTemp!=key_up) //如果键被按下
            {  
                s_u8keyState=state_keyDown; //更新键的状态，普通被按下  
            }  
        }  
        break;  
          
        case state_keyDown: //如果先前是被按着的  
        {  
            if(key_up!=keyTemp) //如果现在还是被按着  
            {  
                s_u8keyState=state_keyTime; //转换到计时态  
                s_u16keyTimeCounts=0;  
                s_u8LastKey = keyTemp;     //保存键值  
            }  
            else  
            {  
                s_u8keyState=state_keyUp; //键没被按着，回到初始态，说明是干扰  
            }  
        }  
        break;  
          
        case state_keyTime:  //如果先前已经转换到计时态  
        {  //如果真是手动按键，必然进入到本代码块，并且会多次进入  
            if(key_up==keyTemp) //如果未按键 
            {  
                s_u8keyState=state_keyUp;   
                key_return=return_keyPressed;    //返回2，一次完整的普通按键 
                //程序进入这个语句块，说明已经有2次以上10MS的中断，等于已经消抖 
                //那么此时检测到按键被释放，说明是一次普通的短按  
            }  
            else  //在计时状态，检测到键还被按着  
            {  
                if(++s_u16keyTimeCounts>key_longTimes) //时间达到2秒  
                {  
                    s_u8keyState=state_keyLong;  //进入产按键状态  
                    s_u16keyTimeCounts=0;        //技术器清零，便于进入连发重新计数  
                    key_return=return_keyLong;   //返回state_keyLong  
                }  
           
            }  
        }  
        break;  
          
        case state_keyLong:  //在长安状态检测连发 ,每0.2秒发一次 
        {  
            if(key_up==keyTemp)   
            {  
               s_u8keyState=state_keyUp;   
            }  
            else //按键时间超过2秒时 
            {  
                if(++s_u16keyTimeCounts>key_autoTimes)//10*key_autoTimes
                {  
                    s_u16keyTimeCounts=0;  
                    key_return=return_keyAuto;  //每0.2秒返回值的第二位置位  
                }//连发的时候伴随着长按  
            }  
            key_return |= return_keyLong;  //0x02????,0x04|0x02????  
        }  
        break;  
          
        default:  
        break;  
    }  
    *pKeyValue = s_u8LastKey ; //返回键值 
    return key_return;  
}