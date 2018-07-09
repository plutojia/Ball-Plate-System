#include "ultrasonic.h"
//超声波测距

 
#define HCSR04_PORT     GPIOB
#define HCSR04_CLK      RCC_APB2Periph_GPIOB
#define HCSR04_TRIG     GPIO_Pin_3
#define HCSR04_ECHO     GPIO_Pin_5



u16 msHcCount = 0;//ms计数

void Hcsr04Init(void)
{  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;     //生成用于定时器设置的结构体
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
     
        //IO初始化
	
	GPIO_InitStructure.GPIO_Pin = HCSR04_TRIG;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	 GPIO_ResetBits(HCSR04_PORT,HCSR04_TRIG);  
	
    GPIO_InitStructure.GPIO_Pin =  HCSR04_ECHO;     //返回电平引脚
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);  
        GPIO_ResetBits(HCSR04_PORT,HCSR04_ECHO);    
     
  //定时器初始化 使用基本定时器TIM2
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  ///使能TIM2时钟
	
	TIM_TimeBaseInitStructure.TIM_Period = 1000-1; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=84-1;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM5,ENABLE); //使能定时器5
  TIM_ClearFlag(TIM5, TIM_FLAG_Update);   //清除更新中断，免得一打开中断立即产生中断
  TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);    //打开定时器更新中断
  hcsr04_NVIC();
  TIM_Cmd(TIM5,DISABLE);     
}


//tips：static函数的作用域仅限于定义它的源文件内，所以不需要在头文件里声明
static void OpenTimerForHc(void)        //打开定时器
{
        TIM5->CNT = 0; //清除计数
        msHcCount = 0;
        TIM_Cmd(TIM5, ENABLE);  //使能TIMx外设
}
 
static void CloseTimerForHc(void)        //关闭定时器
{
        TIM_Cmd(TIM5, DISABLE);  //使能TIMx外设
}
 
 
 //NVIC配置
void hcsr04_NVIC(void)
{
            NVIC_InitTypeDef NVIC_InitStructure;
	
            NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn; //定时器3中断
						NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //抢占优先级1
						NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; //子优先级3
						NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
						NVIC_Init(&NVIC_InitStructure);
}


//定时器2中断服务程序
void TIM5_IRQHandler(void) //
{
		if(TIM_GetITStatus(TIM5,TIM_IT_Update)==SET) //溢出中断
        {
                msHcCount++;
					TIM_ClearITPendingBit(TIM5,TIM_IT_Update);  //清除中断标志位
        }
}
 

//获取定时器时间
u32 GetEchoTimer(void)
{
        u32 t = 0;
        t = msHcCount*1000;//得到MS
        t += TIM5->CNT; ;//得到US
          TIM2->CNT = 0;  //将TIM2计数寄存器的计数值清零
                //delay_ms(50);
        return t;
}
 

//一次获取超声波测距数据 两次测距之间需要相隔一段时间，隔断回响信号
//为了消除余震的影响，取五次数据的平均值进行加权滤波。
//单位厘米，读取时间太长会被判定无效，返回-1，最多等64CM
float Hcsr04GetLength(void )			//cm
{
        u32 t = 0;
        int i = 0;int deadline,state=0;
        float lengthTemp = 0;
        float sum = 0;
        while(i!=1)
        {
        TRIG_Send = 1;      //发送口高电平输出
        delay_us(21);
        TRIG_Send = 0;
				deadline=0;
        while(ECHO_Reci == 0){
						if(++deadline>5000)
							return	-1;						//overtime
				};      //等待接收口高电平输出
            OpenTimerForHc();        //打开定时器
            i = i + 1;
						deadline=0;
            while(ECHO_Reci == 1){
							if(++deadline>40000){				//最多等64CM左右的时间
								state=1;
							break;
							}
						};
            CloseTimerForHc();        //关闭定时器
						if(state!=0) return -1;			//overtime
            t = GetEchoTimer();        //获取时间,分辨率为1US
            lengthTemp = ((float)t/58.0f);//cm
            sum = lengthTemp + sum ;
    }
        lengthTemp = sum/1.0f;
        return lengthTemp;
}


