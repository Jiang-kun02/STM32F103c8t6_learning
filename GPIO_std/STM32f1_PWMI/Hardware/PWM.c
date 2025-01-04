#include "stm32f10x.h"                  // Device header


void PWM_Init(void)
{
	// 打开定时器时钟和GPIO的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA , &GPIO_InitStruct);

	// 选择使用内部时钟 ， 开始配置时钟。
	TIM_InternalClockConfig(TIM2);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 100 - 1;		//ARR 自动重装寄存器
	TIM_TimeBaseInitStruct.TIM_Prescaler = 720 - 1;	//PSC 预分频器
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;	//重复计数器，但只有高级定时器才有
	TIM_TimeBaseInit(TIM2 , &TIM_TimeBaseInitStruct);
	
	//配置OC(输出比较)
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCStructInit(&TIM_OCInitStruct);	//这里是先给TIM_OCInitStruct一个初始值，防止有些值为空
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 0;			// 设置CCR的值
	TIM_OC1Init(TIM2 , &TIM_OCInitStruct);
	
	TIM_Cmd(TIM2 , ENABLE);

}

// 设置CCR的值
void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM2, Compare);
}

// 设置PSC的值
void PWM_SetPrescaler(uint16_t Prescaler)
{
	TIM_PrescalerConfig(TIM2, Prescaler, TIM_PSCReloadMode_Immediate);
}




