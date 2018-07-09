#include "helpers.h"

int shift(int x,int min,int max){
	if(x-max>0) x=min+x-max;
	if(x-min<0) x=max+x-min;
	return x;
}

int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
void inttostr(unsigned char *str,int a,int n){
	int shang,yu;
	str[n]=0;
	for(;n>0;){
		n--;
		shang=a/10;
		yu=a%10;
		str[n]='0'+yu;
		a=shang;
	}
	
}
void chtostr(unsigned char *str,char a,int n,int sign){
	char shang,yu=0;
	
	if(sign==0){					//ÎŞ·ûºÅ
		str[n]=0;
		for(;n>0;){
			n--;
			shang=a/10;
			yu=a%10;
			str[n]='0'+yu;
			a=shang;
		}
	}
	if(sign==1){					//ÓĞ·ûºÅ
		
		if(a>127){
		sign=0;
		a=256-a;
		}
		else sign=1;
		
		str[n]=0;
		for(;n>1;){
			n--;
			shang=a/10;
			yu=a%10;
			str[n]='0'+yu;
			a=shang;
		}
		
		if(sign==0)str[0]='-';
		else str[0]='+';
			
	}
	
}

