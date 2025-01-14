#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"

uint16_t ADValue[4];


int main(void)
{
	//Init
	OLED_Init();
	AD_Init();
	
	OLED_ShowString(1, 1, "AD0:");
	OLED_ShowString(2, 1, "AD1:");
	OLED_ShowString(3, 1, "AD2:");
	OLED_ShowString(4, 1, "AD3:");
	
	//main loop
	while(1)
	{
		AD_GetValue();
		
		OLED_ShowNum(1 , 5 , ADValue[0] , 4);
		OLED_ShowNum(2 , 5 , ADValue[1] , 4);
		OLED_ShowNum(3 , 5 , ADValue[2] , 4);
		OLED_ShowNum(4 , 5 , ADValue[3] , 4);
			
		Delay_ms(100);
	}
}
