#include "stm32f10x.h"                  // Device header


void Timer2_Init(void)
{
	//开启内部时钟源TIM2的时钟，并不是配置时基单元。
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	
	//选择内部时钟源TIM2 ， 这里开始配置定时器
	//值得注意一下的是如何计算定时时间，首先您的内部时钟源 = CK_HZ , 通过与分频器后 = CK_HZ / (PSC + 1) , 那么每一个计数 = (PSC + 1) / CK_HZ 。则更新的时间 = (PSC + 1) / CK_HZ * (ARR + 1);
	TIM_InternalClockConfig(TIM2);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;		//这里是输入滤波的参数，当连续多少个点都是高，才判断为高
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;	//向上计数模式
	TIM_TimeBaseInitStruct.TIM_Period = 10000 - 1;					//ARR 自动重装寄存器的值
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200 - 1;				//预分频器的值
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;				//重复计数器，但只有高级定时器才有
	TIM_TimeBaseInit(TIM2 , &TIM_TimeBaseInitStruct);
	
	//配置完成时基单元后配置更新中断，首先使能定时器的更新中断
	TIM_ClearFlag(TIM2 , TIM_FLAG_Update);	//这是因为TIM_TimeBaseInit初始化时基单元的过程中会产生更新时间导致中断标志位置1了，如果不清楚，那么以上点就进中断了。
	TIM_ITConfig(TIM2 , TIM_IT_Update , ENABLE);

	//配置NVIC
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
	
	//使能TIMER2时钟
	TIM_Cmd(TIM2 , ENABLE);
	
}



