#ifndef MOTOR_BLDC_H
#define MOTOR_BLDC_H

#include "math.h"

#define PI 3.1416
#define MOTOR_POLE_PAIRS 7
#define VOLTAGE_POWER_SUPPLY 12.10

// 首先要初始化PWMA , PWMB , PWMC三个GPIO口，他们按照道理来说应该是输出PWM的
// 根据STM32F103CBT6 的引脚定义表， 这里使用的 TIM3_CH1-PB4-40 , TIM3_CH2-PB5-41 , TIM3_CH3-PB0-18
// 因为本项目使用的是DRV8313内部集成了MOS管，所以我们可以使用一个通用定时器完成三项无刷电机的驱动，其他的DRV8313给我们完成了。
// 这里说白了就是配置三路PWM ， 所以代码里 先打开各路时钟 , AFIO复用端口 , 配置定时器 , 配置输出比较。
void Motor_BLDC_Init(void);

void Motor_Enable(void);
void Motor_Disable(void);

// 一次性设置TIM3的三个PWM的占空比
void Motor_SetPWM(uint16_t PWMA , uint16_t PWMB , uint16_t PWMC );

// 设置PWM的占空比
void PWM_SetCompare1(uint16_t Compare);
void PWM_SetCompare2(uint16_t Compare);
void PWM_SetCompare3(uint16_t Compare);

// 角度归一化
float Normalize_Angle(float angle);
// 输入机械角度得到电角度
float Get_Electrical_Degree(float Mec_Angle);

//设置PWM
void SetPWM(float Ua , float Ub , float Uc);
//限制电压函数
int Limit_Voltage(float number , int Limit_MIN , int Limit_MAX);

// 设置Uq大小，控制力矩
void SetPhaseVoltage(float Uq , float Ud , float elc_angle);

// 开环速度控制
float Velocity_Openloop(float target_vel);




#endif
