#include "stm32f10x.h"                  // Device header

void Encoder_Init(void)
{
	//开启TIM时钟和GPIO的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);

	//初始化GPIO	这里TIM3的CH1 和 CH2分别是PA6 和 PA7
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA , &GPIO_InitStruct);
	
	// 配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;		// 已经被编码器托管了，无作用
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;	// 已经被编码器托管了，无作用
	TIM_TimeBaseInitStruct.TIM_Period = 65536 - 1;		//满量程
	TIM_TimeBaseInitStruct.TIM_Prescaler = 1 - 1;		//这里已经被编码器托管了，所以不用分频。
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;	//这里没用，同时仅高级定时器起作用
	TIM_TimeBaseInit(TIM3 , &TIM_TimeBaseInitStruct);
	
	// 配置出入捕获IC 注意这里不想GPIO，这里CH1和CH2得分开初始化
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_ICStructInit(&TIM_ICInitStruct);
	// TIM_ICInitStruct.TIM_Channel = TIM_Channel_1 | TIM_Channel_2;	//这里似乎不能这么写，0x0000 0x0004
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStruct.TIM_ICFilter = 0xF;
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;			// 这里在下面编码器模式配置会再配置一次
	// TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;				//不管后续电路CCR和事情
	// TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;		//不管后续电路CCR和事情
	TIM_ICInit(TIM3 , &TIM_ICInitStruct);
	
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStruct.TIM_ICFilter = 0xF;
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;		// 这里在下面编码器模式配置会再配置一次
	TIM_ICInit(TIM3 , &TIM_ICInitStruct);
	
	//配置编码器模式
	TIM_EncoderInterfaceConfig(TIM3 , TIM_EncoderMode_TI12 , TIM_ICPolarity_Rising , TIM_ICPolarity_Rising);

	TIM_Cmd(TIM3 , ENABLE);
}



int16_t Encoder_Get(void)
{
	int16_t temp;
	temp = TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3 , 0);
	return temp;
}



