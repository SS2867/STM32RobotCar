#include "stm32f10x.h"                  // Device header
#include "utils.h"
#include "Motor.h"
#include <stdio.h>
#include <string.h>
#include "parking.h"

extern char msg[20];
int readLineTracker(void){
	return (16*GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)+
					8*GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4)+
					4*GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5)+
					2*GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9)+
					1*GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7));
}


/*int SPEED_FORWARD = 50;
int SPEED_FORWARD_1 = 45;
int SPEED_FORWARD_2 = 50;
int SPEED_BRAKE = 35 ;
int SPEED_ADJUST_1 = 68; int SPEED_ADJUST;
int SPEED_ADJUST_2 = 70;
int SPEED_RIGHT_1 = 56;
int SPEED_LEFT_1 = 44;
int SPEED_RIGHT_2 = 63;
int SPEED_LEFT_2 = 37;
int SPEED_TURNING = 50;*/
int SPEED_FORWARD = 70;
int SPEED_FORWARD_1 = 85;
int SPEED_FORWARD_2 = 100;
int SPEED_BRAKE = 50 ;
int SPEED_ADJUST_1 = 90; int SPEED_ADJUST;
int SPEED_ADJUST_2 = 95;
int SPEED_RIGHT_1 = 85;
int SPEED_LEFT_1 = 15;
int SPEED_RIGHT_2 = 90;
int SPEED_LEFT_2 = 10;
int SPEED_TURNING = 80;

extern int SYS_CLK_MULT;

void LineTracker_init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA, ENABLE);
		
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_9 | GPIO_Pin_7; 
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	setMotor(3, 0, 20);
}


int line_prev[3] = {0, 0, 0};
int forward_ticker = 0; int brake_ticker =0; int adjust_ticker = 0; 
int brake_buffer_ticker = 0; int adjust_l_r_lock_ticker = 0; 
int steer_ticker = 0; // >0: right, <0:left
int adjust_l_r = 0; //=1: left detect first, =0: right detect first
int adjustCnt = 0;
int routeCount = 0;
void lineTracking(void){
	int line = readLineTracker();
	if(line==0x1B){// || (line==0x13) || (line==0x19 )){
		setMotor(3, 0, SPEED_FORWARD);
		if (forward_ticker <= 8*SYS_CLK_MULT){forward_ticker++;}else{brake_ticker=2*SYS_CLK_MULT;}
		//sprintf(msg, "\n\rForward");
	}else if (brake_ticker){
		setMotor(3, 1, SPEED_BRAKE);
		adjustCnt = 0;
		forward_ticker = 0;
		brake_buffer_ticker = 10*SYS_CLK_MULT;
		brake_ticker--;
		//sprintf(msg, "\n\rBrake");
	}else if (adjust_ticker && adjust_ticker<2 *SYS_CLK_MULT){
		setMotor(3, 0, 0);
		adjust_ticker--;
	}else{
		if (adjust_ticker==0){
			adjust_ticker=5*SYS_CLK_MULT;
			if (line==0 && brake_buffer_ticker!=0){}else{ 
				if (line==0x1F){line=0;}
				
				if (adjust_l_r_lock_ticker<0){adjust_l_r_lock_ticker++; adjust_l_r=0;}
				else if (adjust_l_r_lock_ticker>0){adjust_l_r_lock_ticker--; adjust_l_r=1;}
				
				if (adjustCnt*SYS_CLK_MULT<6){ SPEED_ADJUST = SPEED_ADJUST_1;}else{SPEED_ADJUST=SPEED_ADJUST_2;}
				if (adjustCnt*SYS_CLK_MULT<10){adjustCnt++;}
				else {SPEED_ADJUST = SPEED_ADJUST_2;}
				
				if (adjust_l_r){
					if ((line&0x01)==0){Joystick_control(SPEED_RIGHT_2-adjustCnt/3/SYS_CLK_MULT, SPEED_ADJUST); adjust_l_r=0;}
					/*
					else{
						if((line&0x02)==0){Joystick_control(SPEED_RIGHT_1, SPEED_ADJUST); adjust_l_r=0;}
						else if ((line&0x08)==0){Joystick_control(SPEED_LEFT_1, SPEED_ADJUST);}
					}*/
					else if((line&0x02)==0){Joystick_control(SPEED_RIGHT_1, SPEED_ADJUST); adjust_l_r=0;}
					else if ((line&0x08)==0){Joystick_control(SPEED_LEFT_1, SPEED_ADJUST);}
					else if ((line&0x10)==0){Joystick_control(SPEED_LEFT_2+adjustCnt/3/SYS_CLK_MULT, SPEED_ADJUST);}
				}else{
					if ((line&0x10)==0){Joystick_control(SPEED_LEFT_2+adjustCnt/3/SYS_CLK_MULT, SPEED_ADJUST); adjust_l_r=1;}
					/*else if ((line&0x01)==0){Joystick_control(SPEED_RIGHT_2-adjustCnt/3, SPEED_ADJUST);}
					else{
						if((line&0x08)==0){Joystick_control(SPEED_LEFT_1, SPEED_ADJUST); adjust_l_r=1;}
						else if ((line&0x02)==0){Joystick_control(SPEED_RIGHT_1, SPEED_ADJUST);}
					}*/
					else if((line&0x08)==0){Joystick_control(SPEED_LEFT_1, SPEED_ADJUST); adjust_l_r=1;}
					else if ((line&0x01)==0){Joystick_control(SPEED_RIGHT_2-adjustCnt/3/SYS_CLK_MULT, SPEED_ADJUST);}
					else if ((line&0x02)==0){Joystick_control(SPEED_RIGHT_1, SPEED_ADJUST);}
				}
			}
		}else{adjust_ticker--;}
		if (brake_buffer_ticker){brake_buffer_ticker--;}
		//adjust_l_r = 0; 
		
		//sprintf(msg, "\n\rAdjust,%d ", adjust_l_r);
	}
	
}

