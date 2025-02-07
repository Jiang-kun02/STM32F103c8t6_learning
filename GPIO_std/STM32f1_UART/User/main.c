#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "UART.h"

uint8_t RxData;


int main(void)
{
	OLED_Init();
	OLED_ShowString(1, 1, "RxData:");
	
	UART1_Init();
	
	while(1)
	{
		if (UART1_GetRxFlag() == 1)
		{
			RxData = UART1_GetRxData();
			USART1_SendByte(RxData);	//发送一个字节
			OLED_ShowHexNum(1, 8, RxData, 2);	//中断里面会完成数据
		}
	}
}
