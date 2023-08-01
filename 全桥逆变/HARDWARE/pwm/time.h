#ifndef _TIME_H_
#define _TIME_H_
#include "sys.h"


extern int   AC_MAX;
extern float DC_v0ltmeter;  //BUCK 
extern float DC_I0ltmeter;  //BUCK
extern double Sine12bit[400];        //储存生成的正弦波表

void TIM2_Int_Init(u16 arr,u16 PSC);
void TIM3_Int_Init(u16 arr,u16 PSC);


#endif

