#include "ultrasonic.h"
//���������

 
#define HCSR04_PORT     GPIOB
#define HCSR04_CLK      RCC_APB2Periph_GPIOB
#define HCSR04_TRIG     GPIO_Pin_3
#define HCSR04_ECHO     GPIO_Pin_5



u16 msHcCount = 0;//ms����

void Hcsr04Init(void)
{  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;     //�������ڶ�ʱ�����õĽṹ��
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
     
        //IO��ʼ��
	
	GPIO_InitStructure.GPIO_Pin = HCSR04_TRIG;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	 GPIO_ResetBits(HCSR04_PORT,HCSR04_TRIG);  
	
    GPIO_InitStructure.GPIO_Pin =  HCSR04_ECHO;     //���ص�ƽ����
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);  
        GPIO_ResetBits(HCSR04_PORT,HCSR04_ECHO);    
     
  //��ʱ����ʼ�� ʹ�û�����ʱ��TIM2
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  ///ʹ��TIM2ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Period = 1000-1; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=84-1;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM5,ENABLE); //ʹ�ܶ�ʱ��5
  TIM_ClearFlag(TIM5, TIM_FLAG_Update);   //��������жϣ����һ���ж����������ж�
  TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);    //�򿪶�ʱ�������ж�
  hcsr04_NVIC();
  TIM_Cmd(TIM5,DISABLE);     
}


//tips��static����������������ڶ�������Դ�ļ��ڣ����Բ���Ҫ��ͷ�ļ�������
static void OpenTimerForHc(void)        //�򿪶�ʱ��
{
        TIM5->CNT = 0; //�������
        msHcCount = 0;
        TIM_Cmd(TIM5, ENABLE);  //ʹ��TIMx����
}
 
static void CloseTimerForHc(void)        //�رն�ʱ��
{
        TIM_Cmd(TIM5, DISABLE);  //ʹ��TIMx����
}
 
 
 //NVIC����
void hcsr04_NVIC(void)
{
            NVIC_InitTypeDef NVIC_InitStructure;
	
            NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn; //��ʱ��3�ж�
						NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //��ռ���ȼ�1
						NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; //�����ȼ�3
						NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
						NVIC_Init(&NVIC_InitStructure);
}


//��ʱ��2�жϷ������
void TIM5_IRQHandler(void) //
{
		if(TIM_GetITStatus(TIM5,TIM_IT_Update)==SET) //����ж�
        {
                msHcCount++;
					TIM_ClearITPendingBit(TIM5,TIM_IT_Update);  //����жϱ�־λ
        }
}
 

//��ȡ��ʱ��ʱ��
u32 GetEchoTimer(void)
{
        u32 t = 0;
        t = msHcCount*1000;//�õ�MS
        t += TIM5->CNT; ;//�õ�US
          TIM2->CNT = 0;  //��TIM2�����Ĵ����ļ���ֵ����
                //delay_ms(50);
        return t;
}
 

//һ�λ�ȡ������������� ���β��֮����Ҫ���һ��ʱ�䣬���ϻ����ź�
//Ϊ�����������Ӱ�죬ȡ������ݵ�ƽ��ֵ���м�Ȩ�˲���
//��λ���ף���ȡʱ��̫���ᱻ�ж���Ч������-1������64CM
float Hcsr04GetLength(void )			//cm
{
        u32 t = 0;
        int i = 0;int deadline,state=0;
        float lengthTemp = 0;
        float sum = 0;
        while(i!=1)
        {
        TRIG_Send = 1;      //���Ϳڸߵ�ƽ���
        delay_us(21);
        TRIG_Send = 0;
				deadline=0;
        while(ECHO_Reci == 0){
						if(++deadline>5000)
							return	-1;						//overtime
				};      //�ȴ����տڸߵ�ƽ���
            OpenTimerForHc();        //�򿪶�ʱ��
            i = i + 1;
						deadline=0;
            while(ECHO_Reci == 1){
							if(++deadline>40000){				//����64CM���ҵ�ʱ��
								state=1;
							break;
							}
						};
            CloseTimerForHc();        //�رն�ʱ��
						if(state!=0) return -1;			//overtime
            t = GetEchoTimer();        //��ȡʱ��,�ֱ���Ϊ1US
            lengthTemp = ((float)t/58.0f);//cm
            sum = lengthTemp + sum ;
    }
        lengthTemp = sum/1.0f;
        return lengthTemp;
}


