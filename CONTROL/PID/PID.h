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
extern int Incremental_PI (PID *mPID); //增量式
extern int Position_PID (PID *mPID);	//位置式
extern int IS_PID(PID *mPID);		//积分分离位置
extern int AWIS_PID(PID *mPID);	//抗饱和积分分离位置
extern int VI_PID(PID *mPID);		//变积分位置
extern void clearPID(PID *mPID);
extern void print_pid(PID mPID);
#endif
