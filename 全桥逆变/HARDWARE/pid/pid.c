#include "exti.h"
#include "pid.h"
#include "usart.h"



/*************************************************************************
函数功能：增量PID控制器
入口参数:PID序号，测量值，目标值
返回  值：计算结果
根据增量式离散PID公式 
dac+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  以此类推 
Result代表增量输出
Dac+=Kp[e（k）-e(k-1)]+Ki*e(k)
**************************************************************************/


int Incremental_PID_PWM(int Number,float Measured,float Target,int period)
{ 	float Kp,Ki,Kd;
	switch(5)
		{ case 1: Kp=20,Ki=0,Kd=0;	break;
			case 2:Kp=50,Ki=0.4,Kd=0;	break;
			case 3:Kp=50,Ki=0,Kd=0;	break;
			case 4:Kp=10,Ki=0,Kd=0;	break;
			case 5:Kp=10,Ki=0,Kd=0;	break;
		}
	 static float Bias,Last_bias[Quantity],Previous_bias[Quantity],Result[Quantity],sum_error=0;
	 Bias = Target - Measured;                     //计算偏差
		sum_error+=Bias;
//	 Result[Number] += Kp*(Bias-Last_bias[Number])
//	 +Ki*Bias+Kd*(Bias - 2*Last_bias[Number] + Previous_bias[Number]);  //增量式PID控制器
		
		Result[Number] +=0.01 *( Kp*(Bias) +Ki*sum_error +Kd*(Bias -Previous_bias[Number] )  );  //增量式PID控制器
	printf("result %f\n",Result[Number]);
	 Previous_bias[Number]=Last_bias[Number];        //保存上次偏差
	 Last_bias[Number]=Bias;	                   //保存本次偏差 
		
	 if( Result[Number]>= 0.5*period) Result[Number] = (int)(0.5*period);
	 else if( Result[Number]<= -0.5*period)  Result[Number] = -0.5*period;
		#if 0
		printf("target  %f\n",		Target);
		printf("measured  %f\n",	Measured);
		printf("bias  %f\n",Bias);
		printf("sum_error  %f\n",sum_error);
		printf("last bias  %f\n",Last_bias[Number]);
		printf("previous bias  %f\n",Previous_bias[Number]);
		printf("result %f\n",Result[Number]);
		#endif
	 return Result[Number]; 
    
}




