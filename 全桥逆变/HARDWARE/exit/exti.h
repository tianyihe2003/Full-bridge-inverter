#ifndef __EXIT_H	
#define __EXIT_H	 
#include "sys.h"
extern  float Kp,Ki,Kd;

void EXTIX_Init(void);           //外部中断初始化	
void EXTI4_IRQHandler(void);     //外部中断服务程序
void EXTI9_5_IRQHandler(void);   //外部中断服务程序
void EXTI15_10_IRQHandler(void); //外部中断服务程序

#endif



