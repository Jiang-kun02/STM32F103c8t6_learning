#include "stm32f10x.h"                  // Device header
#include "AS5600.h"

// @brief 初始化AS5600模块
// @param RegAddress 是需要配置的寄存器地址
// @param Data		 是需要传入寄存器的数据
// @retval None
void AS5600_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	/*
	MyI2C_Start();
	MyI2C_SendByte(AS5600_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(Data);
	MyI2C_ReceiveAck();
	//这里可以顺序写入数据,写完了再停止
	// MyI2C_SendByte(Data);
	// MyI2C_ReceiveAck();
	MyI2C_Stop();
	*/
	
	I2C_GenerateSTART(I2C2 , ENABLE);		//生成起始条件
	while(I2C_CheckEvent(I2C2 , I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)	//这里有三种检测方式，第一种基础状态检测事件。第二种事高级状态检测。第三种是标志位检测。
	{
		//EV5
		//这里我们需要做一些超时退出机制，避免程序堵塞。同时需要加一些警告信息。
	}
	// @brief 硬件I2C ， 从机地址 ， 发送(读)/接收(写)
	I2C_Send7bitAddress(I2C2 , AS5600_ADDRESS , I2C_Direction_Transmitter);		//发送从机地址 , 这里面带了应答位的处理
	while(I2C_CheckEvent(I2C2 , I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS)	//这里有三种检测方式，第一种基础状态检测事件。第二种事高级状态检测。第三种是标志位检测。
	{
		//EV6_Transmitter
		//这里我们需要做一些超时退出机制，避免程序堵塞。同时需要加一些警告信息。
	}
	//发送寄存器地址
	I2C_SendData(I2C2 , RegAddress);
	while(I2C_CheckEvent(I2C2 , I2C_EVENT_MASTER_BYTE_TRANSMITTING) != SUCCESS)	//这里有三种检测方式，第一种基础状态检测事件。第二种事高级状态检测。第三种是标志位检测。
	{
		//EV8
		//这里我们需要做一些超时退出机制，避免程序堵塞。同时需要加一些警告信息。
	}
	//发送数据
	I2C_SendData(I2C2 , Data);
	while(I2C_CheckEvent(I2C2 , I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS)	//这里有三种检测方式，第一种基础状态检测事件。第二种事高级状态检测。第三种是标志位检测。
	{
		//EV8_2
		//这里我们需要做一些超时退出机制，避免程序堵塞。同时需要加一些警告信息。
	}
	//产生停止信号
	I2C_GenerateSTOP(I2C2 , ENABLE);
	
}

// @ brief 读取寄存器的值
// @ param RegAddress 寄存器的地址
// @ retval 寄存器的值
uint8_t AS5600_ReadReg(uint8_t RegAddress)
{
	/*
	uint8_t data = 0x00;

	MyI2C_Start();
	MyI2C_SendByte(AS5600_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	
	MyI2C_Start();
	MyI2C_SendByte(AS5600_ADDRESS | 0x01);
	MyI2C_ReceiveAck();
	data = MyI2C_ReceiveByte();
	MyI2C_SendAck(1);
	MyI2C_Stop();
	
	return data;
	*/
	
	uint8_t data = 0x00;
	
	I2C_GenerateSTART(I2C2 , ENABLE);		//生成起始条件
	while(I2C_CheckEvent(I2C2 , I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)	//这里有三种检测方式，第一种基础状态检测事件。第二种事高级状态检测。第三种是标志位检测。
	{
		//EV5
		//这里我们需要做一些超时退出机制，避免程序堵塞。同时需要加一些警告信息。
	}
	// @brief 硬件I2C ， 从机地址 ， 发送(读)/接收(写)
	I2C_Send7bitAddress(I2C2 , AS5600_ADDRESS , I2C_Direction_Transmitter);		//发送从机地址 , 这里面带了应答位的处理
	while(I2C_CheckEvent(I2C2 , I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS)	//这里有三种检测方式，第一种基础状态检测事件。第二种事高级状态检测。第三种是标志位检测。
	{
		//EV6_Transmitter
		//这里我们需要做一些超时退出机制，避免程序堵塞。同时需要加一些警告信息。
	}
	//发送寄存器地址
	I2C_SendData(I2C2 , RegAddress);
	while(I2C_CheckEvent(I2C2 , I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS)	//这里有三种检测方式，第一种基础状态检测事件。第二种事高级状态检测。第三种是标志位检测。
	{
		//EV8_2
		//这里我们需要做一些超时退出机制，避免程序堵塞。同时需要加一些警告信息。
	}
	//生成重复起始条件
	I2C_GenerateSTART(I2C2 , ENABLE);		//生成起始条件
	while(I2C_CheckEvent(I2C2 , I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)	//这里有三种检测方式，第一种基础状态检测事件。第二种事高级状态检测。第三种是标志位检测。
	{
		//EV5
		//这里我们需要做一些超时退出机制，避免程序堵塞。同时需要加一些警告信息。
	}
	//发送从机地址后开始接收数据
	I2C_Send7bitAddress(I2C2 , AS5600_ADDRESS , I2C_Direction_Transmitter);		//发送从机地址 , 这里面带了应答位的处理
	while(I2C_CheckEvent(I2C2 , I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) != SUCCESS)	//这里有三种检测方式，第一种基础状态检测事件。第二种事高级状态检测。第三种是标志位检测。
	{
		//EV6_Receiver
		//这里我们需要做一些超时退出机制，避免程序堵塞。同时需要加一些警告信息。
	}
	//此次我们仅收一个字节，应马上设置应答位和停止位
	I2C_AcknowledgeConfig(I2C2 , DISABLE);	//配置应答位
	I2C_GenerateSTOP(I2C2 , ENABLE);		//配置停止位,申请产生停止条件
	while(I2C_CheckEvent(I2C2 , I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS)	//这里有三种检测方式，第一种基础状态检测事件。第二种事高级状态检测。第三种是标志位检测。
	{
		//EV7
		//这里我们需要做一些超时退出机制，避免程序堵塞。同时需要加一些警告信息。
	}
	//读取数据
	data = I2C_ReceiveData(I2C2);	//从DR寄存器中读取刚收到的数据
	
	//将应答位设置为1，方便下一次读取I2C
	I2C_AcknowledgeConfig(I2C2 , ENABLE);	//配置应答位
	
	return data;
}


// @brief 初始化AS5600模块
// @retval None
void AS5600_Init(void)
{
	//这是使用硬件I2C来发送和读取数据，所以要打开I2C的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2 , ENABLE);	//PB10 和 PB11
	//打开GPIO的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	
	//配置GPIO为复用开漏的模式并初始化
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB , &GPIO_InitStruct);
	
	//配置硬件I2C并初始化
	I2C_InitTypeDef I2C_InitStruct;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;			//之后会有函数可以改这里
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;	//这是作为从机时的，指定STM32可以响应几位的地址
	I2C_InitStruct.I2C_ClockSpeed = 50000;				//I2C的标准速度是100kHz , 快速高达400kHz
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;		//这个占空比是为了I2C的快速模式建立的，在标准速度以下都是1:1的占空比，所以这里配置没用。主要是因为弱上拉，从0到1需要一定时间，如果SCL的低电平时间长一点，有利于SDA的数据变化
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;				//I2C的模式，因为I2C兼容了SMBALERT 系统管理总线警告System Management Bus Alert
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;		//这是作为从机时的，自身地址，先随便给
	I2C_Init(I2C2 , &I2C_InitStruct);	
	
	I2C_Cmd(I2C2 , ENABLE);
	
	
	// 后面需要配置这两个寄存器，来修改AS5600的工作模式
	// #define AS5600_CONF_H 0x07
	// #define AS5600_CONF_L 0x08

}

// @brief 获取AS5600处理后的角度数据，也就是0x0E和0x0F两个寄存器的值
// @retval 返回16位的角度数据，但只有低12位有作用
uint16_t AS5600_Getangle(void)
{
	uint8_t DataH, DataL;

	DataH = AS5600_ReadReg(AS5600_RAW_ANGLE_H);
	DataL = AS5600_ReadReg(AS5600_RAW_ANGLE_L);
	
	return (DataH << 8) | DataL;
}