int state = 0;
void routeSelect(void){
	routeCount++; 
	//sprintf(msg, "\n\rstate=%d ", state);
	int line = readLineTracker();
	int odom = TIM_GetCounter(TIM4);
	if (brake_buffer_ticker==0){
		for (int i=0; i<2; i++){line_prev[i]=line_prev[i+1];}
		line_prev[2] = line;
	}
	if(line){
		if (odom<40){adjust_l_r=1;} else if (odom>120 && odom<150){adjust_l_r=0;}
		else if (odom>170 && odom<190){adjust_l_r=0;}
		else if (odom>222 && odom <=229){adjust_l_r=1;}
		else if (odom>229 && odom <240){adjust_l_r=1;} 
		else if(odom>266 && odom<320){adjust_l_r=0;}
		else if (odom>320 && odom<370) {adjust_l_r=0;}
		if (odom==267) {steer_ticker=-12;} 
	}
	
	if (odom==60){SPEED_ADJUST=SPEED_ADJUST_1;}
	else if (odom==80){SPEED_FORWARD=SPEED_FORWARD_2;}
	else if(odom==130){SPEED_FORWARD=SPEED_FORWARD_1;}
	else if (odom==180){SPEED_FORWARD=SPEED_FORWARD_2;}
	else if(odom==215){SPEED_FORWARD=SPEED_FORWARD_1;}
	else if (odom==235){SPEED_FORWARD=SPEED_FORWARD_2;}
	else if (odom==268){SPEED_FORWARD=SPEED_FORWARD_1;}
	
	if(steer_ticker>0){
		steer_ticker--;
		setMotor(3, 3, SPEED_TURNING);
	}else if(steer_ticker<0){
		steer_ticker++;
		setMotor(3, 2, SPEED_TURNING);
	}
	else{ lineTracking(); }
}

int darkCnt(int line){
	int darkCnt = 0;
	for(int i=0; i<5; i++){if((line&(1<<i))==0){darkCnt++;}}
	return darkCnt * (brake_buffer_ticker==0);
}

