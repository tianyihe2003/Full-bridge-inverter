#include  "exti.h"
#include "key.h"
#include "delay.h"
#include "pid.h"
#include "lcd.h"
#include "io.h"
#include "time.h"
float Kp=50,Ki=5,Kd=0;

#if 0
void EXTIX_Init(void)
{
 
	
   	EXTI_InitTypeDef EXTI_InitStructure;
 	  NVIC_InitTypeDef NVIC_InitStructure;

    KEY_Init();	 //	按键端口初始化

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟

    //GPIOC.4 中断线以及中断初始化配置   下降沿触发
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource4);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line4;	//KEY2
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

   //GPIOC.5	  中断线以及中断初始化配置 下降沿触发 //KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource5);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

   //GPIOC.9  中断线以及中断初始化配置  下降沿触发	//KEY0
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource9);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line9;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


   //GPIOC.10	  中断线以及中断初始化配置 上升沿触发 PA0  WK_UP
 	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource10); 
    EXTI_InitStructure.EXTI_Line=EXTI_Line10;
  	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

    //GPIOC.11	  中断线以及中断初始化配置 上升沿触发 PA0  WK_UP
 	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource11); 
    EXTI_InitStructure.EXTI_Line=EXTI_Line11;
  	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
		
		 //GPIOC.12	  中断线以及中断初始化配置 上升沿触发 PA0  WK_UP
 	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource12); 
    EXTI_InitStructure.EXTI_Line=EXTI_Line12;
  	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
    
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//使能按键WK_UP所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 

    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键KEY2所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//子优先级2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//使能按键KEY1所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级1 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
 
 
}
#else
void EXTIX_Init(void)
{
	//第一步：初始化GPIO
	KEY_Init();	 //	按键端口初始化
	
	//第二步：初始化EXTI
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	//将中断源和KEY引脚连接
	//使用PE2、3、4、5引脚作为key
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);//PA0 连接到中断线0--对应K1按键
	
	
	EXTI_InitTypeDef   EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;//LINE0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发   
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
  EXTI_Init(&EXTI_InitStructure);//配置
	
	//第三步：配置NVIC
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//外部中断0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
	
}

#endif

//外部中断4服务程序 
void EXTI4_IRQHandler(void)     //PC4
{
	delay_ms(40);
	if(EXTI_GetITStatus(EXTI_Line4)!=RESET)
		{
			
     Kp = Kp + 0.5f;
		// LCD_ShowFloatNum3(18,12, Kp,5,3,12);
			LCD_ShowFloatNum3(20,80,Kp,4,RED,WHITE,16);
		}
	EXTI_ClearITPendingBit(EXTI_Line4);  //清除LINE8上的中断标志位  
} 

//外部中断9_5服务程序 
void EXTI9_5_IRQHandler(void)
{
	delay_ms(20);
	if(EXTI_GetITStatus(EXTI_Line5)!=RESET)
		{
		
	   Kp = Kp - 0.5f;
		LCD_ShowFloatNum3(20,80,Kp,4,RED,WHITE,16);
		}
	if(EXTI_GetITStatus(EXTI_Line9)!=RESET)
		{
			
	   Ki = Ki + 0.5f;
		LCD_ShowFloatNum3(20,80,Ki,4,RED,WHITE,16);
			
		}
	EXTI_ClearITPendingBit(EXTI_Line5);  //清除LINE8上的中断标志位  
	EXTI_ClearITPendingBit(EXTI_Line9);  //清除LINE8上的中断标志位  	
} 

//外部中断1服务程序 
void EXTI15_10_IRQHandler(void)
{
	delay_ms(20);
	if(EXTI_GetITStatus(EXTI_Line10)!=RESET)
		{
	   Ki = Ki - 0.5f;
		LCD_ShowFloatNum3(20,80,Ki,4,RED,WHITE,16);			
		}
	if(EXTI_GetITStatus(EXTI_Line11)!=RESET)
		{
		 Kd = Kd +0.5f;
		LCD_ShowFloatNum3(20,80,Kd,4,RED,WHITE,16);			
		}
	if(EXTI_GetITStatus(EXTI_Line12)!=RESET)
		{
	
		 Kd = Kd - 0.5f;
		LCD_ShowFloatNum3(20,80,Kd,4,RED,WHITE,16);		}
	EXTI_ClearITPendingBit(EXTI_Line10);  //清除LINE8上的中断标志位  
	EXTI_ClearITPendingBit(EXTI_Line11);  //清除LINE8上的中断标志位  
	EXTI_ClearITPendingBit(EXTI_Line12);  //清除LINE8上的中断标志位  
} 






