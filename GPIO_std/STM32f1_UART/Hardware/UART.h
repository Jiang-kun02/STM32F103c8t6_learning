#ifndef UART_H
#define UART_H

void UART1_Init(void);
void USART1_SendByte(uint8_t Byte);
uint8_t UART1_GetRxFlag(void);
uint8_t UART1_GetRxData(void);



#endif
