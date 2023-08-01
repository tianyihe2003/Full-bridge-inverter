#ifndef __ADC_H__
#define __ADC_H__

#include "sys.h" 
#define N 15 //每通道采15次
#define M 4//6通道
#define ADC1_DR_ADDRESS    ((u32)ADC1+0x4c)
void  Adc1_Init(void);//adc初始化函数

int ADC_Value(u8 Channel);	
int Adfilter(u8 Channel); 
uint16_t ADC_Filter(u8 Channel);		//中位值平均滤波法（又称防脉冲干扰平均滤波法）
int Rms_voltage( int *arr,int data_len );
int First_order_filter(u8 Channel,u8 coefficient);   //一阶低通滤波器


#endif
