#ifndef _MOTOR_H
#define _MOTOR_H
#include "sys.h"

//TIM1 CH1--PA8 CH2--PA11 CH3--PE13 CH4--PE14
/*
���1��	PWM--PE9(TIM1.CH1) 	����ת PE2 PE3   ���PWM����Ϊ��ʱ��ǰһ������Ϊ������Ϊ����Ϊ����֮
���2��	PWM--PE11(TIM1.CH2)	����ת PE4 PE5
���3��	PWM--PE13(TIM1.CH3)	����ת PE6 PE7
���4��	PWM--PE14(TIM1.CH4)	����ת PE8 PE10 
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
//���1 ����ת
#define PE2 PEout(2)	
#define PE3 PEout(3)	
//���2 ����ת
#define PE4 PEout(4)	
#define PE5 PEout(5)	
//���3 ����ת
#define PE6 PEout(6)	
#define PE7 PEout(7)	
//���4 ����ת
#define PE8 PEout(8)	
#define PE10 PEout(10)	
//����
#define PD0  PDout(0)      //PD0
#define PD1  PDout(1)      //PD1
#define PD2  PDout(2)      //PD2
#define PD3  PDout(3)      //PD3
#endif
