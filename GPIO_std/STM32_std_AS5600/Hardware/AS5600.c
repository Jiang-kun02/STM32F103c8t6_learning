#include "stm32f10x.h"                  // Device header
#include "AS5600.h"

// @brief 初始化AS5600模块
// @param RegAddress 是需要配置的寄存器地址
// @param Data		 是需要传入寄存器的数据
// @retval None
void AS5600_WriteReg(uint8_t RegAddress, uint8_t Data)
{
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
}

// @ brief 读取寄存器的值
// @ param RegAddress 寄存器的地址
// @ retval 寄存器的值
uint8_t AS5600_ReadReg(uint8_t RegAddress)
{
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
}


// @brief 初始化AS5600模块
// @retval None
void AS5600_Init(void)
{
	MyI2C_Init();
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



