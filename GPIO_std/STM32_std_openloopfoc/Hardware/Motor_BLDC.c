#include "stm32f10x.h"                  // Device header
#include "Motor_BLDC.h"		// 正常来说, .c 文件会隐式包含 .h文件，但是我不包含这个似乎会出现宏定义的不能使用
#include "OLED.h"			// 这个头文件是为了打印一些数据的时候使用的，debug专用


extern float Electrical_Degree;
extern float Mechanical_Angle;

float Shaft_angle = 0;	//这个全局变量使用来存储机械角度的

// @brief 这个函数将初始化三项无刷电机的三个输入端。
// @retval None
void Motor_BLDC_Init(void)
{
	//开启RCC时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);
	
	//引脚重映射
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3 , ENABLE);		//TIM3的部分重映射，可以让TIM3的四个输出变为 PB 4 5 0 1
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST , ENABLE);		//关闭默认的调试端口，这里有三个参数，第三个参数不能选否则SWD调试也会失效
	
	//配置GPIOB
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//这里采用的是复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//配置DRV_EN 驱动芯片的使能				这段代码是新加的，用来配置芯片的EN引脚
	//GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB , &GPIO_InitStructure);

	//选择内部时钟TIM3 ， 开始配置定时器
	TIM_InternalClockConfig(TIM3);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 100 - 1;		// ARR 自动重装器
	TIM_TimeBaseInitStruct.TIM_Prescaler = 720 - 1;		// PSC 预分频器
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3 , &TIM_TimeBaseInitStruct);
	
	//配置输出比较电路
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);		//给结构体附一个初始值
	// TIM_OCInitStructure.TIM_OCIdleState = ;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	// TIM_OCInitStructure.TIM_OCNIdleState = ;
	// TIM_OCInitStructure.TIM_OCNPolarity = ;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	// TIM_OCInitStructure.TIM_OutputNState = ;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR寄存器的值
	TIM_OC1Init(TIM3 , &TIM_OCInitStructure);
	TIM_OC2Init(TIM3 , &TIM_OCInitStructure);
	TIM_OC3Init(TIM3 , &TIM_OCInitStructure);
	
	TIM_Cmd(TIM3 , ENABLE);
	//这样的参数配置下来就是 一个1k赫兹，占空比为 0 = CCR ， 分辨率为1%
}



// @brief 这里是一个使能驱动芯片，让他工作
void Motor_Enable(void)
{	
	GPIO_SetBits(GPIOB , GPIO_Pin_12);
}

// @brief 这里是一个失能驱动芯片，让他不工作
void Motor_Disable(void)
{	
	GPIO_ResetBits(GPIOB , GPIO_Pin_12);
}



// @brief 这函数是用来设置TIM3的CH1-3的CCR寄存器的值，暂时设置的精度是1% ， 同时CRR的值范围是1-100
void Motor_SetPWM(uint16_t PWMA , uint16_t PWMB , uint16_t PWMC )
{
	TIM_SetCompare1(TIM3, PWMA);
	TIM_SetCompare2(TIM3, PWMB);
	TIM_SetCompare3(TIM3, PWMC);
}

// @brief 这个函数是单独设置TIM3_CH1的CCR的值
// @param Compare就是CCR的值
// retval None
void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM3, Compare);
}

// @brief 这个函数是单独设置TIM3_CH2的CCR的值
// @param Compare就是CCR的值
// retval None
void PWM_SetCompare2(uint16_t Compare)
{
	TIM_SetCompare2(TIM3, Compare);
}

// @brief 这个函数是单独设置TIM3_CH3的CCR的值
// @param Compare就是CCR的值
// retval None
void PWM_SetCompare3(uint16_t Compare)
{
	TIM_SetCompare3(TIM3, Compare);
}


// @brief 控制角度在[0 , 2PI] 的范围内
// @param angle是需要归一化的角度
// @retval 返回归一化的角度
float Normalize_Angle(float angle)		//这个函数是GPT帮我写的，之后我们再修改
{
    // 首先通过加减2π，将角度值限制到[0, 2π]的范围
    while (angle >= 2 * 3.1416)
    {
        angle -= 2 * 3.1416;
    }
    
    while (angle < 0)
    {
        angle += 2 * 3.1416;
    }

    return angle;
}

