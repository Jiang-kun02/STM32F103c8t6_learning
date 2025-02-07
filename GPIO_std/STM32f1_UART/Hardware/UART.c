#include "stm32f10x.h"                  // Device header


uint8_t Serial_RxData;
uint8_t Serial_RxFlag;

void UART1_Init(void)
{
	//开启时钟，这里 UART1是高速的APB2时钟 , 同时开启GPIOA的时钟，因为UART1的TX和RX分别是PA9和PA10
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	
	//配置并初始化GPIO引脚到对应的输出输入模式
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA , &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;	//这里选择上拉输入，因为串口是为数组信号所以上拉浮空都可以
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA , &GPIO_InitStruct);
	
	//配置并初始化UART1
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 9600;			//波特率
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//是否启用硬件控制
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	//配置为TX和RX 的输出输入模式
	USART_InitStruct.USART_Parity = USART_Parity_No;	//没有奇偶校验位
	USART_InitStruct.USART_StopBits = USART_StopBits_1;		//停止位长度
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;	//传输的长度
	USART_Init(USART1 , &USART_InitStruct);
	
	//这里如果使用中断接收串口信息需要打开，如果使用在主函数中查询便不用
	USART_ITConfig(USART1 , USART_IT_RXNE , ENABLE);	//打开串口1的接收标志位中断
	
	//下面开始配置中断
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//中断分组
	
	//配置并初始化NVIC
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;		//中断通道的选择
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;		//通道使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;	//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;			//从站优先级
	NVIC_Init(&NVIC_InitStruct);
	
	
	//打开串口通信
	USART_Cmd(USART1 , ENABLE);
}

//使用USART1发送一个字节
void USART1_SendByte(uint8_t Byte)
{
	USART_SendData(USART1 , Byte);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	//这个标志位会在SendData后自动变为RESET,直到发送完毕后被置为SET
}

// 通过自定义的标志位 Serial_RxFlag 来判断是否在USART1中接收到了数据
uint8_t UART1_GetRxFlag(void)
{
	if (Serial_RxFlag == 1)
	{
		Serial_RxFlag = 0;
		return 1;
	}
	return 0;
}

//返回USART1接受到的数据
uint8_t UART1_GetRxData(void)
{
	return Serial_RxData;
}

// USART1的串口中断函数，在这个函数里面我们将得到的串口信息保存在 Serial_RxData 中，并置起了一个自定义的标志位 Serial_RxFlag
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1 , USART_IT_RXNE) == SET)
	{
		Serial_RxData = USART_ReceiveData(USART1);
		Serial_RxFlag = 1;
		USART_ClearITPendingBit(USART1 , USART_IT_RXNE);	//如果读取了RX的数据会自动清，但我们还是自己清一下
	}
}