int d[5] = {50, 50, 50, 50, 50};
void routeRelay1(void){ 	// Car 1
	routeCount++; 
	int line = readLineTracker();
	if (state==0){ 	// Car 1: A -> D -> C
		lineTracking(); 
		
		int reached = (routeCount>50*SYS_CLK_MULT? 1:0); 	
		for(int i=0; i<5; i++){if(d[i]>60){reached=0;}} 
		if(reached==1){
			state = 1;
			routeCount = 0;
		}					
	}else if (state==1){  // Car 1 stop at C
		lineTracking(); 
		if(darkCnt(line)>=3){state = 2;}
	}else if (state==2){  // Car 1 wait for Car 2
		setMotor(3, 0, 0);
		int reached=1;
		for(int i=0; i<5; i++){if(d[i]>40){reached=0;}}
		if(reached && (routeCount>1500*SYS_CLK_MULT)){
			state = 3;
			routeCount=0;
		}
	}else if (state==3){ // Car 1: wait a bit for Car 2 to detect
		if(routeCount>20*SYS_CLK_MULT){state=4; routeCount=0;}
	}else if (state==4){
		if (routeCount<50*SYS_CLK_MULT){
			setMotor(3, 3, SPEED_TURNING);
		}else{
			lineTracking(); 
		}
	}
}


void routeRelay2(void){ 	// Car 2
	int line = readLineTracker();
	routeCount++; 
	if (state==0){ 	// Car 2 wait for Car 1
		setMotor(3, 0, 0);
		int reached=1;
		for(int i=0; i<5; i++){if(d[i]>50){reached=0;}}
		if(reached && (routeCount>200*SYS_CLK_MULT)){
			state = 1;
			routeCount=0;
		}
	}else if (state==1){  // Car 2 wait a bit for Car 1 to detect
		if(routeCount>20*SYS_CLK_MULT){state = 2; routeCount=0;}
	}else if (state==2){  // Car 2:  C -> X -> (A)
		if (routeCount<5*SYS_CLK_MULT){setMotor(3, 2, 100);  SPEED_FORWARD = SPEED_FORWARD_1;}
		else if(routeCount<60*SYS_CLK_MULT){setMotor(3, 2, SPEED_TURNING); }
		else if(routeCount<70*SYS_CLK_MULT){setMotor(3, 0, 0);}
		else{
			if (routeCount<500*SYS_CLK_MULT){adjust_l_r=1;}
			lineTracking(); 
			if((darkCnt(line)>=3) && (routeCount>1000*SYS_CLK_MULT)){
				routeCount = 0;
				state = 3;
			}
		}
	}else if (state==3){ // Car 2:  -> A -> B -> C
		if(routeCount<100*SYS_CLK_MULT){lineTracking();}
		else if(routeCount<110*SYS_CLK_MULT){setMotor(3, 0, 0); SPEED_FORWARD = SPEED_FORWARD_2;}
		else if (routeCount<113*SYS_CLK_MULT){setMotor(3, 2, 80);}
		else if(routeCount<180*SYS_CLK_MULT){setMotor(3, 2, SPEED_TURNING); }
		else{
			if (routeCount<500*SYS_CLK_MULT){adjust_l_r=1;}
			lineTracking(); 
			int reached=1;
			for(int i=0; i<5; i++){if(d[i]>35){reached=0;}}
			if(reached){
				state = 5;
				routeCount = 0;
				SPEED_FORWARD = SPEED_FORWARD_1;
			}			
			//if((darkCnt(line)>=3) && routeCount>=300) {routeCount=0; state=4; } 
		}
	}else if (state==4){ // Car 2:  -> B -> C
		lineTracking();
		if(routeCount==200*SYS_CLK_MULT){setMotor(3, 0, 0); SPEED_FORWARD = SPEED_FORWARD_1;}
		if(routeCount>200*SYS_CLK_MULT){ 
			int reached=1;
			for(int i=0; i<5; i++){if(d[i]>60){reached=0;}}
			if(reached){
				state = 4;
				routeCount = 0;
				SPEED_FORWARD = SPEED_FORWARD_1;
			}			
		}
	}else if (state==5){ // Car 2 arrive at C
		lineTracking();
		if((darkCnt(line)>=3)) {routeCount=0; state=6; } 
	}else if (state==6) {
		if(routeCount<200*SYS_CLK_MULT){lineTracking();}
		else{setMotor(3, 0, 0);}
	}
}

