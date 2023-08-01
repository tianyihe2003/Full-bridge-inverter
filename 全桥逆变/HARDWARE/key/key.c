#include "stm32f4xx.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"

								    
//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);//使能PORTA,PORTE时钟

	//PC4,C5,C9,C1O,C11,C12	设置成上拉输入
	GPIO_InitStructure.GPIO_Pin  =GPIO_Pin_2|GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_11| GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //PC4,C5,C9,C1O,C11,C12	设置成上拉输入设置成输入，默认上拉  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化PC4,C5,C9,C1O,C11,C12	设置成上拉输入
  
}

//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;

//注意此函数有响应优先级,KEY0>KEY1>KEY2>KEY3!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0||KEY5==0||KEY6==0))
	{
		delay_ms(5);//去抖动 
		key_up=0;
		if(KEY1==0)return 1;
		else if(KEY2==0)return 2;
		else if(KEY3==0)return 3;
		else if(KEY4==0)return 4;
		else if(KEY5==0)return 5;
		else if(KEY6==0)return 6;
	}else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1&&KEY5==1&&KEY6==1)key_up=1; 	    
 	return 0;// 无按键按下
}

void LED2_INIT(void )
{

 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);//使能PORTA,PORTE时钟

	//PC4,C5,C9,C1O,C11,C12	设置成上拉输入
	GPIO_InitStructure.GPIO_Pin  =GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_OUT ; //PC4,C5,C9,C1O,C11,C12	设置成上拉输入设置成输入，默认上拉  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化PC4,C5,C9,C1O,C11,C12	设置成上拉输入
 #if 1
GPIO_SetBits(GPIOB,GPIO_Pin_0 );	
	#else
	GPIO_ResetBits(GPIOB,GPIO_Pin_0 );	
	#endif
}



