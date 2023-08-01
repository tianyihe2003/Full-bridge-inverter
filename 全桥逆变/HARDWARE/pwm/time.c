#include "time.h"
#include "adc.h"
#include "pid.h"
#include "delay.h"
#include "led.h"
#include "pwm.h"
#include "key.h"
#include "io.h"
#include "usart.h"
#include "main.h"

int Out_Pwm;
extern int k;

void TIM2_Int_Init(u16 arr,u16 PSC)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
  
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =PSC; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
	// ������ʱ�������ж�
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	// ʹ�ܶ�ʱ��
	TIM_Cmd(TIM2, ENABLE);	
							 
}


int v,max=0,min=4096;
void TIM2_IRQHandler(void)   //TIM2�ж�
{  
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{  
		
		
			v=First_order_filter(1,30);
			if(max<v)   max=v;
			if(min>v)  min=v;
		static int T=0;
		
			if(T==0 )
					{											
						AC_MAX=Incremental_PID_PWM(0,max-min,248.242424*4 ,period);//targetΪ�������ֵ��������ֱ�����һ��
						max=0,min=4096;
			#if 0
						printf("Out_Pwm:%d\n",Out_Pwm) 			;
						printf("AC_MAX:%d\n",AC_MAX) 			;
						printf("PERIOD %d\n",(int)(0.5*period) ) 			;
						printf("max  %d  min  %d\n\n\n",max,min);
					
			#endif
					}
	
					
					
					//AC_MAX=(0.5*period);//����ʱʹ��
    			Out_Pwm=(int)((AC_MAX)*Sine12bit[T]+period*0.5);     //+1159A
				  TIM_SetCompare1(TIM1,Out_Pwm);
					
			#if 0
						printf("Out_Pwm:%d\n",Out_Pwm) 			;
						printf("AC_MAX:%d\n",AC_MAX) 			;
						printf("PERIOD %d\n",(int)(0.5*period) ) 			;
						printf("max  %d  min  %d\n\n\n",max,min);
					
			#endif
				T++;
				T%=400;
			//delay_ms(1000);
				
}				 
     TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ   
			
		
}



  
