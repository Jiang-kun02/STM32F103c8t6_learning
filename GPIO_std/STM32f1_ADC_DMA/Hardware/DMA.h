#ifndef DMA_H
#define DMA_H

//注意，此模块我们没有使用，我们将DMA融合到AD里面去了，请看AD.h文件
void MYDMA_Init(uint32_t addressPeripheral , uint32_t addressMemory , uint16_t size);
void MyDMA_Transfer(void);


#endif


