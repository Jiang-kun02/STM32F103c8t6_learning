#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Motor_BLDC.h"
#include "AS5600.h"

extern float Shaft_angle;

uint16_t angle;

int main(void)
{
	
	OLED_Init();
	// Motor_BLDC_Init();		
	// Motor_Enable();
	AS5600_Init();  
	while(1)
	{
		// OLED_ShowNum(1, 2, Shaft_angle, 4);
		// Velocity_Openloop(5);
		angle = AS5600_Getangle();                                                                                                                                                                                                                                                                  
		OLED_ShowNum(2, 2, angle, 4);
		// Delay_ms(500);
	}
	
	
	
}
