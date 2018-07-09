#include "12864.h"


void delay(unsigned int n) 
{
	unsigned int i;
	for(i=0; i<n; i++) {;}
}
//串行发送一字节数据
void SendByte(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		EN_L;
		if(dat&0x80) 
			RW_H;
		else RW_L;
		delay(50);
		EN_H;
		dat=dat<<1;
		delay(50);
	}
}
//串行接收一字节数据
//unsigned char ReceieveByte(void)
//{
//	unsigned char i,d1,d2;
//	for(i=0;i<8;i++)
//	{
//		EN_L;
//		delay(100);
//		EN_H;
//		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7))d1++;
//		d1=d1<<1;
//	}
//	for(i=0;i<8;i++)
//	{
//		EN_L;
//		delay(100);
//		EN_H;
//		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7))d2++;
//		d2=d2<<1;
//	}
//	return (d1&0xF0+d2&0x0F);
//}
////读取标志位BF

//unsigned char ReadBF(unsigned char bf)
//{
//	unsigned char dat;
//	SendByte(0xFA);//11111,01,0 RW=1,RS=0
//	dat=ReceieveByte();
//	if(dat>0x7F)bf=1;else bf=0; //大于7F忙。
//	return bf;
//}      
//写控制命令

void SendCMD(unsigned char dat)
{
//      while(ReadBF){;}
	RS_H;
	SendByte(0xF8);//11111,00,0 RW=0,RS=0   同步标志
	SendByte(dat&0xF0);//高四位
	SendByte((dat&0x0F)<<4);//低四位
	RS_L;
}

//写显示数据或单字节字符
void SendDat(unsigned char dat)
{
//      while(ReadBF){;}
	RS_H;
	SendByte(0xFA);//11111,01,0 RW=0,RS=1
	SendByte(dat&0xF0);//高四位
	SendByte((dat&0x0F)<<4);//低四位
	RS_L;
} 

/*      写汉字到LCD 指定的位置
     x_add显示RAM的地址
     dat1/dat2显示汉字编码
*/
//void display(unsigned char x_add,unsigned char dat1,unsigned char dat2)
//{
//     SendCMD(x_add);//1xxx,xxxx 设定DDRAM 7位地址xxx,xxxx到地址计数器AC
//     SendDat(dat1);
//     SendDat(dat2);
//}

//void PutStr(unsigned char x, unsigned char y, unsigned char *ptr)
//{
// WriteCommand(0x00,0xCD);
// LocateXY(x,y);
// while(*ptr != '\0')
// {
//   WriteData(*ptr);
//   ++ptr;
// } 
//}

void display(unsigned char x_add,unsigned char *ptr)
{
	SendCMD(x_add);//1xxx,xxxx 设定DDRAM 7位地址xxx,xxxx到地址计数器AC
	while(*ptr != '\0')
	{
		SendDat(*ptr);
		++ptr;
	}
}
//初始化 LCM
void LCD_Init(void)
{
	LCD_GPIO_Configuration();
	
  RST=0;
	RS_L;
	//      PSB=0;//serial mode
	delay_ms(100);
	RST=1;
	delay_ms(150);
	SendCMD(0x30);//功能设置，一次送8位数据，基本指令集
	SendCMD(0x0C);//0000,1100 整体显示，游标off，游标位置off 
	SendCMD(0x01);//0000,0001 清DDRAM
	SendCMD(0x02);//0000,0010 DDRAM地址归位
	SendCMD(0x80);//1000,0000 设定DDRAM 7位地址000，0000到地址计数器AC
//      SendCMD(0x04);//点设定，显示字符/光标从左到右移位，DDRAM地址加 一
//      SendCMD(0x0F);//显示设定，开显示，显示光标，当前显示位反白闪动
	delay_ms(300);
}
void LCD_GPIO_Configuration(void) 
{ 
	
	 GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOF时钟

  //GPIOC6,C7,C8初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化
	
}

