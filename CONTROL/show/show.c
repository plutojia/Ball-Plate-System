#include "show.h"
#include "DataScope_DP.h"
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
/**************************************************************************
�������ܣ�����ʾ��������λ���������� �ر���ʾ��
��ڲ�������
����  ֵ����
��    �ߣ�ƽ��С��֮��
**************************************************************************/
unsigned char i;          //��������
unsigned char Send_Count; //������Ҫ���͵����ݸ���
float Vol;
void DataScope(void)
{   
		DataScope_Get_Channel_Data( sudu1, 1 );       // motor1_PID �ٶ�
	  DataScope_Get_Channel_Data( position1, 2 );          //  motor1_PID λ��
	  DataScope_Get_Channel_Data( position1_PID.Target ,3);   //motor2_PID �ٶ�	
	
		DataScope_Get_Channel_Data( sudu2 ,4);   //motor2_PID �ٶ�	
		DataScope_Get_Channel_Data( position2, 5 );                 //motor2_PID λ��
		DataScope_Get_Channel_Data( position2_PID.Target, 6);                 //motor2_PID λ��
//		DataScope_Get_Channel_Data(0, 5 ); //����Ҫ��ʾ�������滻0������
//		DataScope_Get_Channel_Data(0 , 6 );//����Ҫ��ʾ�������滻0������
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
