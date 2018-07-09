#include "motor.h"
 
//TIM14 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
int MOTOR[4];
int stepPeriod,stepCount,stepState,stepDirection;
//TIM1 CH1--PE9 CH2--PE11 CH3--PE13 CH4--PE14
int lastPWM1,lastPWM2;

void MOTOR_Init(u32 arr,u32 psc)        //TIM1 drive motor
{		 					 
	//�˲������ֶ��޸�IO������
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM1ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��PORTAʱ��	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 	//ʹ��PORTE��

  GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1); //GPIOE9 ����Ϊ��ʱ��1
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1); //GPIOE11 ����Ϊ��ʱ��1
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1); //GPIOE13 ����Ϊ��ʱ��1
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_TIM1); //GPIOE14 ����Ϊ��ʱ��1
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 |GPIO_Pin_11| GPIO_Pin_13 | GPIO_Pin_14;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOE,&GPIO_InitStructure);              //��ʼ��PE
	 
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//��ʼ����ʱ��1
	
	//��ʼ��TIM1 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ե�
	TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Set;

	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 OC1
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 OC2
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 OC3
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 OC4

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIM1��CCR1�ϵ�Ԥװ�ؼĴ���
  TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIM1��CCR2�ϵ�Ԥװ�ؼĴ���
  TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIM1��CCR3�ϵ�Ԥװ�ؼĴ���
  TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIM1��CCR4�ϵ�Ԥװ�ؼĴ���

	MOTOR_otherGPIO();
  TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPEʹ�� 
	
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
 									  
}  
/*
���1��	PWM--PE9(TIM1.CH1) 	����ת PE2 PE3   ���PWM����Ϊ��ʱ��ǰһ������Ϊ������Ϊ����Ϊ����֮
���2��	PWM--PE11(TIM1.CH2)	����ת PE4 PE5
���3��	PWM--PE13(TIM1.CH3)	����ת PE6 PE7
���4��	PWM--PE14(TIM1.CH4)	����ת PE8 PE10 
*/
void MOTOR_otherGPIO(void)         //�����������ڿ��Ƶ������ת������
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOFʱ��

  //GPIOE2,E3,E4,E5,E6,E7,E8,E10��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3|GPIO_Pin_4 | GPIO_Pin_5|GPIO_Pin_6 | GPIO_Pin_7|GPIO_Pin_8| GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��
}
void TIM7_IRQHandler(void) //
{
		if(TIM_GetITStatus(TIM7,TIM_IT_Update)==SET) //����ж�
        {
					stepCount++;
					if(stepCount%stepPeriod==0){
						if(++stepState==4) stepState=0;
						
						switch(stepDirection){
						case 0:							//��ת
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
						case 1:								//��ת
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
					TIM_ClearITPendingBit(TIM7,TIM_IT_Update);  //����жϱ�־λ
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
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��GPIOFʱ��

  //GPIOF6,F7��ʼ�����á�
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);  ///ʹ��TIM3ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = 130-1; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=840-1;  //��ʱ����Ƶ 0.01ms
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM7,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM7_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

int limitPwm(int pwm)			//===PWM������7200 ������7100
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
