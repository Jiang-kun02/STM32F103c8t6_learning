#include "stm32f10x.h"                  // Device header

//注意，此模块我们没有使用，我们将DMA融合到AD里面去了，请看AD.c文件

uint16_t MyDMA_Size;

void MYDMA_Init(uint32_t addressPeripheral , uint32_t addressMemory , uint16_t size)
{
	//开启DMA的时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);
	
	//配置DMA
	DMA_InitTypeDef DMA_InitStruct;
	DMA_InitStruct.DMA_PeripheralBaseAddr = addressPeripheral;				//外设寄存器的地址
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//选择传输数据的大小 字节/半字/字
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//是否自增
	DMA_InitStruct.DMA_MemoryBaseAddr = addressMemory;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;		//方向选择
	DMA_InitStruct.DMA_BufferSize = size;				//传输计数器的值	范围0~65535
	DMA_InitStruct.DMA_M2M = DMA_M2M_Enable;			//选择是否软件触发
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;			//传输计数器是否自动重装
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;	//DMA的优先级判断
	DMA_Init(DMA1_Channel1 , &DMA_InitStruct);

	//失能DMA，我们可以用函数手动打开
	DMA_Cmd(DMA1_Channel1 , DISABLE);
}

void MyDMA_Transfer(void)
{
	DMA_Cmd(DMA1_Channel1, DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel1, MyDMA_Size);
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
	DMA_ClearFlag(DMA1_FLAG_TC1);
}

