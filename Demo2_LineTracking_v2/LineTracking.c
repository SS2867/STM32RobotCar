#include "stm32f10x.h"                  // Device header
#include "utils.h"
#include "Motor.h"
#include <stdio.h>
#include <string.h>

extern char msg[20];
int readLineTracker(void){
	return (16*GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)+
					8*GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4)+
					4*GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5)+
					2*GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6)+
					1*GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7));
}

/*int SPEED_FORWARD = 40;
int SPEED_BRAKE = 30 ;
int SPEED_ADJUST = 63;
int SPEED_RIGHT_1 = 64;
int SPEED_LEFT_1 = 36;
int SPEED_RIGHT_2 = 67;
int SPEED_LEFT_2 = 33;*/
int SPEED_FORWARD = 47;
int SPEED_FORWARD_1 = 35;
int SPEED_FORWARD_2 = 47;
int SPEED_BRAKE = 37 ;
int SPEED_ADJUST_1 = 74; int SPEED_ADJUST;
int SPEED_ADJUST_2 = 77;
int SPEED_RIGHT_1 = 56;
int SPEED_LEFT_1 = 44;
int SPEED_RIGHT_2 = 63;
int SPEED_LEFT_2 = 37;

void LineTracker_init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA, ENABLE);
		
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7; 
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
	/*if (line_prev[2]!=line){
		for (int i=0; i<2; i++){line_prev[i]=line_prev[i+1];}
		line_prev[2] = line;
	}*/
	if(line==0x1B){
		setMotor(3, 0, SPEED_FORWARD);
		if (forward_ticker <= 10){forward_ticker++;}else{brake_ticker=3;}
		//sprintf(msg, "\n\rForward");
	}else if (brake_ticker){
		setMotor(3, 1, SPEED_BRAKE);
		adjustCnt = 0;
		forward_ticker = 0;
		brake_buffer_ticker = 10;
		brake_ticker--;
		//sprintf(msg, "\n\rBrake");
	}else if (adjust_ticker && adjust_ticker<=4){
		setMotor(3, 0, 0);
		adjust_ticker--;
	}else{
		if (adjust_ticker==0){
			adjust_ticker=5;
			if (line==0x1F){if (brake_buffer_ticker==0){line=0;}}
			if (line==0 && brake_buffer_ticker!=0){}else{ 
				if (adjust_l_r_lock_ticker<0){adjust_l_r_lock_ticker++; adjust_l_r=0;}
				else if (adjust_l_r_lock_ticker>0){adjust_l_r_lock_ticker--; adjust_l_r=1;}
				if (adjustCnt<4){ SPEED_ADJUST = SPEED_ADJUST_1;}else{SPEED_ADJUST=SPEED_ADJUST_2;}
				if (adjustCnt<10){adjustCnt++;}
				else {SPEED_ADJUST = SPEED_ADJUST_2;}
				if (adjust_l_r){
					if ((line&0x01)==0){Joystick_control(SPEED_RIGHT_2-adjustCnt/3, SPEED_ADJUST); adjust_l_r=0;}
					else if ((line&0x10)==0){Joystick_control(SPEED_LEFT_2+adjustCnt/3, SPEED_ADJUST);}
					else{
						if((line&0x02)==0){Joystick_control(SPEED_RIGHT_1, SPEED_ADJUST); adjust_l_r=0;}
						else if ((line&0x08)==0){Joystick_control(SPEED_LEFT_1, SPEED_ADJUST);}
					}
				}else{
					if ((line&0x10)==0){Joystick_control(SPEED_LEFT_2+adjustCnt/3, SPEED_ADJUST); adjust_l_r=1;}
					else if ((line&0x01)==0){Joystick_control(SPEED_RIGHT_2-adjustCnt/3, SPEED_ADJUST);}
					else{
						if((line&0x08)==0){Joystick_control(SPEED_LEFT_1, SPEED_ADJUST); adjust_l_r=1;}
						else if ((line&0x02)==0){Joystick_control(SPEED_RIGHT_1, SPEED_ADJUST);}
					}
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
	if((line&0xE)==0 && brake_buffer_ticker==0 && line_prev[1]!=0){
		state += 1;
	}
	/*if(line){
		if (state<4){adjust_l_r=1;}else if (state==5){adjust_l_r=0;}
	}*/
	
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
		setMotor(3, 3, 50);
	}else if(steer_ticker<0){
		steer_ticker++;
		setMotor(3, 2, 50);
	}
	else{ lineTracking(); }
}

