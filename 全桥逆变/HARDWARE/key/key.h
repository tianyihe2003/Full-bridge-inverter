#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

#define KEY1 PCin(4)    //PC4
#define KEY2 PCin(5)	  //PC5
#define KEY3 PCin(9)	  //PC9
#define KEY4 PCin(10)   //PC10
#define KEY5 PCin(11)   //PC11
#define KEY6 PCin(12)	  //PC12
 





void KEY_Init(void);//IO初始化
u8 KEY_Scan_1(u8);   //按键扫描函数  
u8 KEY_Scan_2(u8);   //按键扫描函数  		
u8 KEY_Scan_3(u8);   //按键扫描函数 
u8 KEY_Scan_4(u8);   //按键扫描函数 

void LED2_INIT(void );

#endif
