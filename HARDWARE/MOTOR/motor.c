#include "motor.h"
 
//TIM14 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
int MOTOR[4];
int stepPeriod,stepCount,stepState,stepDirection;
//TIM1 CH1--PE9 CH2--PE11 CH3--PE13 CH4--PE14
int lastPWM1,lastPWM2;

void MOTOR_Init(u32 arr,u32 psc)        //TIM1 drive motor
{		 					 
	//此部分需手动修改IO口设置
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM1时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTA时钟	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 	//使能PORTE钟

  GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1); //GPIOE9 复用为定时器1
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1); //GPIOE11 复用为定时器1
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1); //GPIOE13 复用为定时器1
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_TIM1); //GPIOE14 复用为定时器1
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 |GPIO_Pin_11| GPIO_Pin_13 | GPIO_Pin_14;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOE,&GPIO_InitStructure);              //初始化PE
	 
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//初始化定时器1
	
	//初始化TIM1 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性低
	TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Set;

	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC1
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC2
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC3
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC4

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR1上的预装载寄存器
  TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR2上的预装载寄存器
  TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR3上的预装载寄存器
  TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR4上的预装载寄存器

	MOTOR_otherGPIO();
  TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM1, ENABLE);  //使能TIM1
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
 									  
}  
/*
电机1：	PWM--PE9(TIM1.CH1) 	正反转 PE2 PE3   输出PWM变量为正时，前一个引脚为正，后为负，为负反之
电机2：	PWM--PE11(TIM1.CH2)	正反转 PE4 PE5
电机3：	PWM--PE13(TIM1.CH3)	正反转 PE6 PE7
电机4：	PWM--PE14(TIM1.CH4)	正反转 PE8 PE10 
*/
void MOTOR_otherGPIO(void)         //配置其他用于控制电机正反转的引脚
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOF时钟

  //GPIOE2,E3,E4,E5,E6,E7,E8,E10初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3|GPIO_Pin_4 | GPIO_Pin_5|GPIO_Pin_6 | GPIO_Pin_7|GPIO_Pin_8| GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化
}
void TIM7_IRQHandler(void) //
{
		if(TIM_GetITStatus(TIM7,TIM_IT_Update)==SET) //溢出中断
        {
					stepCount++;
					if(stepCount%stepPeriod==0){
						if(++stepState==4) stepState=0;
						
						switch(stepDirection){
						case 0:							//正转
							switch(stepState){
								case 0:
									PD0=1;PD1=0;PD2=0;PD3=0;
								break;
								case 1:
									PD0=0;PD1=0;PD2=1;PD3=0;
								break;
								case 2:
									PD0=0;PD1=1;PD2=0;PD3=0;
								break;
								case 3:
									PD0=0;PD1=0;PD2=0;PD3=1;
								break;								
							}
						break;
						case 1:								//反转
							switch(stepState){
								case 0:
									PD0=0;PD1=0;PD2=1;PD3=0;
								break;
								case 1:
									PD0=1;PD1=0;PD2=0;PD3=0;
								break;
								case 2:
									PD0=0;PD1=0;PD2=0;PD3=1;
								break;
								case 3:
									PD0=0;PD1=1;PD2=0;PD3=0;
								break;								
							}
						break;
						}
					}
					TIM_ClearITPendingBit(TIM7,TIM_IT_Update);  //清除中断标志位
        }
}

void set_step(int sudo){
	if(sudo>0) stepDirection=0;
	else stepDirection=1;
	stepPeriod=myabs(1.0/sudo*7200.0);
	
}
void MOTOR_step_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//使能GPIOF时钟

  //GPIOF6,F7初始化设置、
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);  ///使能TIM3时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = 130-1; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=840-1;  //定时器分频 0.01ms
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM7,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM7_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

int limitPwm(int pwm)			//===PWM满幅是7200 限制在7100
{		
	  int Amplitude=3600;   
		if(pwm<-Amplitude-pianzhi) pwm=-Amplitude-pianzhi;	
		if(pwm>Amplitude-pianzhi)  pwm=Amplitude-pianzhi;	
		return pwm;
}

void Set_MOTOR1(int pwm)
{
	pwm=limitPwm(pwm);
	if(pwm*lastPWM1<0){
		pwm=(pwm+lastPWM1)/2.0;
	}
	lastPWM1=pwm;
	if(pwm>0)		PE2=1,PE3=0;
	else 	      PE2=0,PE3=1;
	MOTOR1=myabs(pwm);
}
void Set_MOTOR2(int pwm)
{
	pwm=limitPwm(pwm);
	if(pwm*lastPWM2<0){
		pwm=(pwm+lastPWM2)/2.0;
	}
	lastPWM2=pwm;
	if(pwm>0)		PE4=1,PE5=0;
	else 	      PE4=0,PE5=1;
	MOTOR2=myabs(pwm);
}
void Set_MOTOR3(int pwm)
{
	pwm=limitPwm(pwm);
	if(pwm>0)		PE6=1,PE7=0;
	else 	      PE6=0,PE7=1;
	MOTOR3=myabs(pwm);
}
void Set_MOTOR4(int pwm)
{
	pwm=limitPwm(pwm);
	if(pwm>0)		PE8=1,PE10=0;
	else 	      PE8=0,PE10=1;
	MOTOR4=myabs(pwm);
}
void Set_MOTOR(void)
{
	Set_MOTOR1(MOTOR[0]);
	Set_MOTOR2(MOTOR[1]);
	Set_MOTOR3(MOTOR[2]);
	Set_MOTOR4(MOTOR[3]);
}
void printMOTOR(){

}
