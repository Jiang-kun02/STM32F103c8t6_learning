#ifndef AS5600_REG_H
#define AS5600_REG_H

//////////////////////////////////////////////Configuration Resisters///////////////////////////////////
//烧录次数 ， 这里烧录次数不得多余三次
#define AS5600_ZMCO 0X00

// 0位置
#define AS5600_ZPOS_H 0x01
#define AS5600_ZPOS_L 0x02

// 最大位置
#define AS5600_MPOS_H 0x03
#define AS5600_MPOS_L 0x04

// 角度上限
#define AS5600_MANG_H 0x05
#define AS5600_MANG_L 0x06

// config配置工作模式
#define AS5600_CONF_H 0x07
#define AS5600_CONF_L 0x08

//////////////////////////////////////////////Output Resisters///////////////////////////////////
// 原始角度
#define AS5600_RAW_ANGLE_H 0x0C
#define AS5600_RAW_ANGLE_L 0x0D

// 校正后的角度
#define AS5600_ANGLE_H 0x0E
#define AS5600_ANGLE_L 0x0F

//////////////////////////////////////////////Status Resisters///////////////////////////////////
// 磁场状态寄存器
#define AS5600_STATUS 0x0B
#define AS5600_AGC 0x1A

// 自动增益控制值
#define AS5600_MAGNITUDE_H 0x1B
#define AS5600_MAGNITUDE_L 0x1C

//////////////////////////////////////////////Burn Resisters///////////////////////////////////
// 烧录寄存器，生产使用的
#define AS5600_BURN 0xFF

#endif
