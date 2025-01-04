#include "stm32f10x.h"                  // Device header


void IC_Init(void)
{
	// 打开定时器时钟和GPIO的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;				// 这里选择的是上拉输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;		//	GPIOA_PIN_6 是TIM3的CH1引脚
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA , &GPIO_InitStruct);

	// 选择使用内部时钟 ， 开始配置时钟。
	TIM_InternalClockConfig(TIM3);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 65536 - 1;		//ARR 自动重装寄存器
	TIM_TimeBaseInitStruct.TIM_Prescaler = 72 - 1;	//PSC 预分频器
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;	//重复计数器，但只有高级定时器才有
	TIM_TimeBaseInit(TIM3 , &TIM_TimeBaseInitStruct);

	//配置IC(输入比较的参数)
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_ICStructInit(&TIM_ICInitStruct);
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStruct.TIM_ICFilter = 0xF;	//这里给了滤波，具体是怎样的滤波形式得去查看手册
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;					// 这里是设置分频器的值，也就是说来了多个TI1FP1我才执行CNT转CCR
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;		//设置直连输入
	// TIM_ICInit(TIM3 , &TIM_ICInitStruct);	这是配置普通IC的，下面有一个配置PWMI模式的，会将TI1FP2配置成相反的
	TIM_PWMIConfig(TIM3 , &TIM_ICInitStruct);
	
	
	//接下来配置主从触发模式，我们想要的是当TI1FP1来的时候，CNT转到CCR后，CNT自动清0
	TIM_SelectInputTrigger(TIM3 , TIM_TS_TI1FP1);		// 触发源选择TIM3的CH1通道的TI1FP1
	TIM_SelectSlaveMode(TIM3 , TIM_SlaveMode_Reset);	
	
	// 开启定时器
	TIM_Cmd(TIM3 , ENABLE);	
}

uint32_t IC_GetFreq(void)
{
	// 这里我们设置的TIM3 的PSC为72 - 1 ， 则我们的fc = 72M / 72 = 1M = 1000000
	return 1000000 / TIM_GetCapture1(TIM3);

}

uint32_t IC_GetDuty(void)
{
	return TIM_GetCapture2(TIM3) * 100 / TIM_GetCapture1(TIM3);

}

