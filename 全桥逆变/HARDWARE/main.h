#ifndef __TIM_H_
#define __TIM_H_

#define frequence (40*1000)
#define psc0        9
#define period  ( (int)( 168000000/(frequence)/(psc0+1 )-1) )
//#define period 654888
#endif





