引脚说明：
指示灯：D2(LED0)--PA6 	指示正在运行，正常情况亮灭一次共1秒。
	D3(LED1)--PA7 	未用

按键：	KEY0--PD9 .
	KEY1--PD10 .
	KEY2--PD11.
	KEY3--PD12.
	WK_UP--PA0

定时器：TIM1--电机(4CH)	TIM5-超声波 TIM3--控制	TIM2&TIM4--编码器

电机1：	PWM--PE9(TIM1.CH1) 	正反转 PE2 PE3	输出PWM变量为正时，前一个引脚为正，后为负，为负反之
电机2：	PWM--PE11(TIM1.CH2)	正反转 PE4 PE5
电机3：	PWM--PE13(TIM1.CH3)	正反转 PE6 PE7
电机4：	PWM--PE14(TIM1.CH4)	正反转 PE8 PE10


编码器：开了两路 应该是够用了(增量式编码器）
	ENCODER1---TIM2	  CH1--PA15	CH2--PB3
	ENCODER2---TIM4	  CH1--PB6 	CH2--PB7

usart3: TX--PB10
	RX--PB11

屏幕：	12864屏幕
	RS(4)--PC8	GND(1)--0V		PSB(15)--GND	低电平表示串口方式
	RW(5)--PC7	VCC(2)--3.3V		BLA(19)--VCC	背光源电压高
	EN(6)--PC6	VD(3)--0-3.3V亮度	BLK(20)--GND	背光源电压低
	RST(17)--PC9	重置

		
			
					
					
					