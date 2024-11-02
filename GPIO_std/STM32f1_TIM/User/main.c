#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"

uint16_t counter = 0;

int main(void)
{
	
	OLED_Init();
	Timer2_Init();
	
	while(1)
	{
		OLED_ShowNum(1 , 1 , counter , 5);
	}
}
