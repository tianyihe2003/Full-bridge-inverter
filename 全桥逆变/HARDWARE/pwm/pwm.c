#include "pwm.h"
#include "math.h"

 /**************************************************

函数功能：配置定时器为pwm互补输出
引脚：PA7复用CH1N  PA8  CHI
输入：arr：自动重装载寄存器的值,psc: 时钟预分频系数
输出：无
只有高级定时器才能互补输出TIM1,TIM8   例 CH1 CH1N
一个通道，互补输出，映射，
要打开TIM1时钟
***************************************************/

void TIM1_PWM_Init(u16 arr,u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
   	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
    //第一步：配置时钟        
    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM1时钟使能    

  //GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1, ENABLE);          //引脚重定义
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_TIM1); //GPIOE8复用为定时器1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_TIM1); //GPIOE8复用为定时器1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource12,GPIO_AF_TIM1); //GPIOE8复用为定时器1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1);    
	
	//第二步，配置goio口	
    /********TIM1_CH1 引脚配置*********/        
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13;           //GPIOE8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	//AF是复用的意思
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_Init(GPIOE,&GPIO_InitStructure);              //初始化PE8
	
    
    //第三步，定时器基本配置        
    TIM_TimeBaseStructure.TIM_Period=arr;             // 自动重装载寄存器的值
    TIM_TimeBaseStructure.TIM_Prescaler=psc;            // 时钟预分频数
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;    // 采样分频
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;//向上计数
    TIM_TimeBaseStructure.TIM_RepetitionCounter=0;//重复寄存器，用于自动更新pwm占空比                       
	  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    
   //第四步pwm输出配置
    TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM2;                    //设置为pwm2输出模式
    TIM_OCInitStructure.TIM_Pulse=(int)(arr*0.5);                                 //设置占空比时间  从1900改为0
    TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;            //设置输出极性
    TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;        //使能该通道输出
    //下面几个参数是高级定时器才会用到，通用定时器不用配置
    TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCPolarity_High;        //设置互补端输出极性
    TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Enable;   //使能互补端输出
    TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;        //死区后输出状态
    TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCNIdleState_Reset;     //死区后互补端输出状态
    TIM_OC1Init(TIM1,&TIM_OCInitStructure);                       //按照指定参数初始化
    
	//第五步，死区和刹车功能配置，高级定时器才有的，通用定时器不用配置
    TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;//运行模式下输出选择 
    TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;//空闲模式下输出选择 
    TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;    //锁定设置
    TIM_BDTRInitStructure.TIM_DeadTime =60;// 0xf;                    //死区时间设置0.1us  //死区时钟为168M Tdtg为65ns  
    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;              //刹车功能使能
    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;//刹车输入极性
    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;//自动输出使能 
    TIM_BDTRConfig(TIM1,&TIM_BDTRInitStructure);        


   //第六步，使能端的打开
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIMx在CCR1上的预装载寄存器
    
		TIM_ARRPreloadConfig(TIM1, ENABLE);                //使能TIMx在ARR上的预装载寄存器
    TIM_Cmd(TIM1,ENABLE);                           //打开TIM1

    //下面这句是高级定时器才有的，输出pwm必须打
    TIM_CtrlPWMOutputs(TIM1, ENABLE);                                   //pwm输出使能，一定要记得打开
}

extern double Sine12bit[400];        //储存生成的正弦波表
void SineWave_Data(u16 cycle ,double *D,float Um)
{
 uint16_t i;
    for( i=0;i<cycle;i++)
    {
		  D[i]=(double)(Um*sin((1.0*i/cycle)*2*PI));   //正弦离散化公式
    }
}

