#ifndef __PID_H
#define __PID_H	

#include "sys.h"

typedef struct PID_Struct{
	float KP,KI,KD;
	float Present,Target;
	float Bias,Integral_bias,Last_Bias,Last_Bias2;
	float max,min,maxBias,minBias;
	int output;
} PID;

extern PID position1_PID,position2_PID,xPID,yPID,dxPID,dyPID;
extern int Incremental_PI (PID *mPID); //����ʽ
extern int Position_PID (PID *mPID);	//λ��ʽ
extern int IS_PID(PID *mPID);		//���ַ���λ��
extern int AWIS_PID(PID *mPID);	//�����ͻ��ַ���λ��
extern int VI_PID(PID *mPID);		//�����λ��
extern void clearPID(PID *mPID);
extern void print_pid(PID mPID);
#endif
