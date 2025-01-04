#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Motor_BLDC.h"

extern float Shaft_angle;

int main(void)
{
	
	OLED_Init();
	Motor_BLDC_Init();		
	Motor_Enable();
	while(1)
	{
		OLED_ShowNum(1, 2, Shaft_angle, 4);
		Velocity_Openloop(5);
		// OLED_ShowNum(2, 2, Shaft_angle, 4);
		// Delay_ms(500);
	}
	
	
	
}
