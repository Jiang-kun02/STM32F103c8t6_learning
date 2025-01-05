#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"
#include "Encoder.h"

uint16_t speed = 0;

int main(void)
{
	
	OLED_Init();
	Timer2_Init();
	Encoder_Init();
	
	while(1)
	{
		OLED_ShowSignedNum(1 , 1 , Encoder_Get() , 5);
	}
}




//这个函数在启动文件里面的中断向量表里
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2 , TIM_IT_Update) == SET)
	{
		speed = Encoder_Get();
		TIM_ClearITPendingBit(TIM2 , TIM_IT_Update);
	}

}

