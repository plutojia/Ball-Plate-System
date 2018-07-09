#ifndef __12864_H
#define	__12864_H

#include "sys.h"

/*LCD 12864*/
#define RS_H GPIO_SetBits(GPIOC,GPIO_Pin_8) //RS=1   8PIN
#define RS_L GPIO_ResetBits(GPIOC,GPIO_Pin_8) //RS=0

#define RW_H GPIO_SetBits(GPIOC,GPIO_Pin_7) //RW=1    7PIN
#define RW_L GPIO_ResetBits(GPIOC,GPIO_Pin_7);  //RW = 0

#define EN_H GPIO_SetBits(GPIOC,GPIO_Pin_6);    //EN = 1   6PIN
#define EN_L GPIO_ResetBits(GPIOC,GPIO_Pin_6);  //EN = 0
#define RST	PCout(9)
void LCD_GPIO_Configuration(void) ;
void LCD_Init(void);
void display(unsigned char x_add,unsigned char *ptr);
void SendCMD(unsigned char dat);
void SendByte(unsigned char dat);
void SendDat(unsigned char dat);

#endif
