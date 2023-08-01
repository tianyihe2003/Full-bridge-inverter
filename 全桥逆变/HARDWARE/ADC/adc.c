#include "adc.h"
#include "math.h"



vu16 AD_Value[N][M]; //用来存放ADC的结果 也是DMA的目标地址
vu16 After_filter[M]; //用来存放平均值之后的结果

//初始化ADC
//开启ADC1的通道0~7,对应关系如下
//ADC1_CH0-->PA0	
//ADC1_CH1-->PA1	
//ADC1_CH2-->PA2
//ADC1_CH3-->PA3	
//ADC1_CH4-->PA4	
//ADC1_CH5-->PA5	
//ADC1_CH6-->PA6
//ADC1_CH7-->PA7	

void  Adc1_Init(void)
{ 


  GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	 DMA_InitTypeDef       DMA_InitStructure;
 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC1时钟

	 /* DMA2 Stream0 channel0 配置 **************************************/
    DMA_InitStructure.DMA_Channel = DMA_Channel_0;       //数据流2，通道0
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;  //外设地址
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&AD_Value;  										//内存地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;  //外设到内存
    DMA_InitStructure.DMA_BufferSize = N*M;    //传输4位
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //不增加外设寄存器
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;    //内存可增加
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //外设格式
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;   //内存格式
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;     //循环传输
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;    //高优先级
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         //不用fifo
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;  //单次突发模式
/*DMA都是为了成批传输数据的，不论单次DMA 和突发DMA。不同的是每次传输一个单元数据（比如使用32bit数据位宽，每单元为32bit；比如使用8bit数据位宽，每单元为8bit）所用的时钟数。突发DMA是每个时钟都传输一个单元数据的连续传输；而单次DMA则不是，需要几个时钟才传输一个数据单元。*/        
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//单次突发模式
    DMA_Init(DMA2_Stream0, &DMA_InitStructure);
    DMA_Cmd(DMA2_Stream0, ENABLE);


//Additionalfunctions：附加，辅助功能，引脚被连接到其他模块使用，使用时直接普通配置即可，
//例如ADC的采用输入通道，配置为模拟输入，不用单独用GPIO_PinAFConfig（）配置，直接在引脚配置里使用AN模式。
	//用了8个AD通道
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;//|GPIO_Pin_4|GPIO_Pin_5;//PA4 通道5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化  	 
 
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
//  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式 
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;//两个采样阶段之间的延迟5个时钟
 ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
 //ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//初始化
	
     /* ADC1 初始化 ****************************************************************/
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;  //12位模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;    //连续扫描模式，多通道必开
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  //开启转换模式
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//没有外部触发，用软件触发
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  //右对齐
    ADC_InitStructure.ADC_NbrOfConversion = M;// yuan 2，四个通道
    ADC_Init(ADC1, &ADC_InitStructure);

    //转换次序和转换时间
		//AD_Value的0-2为输入三相电压
		ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_15Cycles);  //ADC10对应AD[0]对应p5  	//PC0
    ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_15Cycles);  //ADC11对应AD[1]对应p6		//PC1
    ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_15Cycles);													//PC2
		//AD_Value的3-5为输入三相电流
    ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 4, ADC_SampleTime_3Cycles);													//PC3
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 5, ADC_SampleTime_3Cycles);													//PC4
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 6, ADC_SampleTime_3Cycles);													//PC5
//    
//		ADC_RegularChannelConfig(ADC1, ADC_Channel_0,  7, ADC_SampleTime_3Cycles);  //ADC0对应AD[6]对应p1 	 	//PA0
//	  ADC_RegularChannelConfig(ADC1, ADC_Channel_1,  8, ADC_SampleTime_3Cycles);  //ADC1对应AD[7]对应p2		//PA1
 
  ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);//开启AD转换器	
  ADC_SoftwareStartConv(ADC1);
	

}


	
	


