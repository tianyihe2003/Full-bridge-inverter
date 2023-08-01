#ifndef __PWM_H
#define __PWM_H
#include "sys.h"
#define PI 3.14159265358979323846


void TIM8_PWM_Init(u16 arr,u16 psc);
void TIM1_PWM_Init(u16 arr,u16 psc);

void SineWave_Data(u16 cycle ,double *D,float Um);
	


#endif
