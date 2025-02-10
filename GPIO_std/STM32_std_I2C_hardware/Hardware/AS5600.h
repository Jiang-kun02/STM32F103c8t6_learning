#ifndef AS5600_H
#define AS5600_H

#include "AS5600_Reg.h"

#define AS5600_ADDRESS 0x6C

void AS5600_WriteReg(uint8_t RegAddress, uint8_t Data);
uint8_t AS5600_ReadReg(uint8_t RegAddress);
void AS5600_Init(void);
uint16_t AS5600_Getangle(void);

#endif

