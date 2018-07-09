#ifndef _MOTOR_H
#define _MOTOR_H
#include "sys.h"

//TIM1 CH1--PA8 CH2--PA11 CH3--PE13 CH4--PE14
/*
电机1：	PWM--PE9(TIM1.CH1) 	正反转 PE2 PE3   输出PWM变量为正时，前一个引脚为正，后为负，为负反之
电机2：	PWM--PE11(TIM1.CH2)	正反转 PE4 PE5
电机3：	PWM--PE13(TIM1.CH3)	正反转 PE6 PE7
电机4：	PWM--PE14(TIM1.CH4)	正反转 PE8 PE10 
*/
extern int MOTOR[4];
void MOTOR_Init(u32 arr,u32 psc);
void Set_MOTOR1(int pwm);
void Set_MOTOR2(int pwm);
void Set_MOTOR3(int pwm);
void Set_MOTOR4(int pwm);
void Set_MOTOR(void);
void MOTOR_otherGPIO(void);
void MOTOR_step_Init(void);
void MOTOR_step(void);
void set_step(int sudo);
#define MOTOR1   TIM1->CCR1
#define MOTOR2   TIM1->CCR2
#define MOTOR3   TIM1->CCR3
#define MOTOR4   TIM1->CCR4
//电机1 正反转
#define PE2 PEout(2)	
#define PE3 PEout(3)	
//电机2 正反转
#define PE4 PEout(4)	
#define PE5 PEout(5)	
//电机3 正反转
#define PE6 PEout(6)	
#define PE7 PEout(7)	
//电机4 正反转
#define PE8 PEout(8)	
#define PE10 PEout(10)	
//步进
#define PD0  PDout(0)      //PD0
#define PD1  PDout(1)      //PD1
#define PD2  PDout(2)      //PD2
#define PD3  PDout(3)      //PD3
#endif