// @brief 计算电角度
// @param float Mec_Angle 这是机械角度
// @retval 返回值是电角度 = 机械角度 * 极对数
float Get_Electrical_Degree(float Mec_Angle)
{
	return Mec_Angle * MOTOR_POLE_PAIRS;		// 这个代码这里居然需要加上 #include"Motor_BLDC.h" 才能使用宏定义，按道理来说应该是隐式包含才对
}


// @brief 通过设置CCR的值调节PWM的占空比
// @param 
// @retval None
void SetPWM(float Ua , float Ub , float Uc)
{
	int DC_A = Limit_Voltage(Ua * 0.083 * 100 , 0 , 70);
	int DC_B = Limit_Voltage(Ub * 0.083 * 100 , 0 , 70);
	int DC_C = Limit_Voltage(Uc * 0.083 * 100 , 0 , 70);
	TIM_SetCompare1(TIM3, DC_A);
	TIM_SetCompare2(TIM3, DC_B);
	TIM_SetCompare3(TIM3, DC_C);
	// debug
	OLED_ShowNum(2, 1, DC_A, 3);
	OLED_ShowNum(2, 5, DC_B, 3);
	OLED_ShowNum(2, 9, DC_C, 3);
}


// @brief Limit_Voltage函数将限制幅度大小在下届和上届之间
// @param number为要被限幅的值
// @param Limit_MIN为下届
// @param Limit_MAX为上届
// @retval 返回上届或下届或number的int形式
int Limit_Voltage(float number , int Limit_MIN , int Limit_MAX)
{
	if(number > Limit_MAX)
	{
		return Limit_MAX;
	}
	else if(number < Limit_MIN)
	{
		return Limit_MIN;
	}
	else 
	{
		return number;
	}
}


// @brief SetPhaseVoltage函数将设置Park变换后的Uq 和 Ud 值，同时传入机械角度
// @param Uq的大小决定了力矩的大小
// @param Ud的大小决定了消磁的大小，但在一般的FOC中我们一般给0 ，因为非0值会导致电机发热
// @param elc_angle机械角度，这个角度一般由磁传感器获得
// @retval None
void SetPhaseVoltage(float Uq , float Ud , float elc_angle)
{
	elc_angle = Normalize_Angle(elc_angle + 0.0);		// 这里加上0.0是表示初始角度为0.0
	
	//帕克变换
	float Ualpha = -Uq * sin(elc_angle);
	float Ubeta = Uq * cos(elc_angle);
	
	//卡拉克变换	这里细节一定要做好，在STM32中有很多隐式的地板除法，会导致很多的 /2 变整数丢精度的问题，咋们换成 *0.5就可以解决该问题。
	float Ua = Ualpha + VOLTAGE_POWER_SUPPLY * 0.5;
	float Ub = (sqrt(3)*Ubeta - Ualpha) * 0.5 + VOLTAGE_POWER_SUPPLY * 0.5;
	float Uc = (-Ualpha - sqrt(3)*Ubeta) * 0.5  + VOLTAGE_POWER_SUPPLY * 0.5;
	
	SetPWM(Ua , Ub , Uc);	
}



// @brief 我们将1代表上管道通，0代表下管道通 让电机旋转一圈可以使用
// 100 , 000 , 110 , 111 , 010 , 000 , 011 , 111 , 001 , 000 , 101 , 111 通过这个顺序可以让电机转动一圈







// @brief 这是一个Clarke_transform 在FOC中是将三项的矢量变成两项的横纵坐标形式
//
// retval


// @brief 开环速度函数
// @param target_vel表示目标速度
// @retval 返回设置的Uq值，Uq值直接关系了力矩的大小
float Velocity_Openloop(float target_vel)
{
	float Ts = 0.1;
	
	Shaft_angle = Normalize_Angle(Shaft_angle + Ts * target_vel);
	
	float Uq = VOLTAGE_POWER_SUPPLY * 0.3;
	
	SetPhaseVoltage(Uq , 0.0 , Get_Electrical_Degree(Shaft_angle));
	OLED_ShowNum(3, 1, Get_Electrical_Degree(Shaft_angle), 3);
	
	return Uq;
}

