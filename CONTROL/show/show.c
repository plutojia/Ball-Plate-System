#include "show.h"
#include "DataScope_DP.h"
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
/**************************************************************************
函数功能：虚拟示波器往上位机发送数据 关闭显示屏
入口参数：无
返回  值：无
作    者：平衡小车之家
**************************************************************************/
unsigned char i;          //计数变量
unsigned char Send_Count; //串口需要发送的数据个数
float Vol;
void DataScope(void)
{   
		DataScope_Get_Channel_Data( sudu1, 1 );       // motor1_PID 速度
	  DataScope_Get_Channel_Data( position1, 2 );          //  motor1_PID 位置
	  DataScope_Get_Channel_Data( position1_PID.Target ,3);   //motor2_PID 速度	
	
		DataScope_Get_Channel_Data( sudu2 ,4);   //motor2_PID 速度	
		DataScope_Get_Channel_Data( position2, 5 );                 //motor2_PID 位置
		DataScope_Get_Channel_Data( position2_PID.Target, 6);                 //motor2_PID 位置
//		DataScope_Get_Channel_Data(0, 5 ); //用您要显示的数据替换0就行了
//		DataScope_Get_Channel_Data(0 , 6 );//用您要显示的数据替换0就行了
//		DataScope_Get_Channel_Data(0, 7 );
//		DataScope_Get_Channel_Data( 0, 8 ); 
//		DataScope_Get_Channel_Data(0, 9 );  
//		DataScope_Get_Channel_Data( 0 , 10);
		Send_Count = DataScope_Data_Generate(6);
		for( i = 0 ; i < Send_Count; i++) 
		{
		while((USART1->SR&0X40)==0);  
		USART1->DR = DataScope_OutPut_Buffer[i]; 
		}
}
