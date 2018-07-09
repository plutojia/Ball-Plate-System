#include "control.h"
int Pcount,Pcount2=5;
int Pstate;
int time,timeflag,framecount,fps,lastframe;
point dingwei[50];
int dingcount;
unsigned char str[20];
int shunxu[9],seek,current;
unsigned char *modestr[]={
	"STOPMODE    ",
	"NOPWM       ",
	"SUBJECT1    ",
	"SUBJECT2    ",
	"SUBJECT3    ",
	"SUBJECT4    ",
	"SUBJECT5    ",
	"SUBJECT6    ",
	"SUBJECT7    ",
	"DINGWEI     "
};
u8 key;

void TIM3_IRQHandler(void) //控制定时器3中断服务函数 5ms
{
	float dingjunx,dingjuny;
	int i,dingwai,deltax,deltay;
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		Led_Flash(0,100);				//LED0闪烁
		
		sudu1=Read_Encoder(2);
		position1+=sudu1;
		
		sudu2=Read_Encoder(4);
		position2+=sudu2;

		position1_PID.Present=position1;
		position2_PID.Present=position2;
		
		switch(mode){
		case STOPMODE:  //上为正

			position1_PID.KP=0.4;             //
			position1_PID.KI=0;             //
			position1_PID.KD=10;            //
			position1_PID.Target=100000;
		
			position2_PID.KP=0.4;             //
			position2_PID.KI=0;             //
			position2_PID.KD=10;            //
			position2_PID.Target=100000;
		
			Position_PID(&position1_PID);    
			Position_PID(&position2_PID); 
			Set_MOTOR1(position1_PID.output-pianzhi);
			Set_MOTOR2(position2_PID.output-pianzhi);			
 			Led_Flash(1,50);
		break;
		case NOPWM:   //上为正
			Set_MOTOR1(0-pianzhi);     //上为正
			Set_MOTOR2(0-pianzhi);		
		break;
		case DINGWEI:   //!!!!!!!!!!!!!!!!!!!!!!
			
		if(handle_flag==1){
				
				xPID.Target=coordinate[4].x;
				xPID.Present=x;
				xPID.minBias=20;
				xPID.maxBias=70;
				xPID.KP=220;
				xPID.KI=0;
				xPID.KD=3000;

				yPID.Target=coordinate[4].y;	
				yPID.Present=y;
				yPID.minBias=20;
				yPID.maxBias=70;	
				yPID.KP=220;
				yPID.KI=0;
				yPID.KD=3000;
				
				VI_PID(&xPID);    
				VI_PID(&yPID);
			
			
			
			if(Pcount2-dingcount>6000){
				dingjunx=0;
				dingjuny=0;
				for(i=0;i<50;i++){
				dingjunx+=dingwei[i].x;
				}
				dingjunx/=50.0f;
				for(i=0;i<50;i++){
				dingjuny+=dingwei[i].y;
				}
				dingjuny/=50.0f;
				for(i=0,dingwai=0;i<50||dingwai<10;i++){
					if(dingwei[i].x*dingwei[i].x+dingwei[i].y*dingwei[i].y>81) dingwai++;
				}	
				if(dingwai<6){
					deltax=dingjunx;
					deltay=dingjuny;
					for(i=0;i<9;i++){
						coordinate[i].x-=deltax;
						coordinate[i].y-=deltay;
					}
				}
				
			}
			
			}
			position1_PID.KP=0.4;             //
			position1_PID.KI=0;             //
			position1_PID.KD=10;        
			position1_PID.Target=100000-yPID.output;   //1-y
		
			position2_PID.KP=0.4;             //
			position2_PID.KI=0;             //
			position2_PID.KD=10;            //
			position2_PID.Target=100000+xPID.output;  //2-x
		
			Position_PID(&position1_PID);    
			Position_PID(&position2_PID); 
			Set_MOTOR1(position1_PID.output-pianzhi);
			Set_MOTOR2(position2_PID.output-pianzhi);

		
		break;
		
		
		case SUBJECT1:    //2
			
			if(handle_flag==1){
				xPID.Target=coordinate[1].x;
				xPID.Present=x;
				xPID.minBias=20;
				xPID.maxBias=70;
				
				yPID.Target=coordinate[1].y;	
				yPID.Present=y;
				yPID.minBias=20;
				yPID.maxBias=70;	
				
				xPID.KP=50;
				xPID.KI=5;
				xPID.KD=50;
				
				yPID.KP=50;
				yPID.KI=5;
				yPID.KD=50;
								
				VI_PID(&xPID);    
				VI_PID(&yPID);
				}

				
			position1_PID.KP=0.4;             //
			position1_PID.KI=0;             //
			position1_PID.KD=10;        
			position1_PID.Target=100000-yPID.output;   //1-y
		
			position2_PID.KP=0.4;             //
			position2_PID.KI=0;             //
			position2_PID.KD=10;            //
			position2_PID.Target=100000+xPID.output;  //2-x
		
			Position_PID(&position1_PID);    
			Position_PID(&position2_PID); 
			//Set_MOTOR1(position1_PID.output-pianzhi);
			//Set_MOTOR2(position2_PID.output-pianzhi);			
	

		break;
			case SUBJECT2:    //1-5
			
			if(handle_flag==1){
				xPID.Target=coordinate[4].x;
				xPID.Present=x;
				xPID.minBias=20;
				xPID.maxBias=70;
				
				yPID.Target=coordinate[4].y;	
				yPID.Present=y;
				yPID.minBias=20;
				yPID.maxBias=70;	
								
				if(myabs(yPID.Present-yPID.Target)<15&&myabs(dx)<2){
					xPID.KP=300;
					xPID.KI=7;
					xPID.KD=5000;
				
					yPID.KP=300;
					yPID.KI=7;
					yPID.KD=5000;
					
				}else{
					xPID.KP=300;
					xPID.KI=3.5;
					xPID.KD=3000;
				
					yPID.KP=300;
					yPID.KI=3.5;
					yPID.KD=3000;
					}
					IS_PID(&xPID);    
					IS_PID(&yPID);
				}

			position1_PID.KP=0.4;             //
			position1_PID.KI=0;             //
			position1_PID.KD=10;        
			position1_PID.Target=100000-yPID.output;   //1-y
		
			position2_PID.KP=0.4;             //
			position2_PID.KI=0;             //
			position2_PID.KD=10;            //
			position2_PID.Target=100000+xPID.output;  //2-x
		
			Position_PID(&position1_PID);    
			Position_PID(&position2_PID); 
			Set_MOTOR1(position1_PID.output-pianzhi);
			Set_MOTOR2(position2_PID.output-pianzhi);			

		break;
		
			case SUBJECT3:    //1-4-5
			
			if(handle_flag==1){
				xPID.Present=x;
				xPID.minBias=20;
				xPID.maxBias=70;
				yPID.Present=y;
				yPID.minBias=20;
				yPID.maxBias=70;
				
				if(time<2000){
					xPID.Target=coordinate[3].x+5;
					yPID.Target=coordinate[3].y;	
					xPID.KP=280;
					xPID.KI=3.5;
					xPID.KD=3300 ;
	
					yPID.KP=280;
					yPID.KI=6;
					yPID.KD=3300;
					
				}else{
					xPID.Target=coordinate[4].x;
					yPID.Target=coordinate[4].y;
					xPID.KP=280;
					xPID.KI=7;
					xPID.KD=3330 ;
	

					yPID.KP=280;
					yPID.KI=3.5;
					yPID.KD=3300;
					
				}

				IS_PID(&xPID);    
				IS_PID(&yPID);

			}
			position1_PID.KP=0.4;             //
			position1_PID.KI=0;             //
			position1_PID.KD=10;        
			position1_PID.Target=100000-yPID.output;   //1-y
		
			position2_PID.KP=0.4;             //
			position2_PID.KI=0;             //
			position2_PID.KD=10;            //
			position2_PID.Target=100000+xPID.output;  //2-x
		
			Position_PID(&position1_PID);    
			Position_PID(&position2_PID); 
			Set_MOTOR1(position1_PID.output-pianzhi);
			Set_MOTOR2(position2_PID.output-pianzhi);			

		break;
			
			case SUBJECT4:    //1-9
			
			if(handle_flag==1){
				xPID.Present=x;
				xPID.minBias=20;
				xPID.maxBias=70;
				yPID.Present=y;
				yPID.minBias=20;
				yPID.maxBias=70;
				if(time<500){
					xPID.Target=coordinate[4].x+30;
					yPID.Target=coordinate[4].y+30;	
					xPID.KP=280;
					xPID.KI=4;
					xPID.KD=3300;
					yPID.KP=280;
					yPID.KI=4;
					yPID.KD=4000;
				}else{
					xPID.Target=coordinate[8].x;
					yPID.Target=coordinate[8].y;
					xPID.KP=250;
					xPID.KI=10;
					xPID.KD=3500;
					yPID.KP=250;
					yPID.KI=10;
					yPID.KD=3500;
				}
				
				VI_PID(&xPID);    
				VI_PID(&yPID);

			}
			position1_PID.KP=0.4;             //
			position1_PID.KI=0;             //
			position1_PID.KD=10;        
			position1_PID.Target=100000-yPID.output;   //1-y
		
			position2_PID.KP=0.4;             //
			position2_PID.KI=0;             //
			position2_PID.KD=10;            //
			position2_PID.Target=100000+xPID.output;  //2-x
		
			Position_PID(&position1_PID);    
			Position_PID(&position2_PID); 
			Set_MOTOR1(position1_PID.output-pianzhi);
			Set_MOTOR2(position2_PID.output-pianzhi);			

		break;
			
		case SUBJECT5:    //1-2-6-9
			
			if(handle_flag==1){
				xPID.Present=x;
				xPID.minBias=20;
				xPID.maxBias=70;
				yPID.Present=y;
				yPID.minBias=20;
				yPID.maxBias=70;
				if(time<200*11){				//1-2
					xPID.Target=coordinate[1].x;
					yPID.Target=coordinate[1].y;

					xPID.KP=300;
					xPID.KI=3;
					xPID.KD=2500;
	
					yPID.KP=300;
					yPID.KI=3;
					yPID.KD=2500;	
				}else if(time<200*22){			//2-6
					xPID.Target=coordinate[5].x;
					yPID.Target=coordinate[5].y;

					xPID.KP=300;
					xPID.KI=5;
					xPID.KD=4000;
	
					yPID.KP=300;
					yPID.KI=4;
					yPID.KD=4000;	
				
				}else{						//6-9
					xPID.Target=coordinate[8].x;
					yPID.Target=coordinate[8].y;

					xPID.KP=300;
					xPID.KI=11;
					xPID.KD=3300;
	
					yPID.KP=300;
					yPID.KI=11;
					xPID.KD=3300;
				}
				
				VI_PID(&xPID);    
				VI_PID(&yPID);

			}
			position1_PID.KP=0.4;             //
			position1_PID.KI=0;             //
			position1_PID.KD=15;        
			position1_PID.Target=100000+yPID.output;   //1-y
		
			position2_PID.KP=0.4;             //
			position2_PID.KI=0;             //
			position2_PID.KD=15;            //
			position2_PID.Target=100000-xPID.output;  //2-x
		
			Position_PID(&position1_PID);    
			Position_PID(&position2_PID); 
			Set_MOTOR1(position1_PID.output-pianzhi);
			Set_MOTOR2(position2_PID.output-pianzhi);			

		break;
			
			case SUBJECT6:    //圆
			
			if(handle_flag==1){
				xPID.Present=x;
				xPID.minBias=20;
				xPID.maxBias=70;
				yPID.Present=y;
				yPID.minBias=20;
				yPID.maxBias=70;	
				if(time<200*2){				
					xPID.Target=coordinate[4].x;
					yPID.Target=coordinate[4].y+40;
					xPID.KP=300;
					xPID.KI=0;
					xPID.KD=2000;
	
					yPID.KP=300;
					yPID.KI=0;
					yPID.KD=2000;
				}else if(time<200*4){				
					xPID.Target=coordinate[4].x+40;
					yPID.Target=coordinate[4].y;

					xPID.KP=300;
					xPID.KI=0;
					xPID.KD=2000;
	
					yPID.KP=300;
					yPID.KI=0;
					yPID.KD=2000;
				}else if(time<200*6){				
					xPID.Target=coordinate[4].x;
					yPID.Target=coordinate[4].y-40;
					xPID.KP=300;
					xPID.KI=0;
					xPID.KD=2000;
	
					yPID.KP=300;
					yPID.KI=0;
					yPID.KD=2000;
				}else if(time<200*8){				
					xPID.Target=coordinate[4].x-40;
					yPID.Target=coordinate[4].y;
					xPID.KP=300;
					xPID.KI=0;
					xPID.KD=2000;
	
					yPID.KP=300;
					yPID.KI=0;
					yPID.KD=2000;
				}else if(time<200*10){				
					xPID.Target=coordinate[4].x;
					yPID.Target=coordinate[4].y+40;
					xPID.KP=300;
					xPID.KI=0;
					xPID.KD=2000;
	
					yPID.KP=300;
					yPID.KI=0;
					yPID.KD=2000;
				}else if(time<200*12){				
					xPID.Target=coordinate[4].x+40;
					yPID.Target=coordinate[4].y;
					xPID.KP=300;
					xPID.KI=0;
					xPID.KD=2000;
	
					yPID.KP=300;
					yPID.KI=0;
					yPID.KD=2000;
				}else if(time<200*14){				
					xPID.Target=coordinate[4].x;
					yPID.Target=coordinate[4].y-40;
					xPID.KP=300;
					xPID.KI=0;
					xPID.KD=2000;
	
					yPID.KP=250;
					yPID.KI=0;
					yPID.KD=2000;
				}else if(time<200*16){				
					xPID.Target=coordinate[4].x-40;
					yPID.Target=coordinate[4].y;
					xPID.KP=300;
					xPID.KI=0;
					xPID.KD=2000;
	
					yPID.KP=300;
					yPID.KI=0;
					yPID.KD=2000;
				}else if(time<200*18){				
					xPID.Target=coordinate[4].x;
					yPID.Target=coordinate[4].y+40;
					xPID.KP=300;
					xPID.KI=0;
					xPID.KD=2000;
	
					yPID.KP=300;
					yPID.KI=0;
					yPID.KD=2000;
				}else if(time<200*20){				
					xPID.Target=coordinate[4].x+40;
					yPID.Target=coordinate[4].y;
					xPID.KP=300;
					xPID.KI=0;
					xPID.KD=2000;
	
					yPID.KP=300;
					yPID.KI=0;
					yPID.KD=2000;
				}else  if(time<200*22){				
					xPID.Target=coordinate[4].x;
					yPID.Target=coordinate[4].y-40;
					xPID.KP=300;
					xPID.KI=0;
					xPID.KD=2000;
	
					yPID.KP=300;
					yPID.KI=0;
					yPID.KD=2000;
				}else if(time<200*24){				
					xPID.Target=coordinate[4].x-40;
					yPID.Target=coordinate[4].y;
					xPID.KP=300;
					xPID.KI=0;
					xPID.KD=2000;
	
					yPID.KP=300;
					yPID.KI=0;
					yPID.KD=2000;
				}else if(time<200*26){				
					xPID.Target=coordinate[4].x;
					yPID.Target=coordinate[4].y+40;
					xPID.KP=300;
					xPID.KI=0;
					xPID.KD=2000;
	
					yPID.KP=300;
					yPID.KI=0;
					yPID.KD=2000;
				}else if(time<200*28){				
					xPID.Target=coordinate[4].x+40;
					yPID.Target=coordinate[4].y;
					xPID.KP=300;
					xPID.KI=0;
					xPID.KD=2000;
	
					yPID.KP=300;
					yPID.KI=0;
					yPID.KD=2000;
				}else{
					xPID.Target=coordinate[8].x;
					yPID.Target=coordinate[8].y;

					xPID.KP=300;
					xPID.KI=11;
					xPID.KD=3300;
	
					yPID.KP=300;
					yPID.KI=11;
					xPID.KD=3300;
					
				}
				
				VI_PID(&xPID);    
				VI_PID(&yPID);

			}
			position1_PID.KP=0.4;             //
			position1_PID.KI=0;             //
			position1_PID.KD=15;        
			position1_PID.Target=100000-yPID.output;   //1-y
		
			position2_PID.KP=0.4;             //
			position2_PID.KI=0;             //
			position2_PID.KD=15;            //
			position2_PID.Target=100000+xPID.output;  //2-x
		
			Position_PID(&position1_PID);    
			Position_PID(&position2_PID); 
			Set_MOTOR1(position1_PID.output-pianzhi);
			Set_MOTOR2(position2_PID.output-pianzhi);			

		break;
			
			case SUBJECT7:    //shunxu
			if(Pstate==0){
				if(handle_flag==1){
				if(time/2000<9&&shunxu[time/2000]!=0){
				xPID.Target=coordinate[shunxu[time/2000]-1].x;
				yPID.Target=coordinate[shunxu[time/2000]-1].y;	
					current=time/2000;
					printf("current:%d",current);
				}
				xPID.Present=x;
				xPID.minBias=20;
				xPID.maxBias=70;
				xPID.KP=300;
				xPID.KI=4;
				xPID.KD=3000;
	
				yPID.Present=y;
				yPID.minBias=20;
				yPID.maxBias=70;	
				yPID.KP=300;
				yPID.KI=4;
				yPID.KD=3000;
				
				VI_PID(&xPID);    
				VI_PID(&yPID);

			}
			position1_PID.KP=0.4;             //
			position1_PID.KI=0;             //
			position1_PID.KD=10;        
			position1_PID.Target=100000-yPID.output;   //1-y
		
			position2_PID.KP=0.4;             //
			position2_PID.KI=0;             //
			position2_PID.KD=10;            //
			position2_PID.Target=100000+xPID.output;  //2-x
		
			Position_PID(&position1_PID);    
			Position_PID(&position2_PID); 
			Set_MOTOR1(position1_PID.output-pianzhi);
			Set_MOTOR2(position2_PID.output-pianzhi);			
		}else{
			position1_PID.KP=0.4;             //
			position1_PID.KI=0;             //
			position1_PID.KD=10;            //
			position1_PID.Target=100000;
		
			position2_PID.KP=0.4;             //
			position2_PID.KI=0;             //
			position2_PID.KD=10;            //
			position2_PID.Target=100000;
		
			Position_PID(&position1_PID);    
			Position_PID(&position2_PID); 
			Set_MOTOR1(position1_PID.output-pianzhi);
			Set_MOTOR2(position2_PID.output-pianzhi);	
			
		}
		break;
		}

		
		
	usart1_Analyse();								//分析串口1的指令
		
		
		
		if(handle_flag==1){
			if(Pcount2-lastframe!=0){
				fps=1000.0/(Pcount2-lastframe);
			}
			lastframe=Pcount2;	
		}
		
		
	if(timeflag==1) time++;
	if(Pcount%50==0){
		//cx
		chtostr(str,cx,4,1);
		display(0x91,str);
		//cy
		chtostr(str,cy,4,1);
		display(0x95,str);
		//time
		inttostr(str,time/200,3);
		display(0x8B,str);
		//fps
		inttostr(str,fps,2);
		display(0x8F,str);
		//mode
		if(Pstate==0){
			display(0x98,"模式");
			display(0x9A,modestr[mode]);
			if(mode==SUBJECT7){
				str[0]=shunxu[current]+'0';
				str[1]=0;
				display(0x9F,str);
			}
		}else{
			display(0x98,"顺序");
				for(i=0;i<9;i++){
				if(shunxu[i]==0) str[i]=' ';
				else str[i]=shunxu[i]+'0';
			}
				display(0x9A,str);
		}
	}
	
	keyHandle();
	handle_flag=0;   //!!!!!!!!
	if(++Pcount>199) Pcount=0;			//Pcount 0-199循环，一秒一轮
	Pcount2+=5;
		
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
}
}
	
