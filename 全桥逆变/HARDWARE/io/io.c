#include "io.h"


//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
void PC0_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	 //使能PB端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //LED0-->PB0 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.8
 
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);						 //PB输出低

}
 