//读取通道电压值
int ADC_Value(u8 Channel)	 //Channel选择ADC通道
{
	unsigned int sum=0,temp;
	u8  number; 				   
	for ( number=0;number<N;number++)	   	            //每通道采N次		
	{  	
		temp=AD_Value[number][Channel];			//每一行是一组数据，包括多个通道
		sum += temp;			//通道Channel	  	
	}  		   		
	After_filter[Channel]=sum/N;		
	sum=0;	   
	return After_filter[Channel];   //3.3V的电平，计算等效电平	  
}
//
int Adfilter(u8 Channel)    
{
	unsigned int num=0;
	u8 i;
	for(i=0;i<100;i++)
	{
		  num+= ADC_Value(Channel);
	}
	i=0;
	return (u16)(num/100);
}



/*******************************************************************************************
*函数名称:int filter(u8 Channel)    //中位值平均滤波法（又称防脉冲干扰平均滤波法）
*入口参数:u8 Channel                //选择ADC通道
*返回值:  (sum/(N-2))               //滤波后结果
*函数功能:相当于“中位值滤波法”+“算术平均滤波法”
*函数说明:连续采样 N 个数据，去掉一个最大值和一个最小值
*         然后计算 N-2 个数据的算术平均值
*         N 值的选取：3~14
*******************************************************************************************/
uint16_t ADC_Filter(u8 Channel)		//中位值平均滤波法（又称防脉冲干扰平均滤波法）
{
	uint8_t count,i,j;
	uint32_t sum=0;
	uint16_t temp=0;
    uint16_t tempbuf[N];
    for(i=0;i<N;i++)
    {
        tempbuf[i] = AD_Value[i][Channel];
    }
	for (j=0;j<N;j++)     //冒泡法
	{
		for (i=j;i<N;i++)
		{
			if ( tempbuf[i]>tempbuf[j])
			{
				temp = tempbuf[i];
				tempbuf[i] = tempbuf[j];
				tempbuf[j] = temp;
			}
		}
	}
	for ( count=2;count<N-2;count++) 
		sum = sum + tempbuf[count];
	return (sum/(N-4));
}


/*****************************************************************************
*函数名称:int Rms_voltage( int *arr,int data_len )
*入口参数:int *arr               //输入数据指针
*         int data_len           //数据长度
*返回值:  rms                    //均方根值
*函数功能:取均方根值
*****************************************************************************/
int Rms_voltage( int *arr,int data_len )
{
	static int sum=0,rms=0;
	int i;
	for(i=0;i<data_len;i++)
	{
		sum+=arr[i]*arr[i];         //计算平方和
	}
	rms=(int)sqrt(sum/data_len);  //平方和取平均，再开方
	sum=0;        
	return rms;
}



/*************************************************************************************************************
*函数名称:int First_order_filter(u8 Channel,u8 coefficient)   //一阶低通滤波器（又称一阶惯性滤波器）
*入口参数:u8 Channel                                    //选择ADC通道
*         u8 coefficien                                //滤波器滤波系数   取值0--100
*返回值:  filtered_data                                //滤波后结果
*函数功能:一阶低通滤波
*函数说明:Y(n)=aX(n)+(1-a)Y(n-1)
*				 X(n)为本次采样值
*				 Y(n-1)为上次滤波输出值
*				 Y(n)为本次滤波输出值
*        一阶低通滤波器采用本次采样值与上次滤波输出值进行加权，得到有效滤波值，使得输出对输入有反馈作用
*************************************************************************************************************/
int First_order_filter(u8 Channel,u8 coefficient)    //一阶低通滤波器（又称一阶惯性滤波器）
{
	static int filtered_data[16],data_collection[16];
    
  data_collection[Channel]=ADC_Filter(Channel);            //获取ADC采集值      
	if(data_collection[Channel] < filtered_data[Channel])
	{
		filtered_data[Channel]=filtered_data[Channel] - (filtered_data[Channel] - data_collection[Channel])*coefficient/100;
	}
	else 
	{
		filtered_data[Channel]=filtered_data[Channel] + (data_collection[Channel] - filtered_data[Channel])*coefficient/100;
	}
	return filtered_data[Channel];
}




