#include "sys.h"

u8 tmp_buf[33];						//wireless
char cx,cy;
int x,y,dx,dy;
int sudu1,sudu2;
long position1,position2;
int pianzhi;
int USART_flag;
int handle_flag;
point coordinate[9]={
	{-93,-80},{1,-78},{88,-80},
	{-90,6},{3,6},{88,6},
	{-83,90},{3,90},{88,90}
};


void myInit(){
	USART_flag=1;
	mode=NOPWM;
	cx=78;
	cy=16;
	position1=100000;
	position2=100000;
	position1_PID.Target=100000;
	position2_PID.Target=100000;
	pianzhi=650;
	handle_flag=0;
}
int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ500000
	uart3_init(115200);
	LED_Init();					//��ʼ��LED 
	KEY_Init();					//��ʼ������
	MOTOR_Init(7200-1,0);
	Encoder_Init_TIM2();
	Encoder_Init_TIM4(); 
	LCD_Init();
	myInit();

	//��ʾ��

  display(0x82,"����ϵͳ");
	display(0x90,"X:");display(0x94,"Y:");
	display(0x88,"��ʱ��");display(0x8D,"FPS:");
	display(0x98,"ģʽ");
	
	
	TIM3_Int_Init(50-1,8400-1);  //����ʱ�ӣ�����������ʱ��ʱ��84M����Ƶϵ��8400������84M/8400=10Khz�ļ���Ƶ�ʣ�����50��Ϊ5ms 
		
		while(1)
	{
		DataScope();
						//printf("x:%d\n",x);
						//printf("y:%d\n",y);
		delay_ms(50);
		
	}
	
}
