#include "SSI.h"

#define SSI_clock PCout(4)
#define SSI_data PCin(5)
#define SSI_CSn PCout(10)

void SSI_Init(void)
{  
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
     
      //SSI_clock初始化PC4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_10;				
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	GPIO_SetBits(GPIOC,GPIO_Pin_4);  
	GPIO_SetBits(GPIOC,GPIO_Pin_10); 
			//SSI_data初始化PC5
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;     //返回电平引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);  
  GPIO_ResetBits(GPIOC,GPIO_Pin_5);  
}

int readSSI(){
	int i,result=0;
	SSI_CSn=0;
	delay_us(20);
	//START
	SSI_clock=0;
	delay_us(5);
	SSI_clock=1;
	delay_us(5);
	//CLCYE
	for(i=0;i<10;i++){
		result=result<<1;
		result+=SSI_data;
		SSI_clock=0;
		delay_us(5);
		SSI_clock=1;
		delay_us(5);	
	}
	for(i=0;i<5;i++){
		SSI_clock=0;
		delay_us(5);
		SSI_clock=1;
		delay_us(5);	
	}
	delay_us(20);	//END
	SSI_CSn=1;
	return result;
}