void usart1_Analyse(void)
{
	if(usart1_ready==0) return;
	switch(usart1_code){
		case mode0: 
			mode=STOPMODE;
		Pstate=0;
			printf("STOPMODE");break;
		case mode1: 
			mode=NOPWM;
			Pstate=0;
			printf("NOPWM");break;
		case mode2: 
			mode=SUBJECT1;
			Pstate=0;
			printf("SUBJECT1");break;
	}
	usart1_ready=0;   //buffer reset
}

void keyHandle(){
	int i;
	key=KEY_Scan(0);
	switch(key){
		case KEY0_PRES:           //回复到100000位置
			if(Pstate==0){
			mode=STOPMODE;
			clearPID(&xPID);
			clearPID(&yPID);
			clearPID(&position1_PID);
			clearPID(&position2_PID);
		  
			timeflag=0;
			time=0;
			printf("STOPMODE");
			}else{
				shunxu[seek++]=1;
				if(seek==9) Pstate=0;
			}
		break;
		case KEY1_PRES:						//重设100000位置
			if(Pstate==0){
			position1=100000;
			position2=100000;
			timeflag=0;
			time=0;
			clearPID(&xPID);
			clearPID(&yPID);
			clearPID(&position1_PID);
			clearPID(&position2_PID);
			}else{
			shunxu[seek++]=2;
			if(seek==9) Pstate=0;
			}
		break;
		case KEY2_PRES:						//速率0
			if(Pstate==0){
			position1=100000;
			mode=NOPWM;
			Pstate=0;
			timeflag=0;
			time=0;
			printf("NOPWM");
			}else{
			shunxu[seek++]=3;
			if(seek==9) Pstate=0;
			}
		break;
		case KEY3_PRES:						//题目1
			if(Pstate==0){
			mode=SUBJECT1;
			clearPID(&xPID);
			clearPID(&yPID);
			clearPID(&position1_PID);
			clearPID(&position2_PID);
			Pstate=0;
			timeflag=1;
			time=0;
			printf("SUBJECT1");
			}else{
			shunxu[seek++]=4;
			if(seek==9) Pstate=0;
			}
		break;
		case KEY4_PRES:						//题目2
			if(Pstate==0){
			mode=SUBJECT2;
			clearPID(&xPID);
			clearPID(&yPID);
			clearPID(&position1_PID);
			clearPID(&position2_PID);
			Pstate=0;
			timeflag=1;
			time=0;
			printf("SUBJECT2");
			}else{
			shunxu[seek++]=5;
			if(seek==9) Pstate=0;
			}
		break;
		case KEY5_PRES:						//题目3
			if(Pstate==0){
			mode=SUBJECT3;
			clearPID(&xPID);
			clearPID(&yPID);
			clearPID(&position1_PID);
			clearPID(&position2_PID);
			Pstate=0;
			timeflag=1;
			time=0;
			printf("SUBJECT3");
			}else{
			shunxu[seek++]=6;
			if(seek==9) Pstate=0;
			}
		break;
		case KEY6_PRES:						//题目4
			if(Pstate==0){
			mode=SUBJECT4;
			clearPID(&xPID);
			clearPID(&yPID);
			clearPID(&position1_PID);
			clearPID(&position2_PID);
			Pstate=0;
			timeflag=1;
			time=0;
			printf("SUBJECT4");
			}else{
			shunxu[seek++]=7;
			if(seek==9) Pstate=0;
			}
		break;
		case KEY7_PRES:						//题目5
			if(Pstate==0){
			mode=SUBJECT5;
			clearPID(&xPID);
			clearPID(&yPID);
			clearPID(&position1_PID);
			clearPID(&position2_PID);
			Pstate=0;
			timeflag=1;
			time=0;
			printf("SUBJECT5");
			}else{
			shunxu[seek++]=8;
			if(seek==9) Pstate=0;
			}
		break;
		case KEY8_PRES:						//题目6
			if(Pstate==0){
			mode=SUBJECT6;
			clearPID(&xPID);
			clearPID(&yPID);
			clearPID(&position1_PID);
			clearPID(&position2_PID);
			Pstate=0;
			timeflag=1;
			time=0;
			printf("SUBJECT6");
			}else{
			shunxu[seek++]=9;
			if(seek==9) Pstate=0;
			}
		break;
		case KEY9_PRES:						//题目7
			if(Pstate==0){
			mode=SUBJECT7;
			clearPID(&xPID);
			clearPID(&yPID);
			clearPID(&position1_PID);
			clearPID(&position2_PID);
			for(i=0;i<9;i++){
				shunxu[i]=0;
			}
			Pstate=1;
			seek=0;
			timeflag=0;
			time=0;
			printf("SUBJECT7");
			}else{
				Pstate=0;
				current=0;
				time=0;
				timeflag=1;
				seek=0;
			}
		break;
		case WKUP_PRES:								//定位
			Pcount2=0;
			dingcount=Pcount2;
			mode=DINGWEI;
			clearPID(&xPID);
			clearPID(&yPID);
			clearPID(&position1_PID);
			clearPID(&position2_PID);
			Pstate=0;
			timeflag=1;
			time=0;
		printf("sss");
		break;
	}

	
}
