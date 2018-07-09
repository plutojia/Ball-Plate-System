#include "12864.h"


void delay(unsigned int n) 
{
	unsigned int i;
	for(i=0; i<n; i++) {;}
}
//���з���һ�ֽ�����
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
//���н���һ�ֽ�����
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
////��ȡ��־λBF

//unsigned char ReadBF(unsigned char bf)
//{
//	unsigned char dat;
//	SendByte(0xFA);//11111,01,0 RW=1,RS=0
//	dat=ReceieveByte();
//	if(dat>0x7F)bf=1;else bf=0; //����7Fæ��
//	return bf;
//}      
//д��������

void SendCMD(unsigned char dat)
{
//      while(ReadBF){;}
	RS_H;
	SendByte(0xF8);//11111,00,0 RW=0,RS=0   ͬ����־
	SendByte(dat&0xF0);//����λ
	SendByte((dat&0x0F)<<4);//����λ
	RS_L;
}

//д��ʾ���ݻ��ֽ��ַ�
void SendDat(unsigned char dat)
{
//      while(ReadBF){;}
	RS_H;
	SendByte(0xFA);//11111,01,0 RW=0,RS=1
	SendByte(dat&0xF0);//����λ
	SendByte((dat&0x0F)<<4);//����λ
	RS_L;
} 

/*      д���ֵ�LCD ָ����λ��
     x_add��ʾRAM�ĵ�ַ
     dat1/dat2��ʾ���ֱ���
*/
//void display(unsigned char x_add,unsigned char dat1,unsigned char dat2)
//{
//     SendCMD(x_add);//1xxx,xxxx �趨DDRAM 7λ��ַxxx,xxxx����ַ������AC
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
	SendCMD(x_add);//1xxx,xxxx �趨DDRAM 7λ��ַxxx,xxxx����ַ������AC
	while(*ptr != '\0')
	{
		SendDat(*ptr);
		++ptr;
	}
}
//��ʼ�� LCM
void LCD_Init(void)
{
	LCD_GPIO_Configuration();
	
  RST=0;
	RS_L;
	//      PSB=0;//serial mode
	delay_ms(100);
	RST=1;
	delay_ms(150);
	SendCMD(0x30);//�������ã�һ����8λ���ݣ�����ָ�
	SendCMD(0x0C);//0000,1100 ������ʾ���α�off���α�λ��off 
	SendCMD(0x01);//0000,0001 ��DDRAM
	SendCMD(0x02);//0000,0010 DDRAM��ַ��λ
	SendCMD(0x80);//1000,0000 �趨DDRAM 7λ��ַ000��0000����ַ������AC
//      SendCMD(0x04);//���趨����ʾ�ַ�/����������λ��DDRAM��ַ�� һ
//      SendCMD(0x0F);//��ʾ�趨������ʾ����ʾ��꣬��ǰ��ʾλ��������
	delay_ms(300);
}
void LCD_GPIO_Configuration(void) 
{ 
	
	 GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOFʱ��

  //GPIOC6,C7,C8��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��
	
}

