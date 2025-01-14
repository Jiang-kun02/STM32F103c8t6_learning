#include "stm32f10x.h"                  // Device header

uint16_t AD_Value[4];

void AD_Init(void)
{
	//开启ADC的时钟和GPIO的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	//开启DMA的时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);
	
	//配置ADC的分频器系数 最大14M HZ
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);	//这里ADC的时钟频率为 72M / 6 = 12M HZ
	
	//初始化GPIO PA0
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA , &GPIO_InitStruct);
	
	//初始化规则组的输入通道
	// 将ADC1的 0通道，放在规则组的序列1位置，采样时间为55.5个ADCCLK的周期
	ADC_RegularChannelConfig(ADC1 , ADC_Channel_0 , 1 , ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1 , ADC_Channel_1 , 2 , ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1 , ADC_Channel_2 , 3 , ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1 , ADC_Channel_3 , 4 , ADC_SampleTime_55Cycles5);

	//配置ADC的参数，并初始化ADC
	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;					//连续模式
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;					//数据对齐
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ADC的触发源选择
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;						//选择ADC是工作在独立模式还是双ADC模式
	ADC_InitStruct.ADC_NbrOfChannel = 4;								//扫描模式下的通道数目
	ADC_InitStruct.ADC_ScanConvMode = ENABLE;							//扫描模式
	ADC_Init(ADC1 , &ADC_InitStruct);
	
	//这里因为我们是使用软件来触发ADC转换，所以触发源不用配置，如果您需要可以在这里配置触发源后开启ADC
	//配置DMA
	DMA_InitTypeDef DMA_InitStruct;
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)ADC1->DR;				//外设寄存器的地址
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//选择传输数据的大小 字节/半字/字
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//是否自增
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)AD_Value;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;		//方向选择
	DMA_InitStruct.DMA_BufferSize = 4;				//传输计数器的值	范围0~65535
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;			//选择是否软件触发
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;			//传输计数器是否自动重装
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;	//DMA的优先级判断
	DMA_Init(DMA1_Channel1 , &DMA_InitStruct);
	
	//使能ADC 	
	ADC_Cmd(ADC1 , ENABLE);
	//打开DMA1通道1的ADC1硬件的DMA，也就是在硬件触发DMA的时候选择的外设请求信号
	ADC_DMACmd(ADC1 , ENABLE);
	//使能DMA
	DMA_Cmd(DMA1_Channel1 , ENABLE);
	
	//校准ADC
	ADC_ResetCalibration(ADC1);							//复位校准
	while( ADC_GetResetCalibrationStatus(ADC1) == SET);	//等待复位校准完成
	ADC_StartCalibration(ADC1);							//开始校准
	while( ADC_GetCalibrationStatus(ADC1) == SET);		//等待开始校准完成
	
}

//获取ADC转换的值
void AD_GetValue(void)		//这里将参数改为ADC1_GPIOA_PINX 或者 ADC1_GetValue(Channle)
{
	// 在DMA失能的时候设置计数器数目，后使能DMA
	DMA_Cmd(DMA1_Channel1, DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel1, 4);
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	//软件触发ADC，在ADC转化完一个后会自动硬件触发DMA数据转运
	ADC_SoftwareStartConvCmd(ADC1 , ENABLE);	//软件触发ADC1
		
	//所以我们仅需要等待DMA的转运完成，标志着全部数据从ADC转换到DMA转运的完成。
	while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
	DMA_ClearFlag(DMA1_FLAG_TC1);
}

