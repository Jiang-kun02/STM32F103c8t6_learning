#include "stm32f10x.h"                  // Device header

void AD_Init(void)
{
	//开启ADC的时钟和GPIO的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	
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

	//配置ADC的参数，并初始化ADC
	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;					//连续模式
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;					//数据对齐
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ADC的触发源选择
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;						//选择ADC是工作在独立模式还是双ADC模式
	ADC_InitStruct.ADC_NbrOfChannel = 1;								//扫描模式下的通道数目
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;							//扫描模式
	ADC_Init(ADC1 , &ADC_InitStruct);
	
	//这里因为我们是使用软件来触发ADC转换，所以触发源不用配置，如果您需要可以在这里配置触发源后开启ADC
	//开启ADC 	
	ADC_Cmd(ADC1 , ENABLE);
	
	//校准ADC
	ADC_ResetCalibration(ADC1);							//复位校准
	while( ADC_GetResetCalibrationStatus(ADC1) == SET);	//等待复位校准完成
	ADC_StartCalibration(ADC1);							//开始校准
	while( ADC_GetCalibrationStatus(ADC1) == SET);		//等待开始校准完成
	
}

//获取ADC转换的值
uint16_t AD_GetValue(void)		//这里将参数改为ADC1_GPIOA_PINX 或者 ADC1_GetValue(Channle)
{
	ADC_SoftwareStartConvCmd(ADC1 , ENABLE);	//软件触发ADC1
	while ( ADC_GetFlagStatus(ADC1 , ADC_FLAG_EOC) == SET );
	return ADC_GetConversionValue(ADC1);		//读取或会自动清除EOC标志位
}

