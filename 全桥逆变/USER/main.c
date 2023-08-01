//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : HuangKai
//  生成日期   : 2018-0101
//  最近修改   : 
//  功能描述   :演示例程(STM32F4系列)
//              说明: 
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接5V或3.3v电源
//              SCL   PG12（SCLK）
//              SDA   PD5（MOSI）
//              RES   PD4
//              DC    PD15
//              CS    PD1
//              BLK   PE8
//              ----------------------------------------------------------------
// 修改历史   :
// 日    期   : 
// 作    者   : HuangKai
// 修改内容   : 创建文件
//版权所有，盗版必究。
//  生成日期   : 2018-0101
//All rights reserved
//******************************************************************************/


//**将 PE8 接low
//**将 PE9 接high 
//**
//**
//**
////
#include "delay.h"

#include "led.h"
#include "lcd_init.h"
#include "lcd.h"
#include "pic.h"
#include "adc.h"
#include "usart.h"
#include "pwm.h"
#include "key.h"
#include "time.h"
#include "pid.h"
#include "main.h"

#define __FPU_PRESENT 1
int   AC_MAX;
float DC_v0ltmeter;
float DC_I0ltmeter;
double Sine12bit[400]={0};        //储存生成的正弦波表
extern vu16 AD_Value[N][M]; //用来存放ADC的结果 也是DMA的目标地址
int k;

int main(void)
{ 
	SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));
	int period0 =(int)period;
	//int t=0;
	//int period = ( (int)( 168000000/(frequence)/(psc+1 )-1) )
	delay_init(168);
//	LED_Init();//LED初始化
//	LCD_Init();//LCD初始化	
//	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);

	//KEY_Init();
	LED2_INIT();
	
	if(period>65535)
	{GPIO_ResetBits(GPIOB,GPIO_Pin_0);}
	
	Adc1_Init();
//	SineWave_Data(400,Sine12bit,1);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	uart_init(115200);	//串口初始化波特率为115200
	printf("串口正常\n");
	TIM1_PWM_Init(period0, psc0);  
	TIM2_Int_Init(period0,psc0); 
SineWave_Data(400,Sine12bit,1);
//extern vu16 AD_Value[N][M]	;
float v_ac_in[8]={0};

//float i_ac_in[3]={0};
	
	
//	double Sine12bit[400];
//	int   AC_MAX;
//	float DC_v0ltmeter;
//	float DC_I0ltmeter;
	while(1) 
	{
		
			#if 1
					 k=(First_order_filter(0,30)+First_order_filter(1,30) )*0.5;
		#else  
			 k=(ADC_Filter(0)+ADC_Filter(0))*0.5;
		#endif
			 //电压
//	 v_ac_in[0]=AD_Value[0][0];
//	 v_ac_in[1]=AD_Value[1][0];
//	 v_ac_in[2]=AD_Value[2][0];
//		v_ac_in[2]=AD_Value[4][0];
//		
//		for(int i=0;i<14;i++)
//		{printf("这是v%d ,  值为%d\n",i,AD_Value[i][0]);}
//		delay_ms(1000);
//		
//	v_ac_in[0]=First_order_filter(10,30)/4096.0*3.3;
//	v_ac_in[2]=First_order_filter(11,30)/4096.0*3.3;

	 //电流
	
		//delay_ms(1000);
//	 i_ac_in[0]=(float)((AD_Value[3]-2280));
//	 i_ac_in[1]=(float)((AD_Value[4]-2280));		
//	 i_ac_in[2]=(float)((AD_Value[5]-2280));
	
//		LCD_ShowChinese(0,0,"中景园电子",RED,WHITE,24,0);
//		LCD_ShowString(24,30,"LCD_W:",RED,WHITE,16,0);
//		LCD_ShowIntNum(72,30,LCD_W,3,RED,WHITE,16);
//		LCD_ShowString(24,50,"LCD_H:",RED,WHITE,16,0);
//		LCD_ShowIntNum(72,50,LCD_H,3,RED,WHITE,16);
		
		//LCD_ShowFloatNum1(20,80,t,4,RED,WHITE,16);
		//k=Get_Adc_Average(10,3);
		//LCD_ShowFloatNum1(20,100,v_ac_in[0],4,RED,WHITE,16);
		
	//	LCD_ShowFloatNum1(20,100,v_ac_in[0],4,RED,WHITE,16);
	//	LCD_ShowFloatNum3(20,80,v_ac_in[1],4,RED,WHITE,16);


//			LCD_ShowIntNum(20,60,v_ac_in[0],4,RED,WHITE,16);
//			LCD_ShowIntNum(20,80,v_ac_in[1],4,RED,WHITE,16);

//			LCD_ShowIntNum(20,100,v_ac_in[4],4,RED,WHITE,16);



		

//		LCD_ShowFloatNum1(20,100,t,3,RED,WHITE,16);
//		LCD_ShowFloatNum3(20,80,t,4,RED,WHITE,16);
		
//		LCD_ShowIntNum(20,80,(int)v_ac_in[0],4,RED,WHITE,16);
//		LCD_ShowIntNum(20,100,(int)v_ac_in[2],4,RED,WHITE,16);
		
//		LCD_ShowFloatNum1(20,100,v_ac_in[0],4,RED,WHITE,16);
//		LCD_ShowFloatNum1(20,80,v_ac_in[2],4,RED,WHITE,16);
		
		//LCD_ShowFloatNum1(20,80,Sine12bit[t],4,RED,WHITE,16);

//		
//		t+=1;
//		t%=400;
//		//LCD_ShowPicture(65,80,40,40,gImage_1);
//		printf("hello world\n");
//		printf("%f\n",v_ac_in[0]);
//		printf("%d\n",AD_Value[0][0]);
//		
//		
//		
//		delay_ms(1000);
	}
}
