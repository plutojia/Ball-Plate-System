#include "PID.h"

PID position1_PID,position2_PID,xPID,yPID,dxPID,dyPID;
int Incremental_PI (PID *mPID)
{ 	
	 mPID->Bias=mPID->Target-mPID->Present;                //����ƫ��
	 mPID->output+=mPID->KP*(mPID->Bias-mPID->Last_Bias)+mPID->KI*mPID->Bias+mPID->KD*(mPID->Bias-2*mPID->Last_Bias+mPID->Last_Bias2);   //����ʽPI������
	 mPID->Last_Bias2=mPID->Last_Bias;
	 mPID->Last_Bias=mPID->Bias;	                   //������һ��ƫ�� 
	 return mPID->output;                         //�������
}
int Position_PID (PID *mPID)
{ 	
	 mPID->Bias=mPID->Target-mPID->Present;                                  //����ƫ��
	 mPID->Integral_bias+=mPID->Bias;	                                 //���ƫ��Ļ���
	 mPID->output=mPID->KP*mPID->Bias+mPID->KI*mPID->Integral_bias+mPID->KD*(mPID->Bias-mPID->Last_Bias);       //λ��ʽPID������
	 mPID->Last_Bias=mPID->Bias;                                       //������һ��ƫ�� 
	 return mPID->output;                                           //�������
}
int IS_PID(PID *mPID)//integral separate Position_PID
{
	int index=0;
	mPID->Bias=mPID->Target-mPID->Present;
	if(myabs(mPID->Bias)<mPID->maxBias) {
		index=1;
	 mPID->Integral_bias+=mPID->Bias;
	}
	mPID->output=mPID->KP*mPID->Bias+index*mPID->KI*mPID->Integral_bias+mPID->KD*(mPID->Bias-mPID->Last_Bias);       //λ��ʽPID������
	mPID->Last_Bias=mPID->Bias;                                       //������һ��ƫ�� 
	return mPID->output; 
}
int AWIS_PID(PID *mPID)//anti-windup integral separate Position_PID
{
	int index=0;
	mPID->Bias=mPID->Target-mPID->Present;
	if(mPID->Present>mPID->max){
		if(myabs(mPID->Bias)<mPID->maxBias) {
			index=1;
			if(mPID->Bias<0)
				mPID->Integral_bias+=mPID->Bias;
		}
	}else if(mPID->Present<mPID->min){
		if(myabs(mPID->Bias)<mPID->maxBias) {
			index=1;
			if(mPID->Bias>0)
				mPID->Integral_bias+=mPID->Bias;
		}
	}else if(myabs(mPID->Bias)<mPID->maxBias) {
		index=1;
		mPID->Integral_bias+=mPID->Bias;
	}
	
	mPID->output=mPID->KP*mPID->Bias+index*mPID->KI*mPID->Integral_bias+mPID->KD*(mPID->Bias-mPID->Last_Bias);       //λ��ʽPID������
	mPID->Last_Bias=mPID->Bias;                                       //������һ��ƫ�� 
	return mPID->output; 
}

int VI_PID(PID *mPID)// variable integral Position_PID
{
	float index=0;
	mPID->Bias=mPID->Target-mPID->Present;
	if(myabs(mPID->Bias)>mPID->maxBias) {
		index=0.0;
	}else if(myabs(mPID->Bias)<mPID->minBias){
		index=1;
		mPID->Integral_bias+=mPID->Bias;
	}else{
		index=(mPID->maxBias-myabs(mPID->Bias))/(mPID->maxBias-mPID->minBias);
		mPID->Integral_bias+=mPID->Bias;
	}
	mPID->output=mPID->KP*mPID->Bias+index*mPID->KI*mPID->Integral_bias+mPID->KD*(mPID->Bias-mPID->Last_Bias);       //λ��ʽPID������
	mPID->Last_Bias=mPID->Bias;                                       //������һ��ƫ�� 
	return mPID->output; 
}
void clearPID(PID *mPID){
	mPID->Bias=0;
	mPID->Integral_bias=0;
	mPID->Last_Bias=0;
}
void print_pid(PID mPID){
	printf("mPID.output:%d,Bias:%f,Integral_bias:%f,Last_Bias:%f,Encoder:%f,Target:%f",mPID.output,mPID.Bias,mPID.Integral_bias,mPID.Last_Bias,mPID.Present,mPID.Target);
}
