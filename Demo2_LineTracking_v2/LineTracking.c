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

int SPEED_FORWARD = 40;
int SPEED_BRAKE = 30 ;
int SPEED_ADJUST = 65;
int SPEED_RIGHT_1 = 65;
int SPEED_LEFT_1 = 35;
int SPEED_RIGHT_2 = 70;
int SPEED_LEFT_2 = 30;


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
int state = 2;
int forward_ticker = 0; int brake_ticker =0; int adjust_ticker = 0; int brake_buffer_ticker = 0;
int adjust_l_r = 0; //=1: left detect first, =0: right detect first
void lineTracking(void){
	int line = readLineTracker();
	if (line_prev[2]!=line){
		for (int i=0; i<2; i++){line_prev[i]=line_prev[i+1];}
		line_prev[2] = line;
	}
	if(line==0x1B){
		setMotor(3, 0, SPEED_FORWARD);
		if (forward_ticker <= 10){forward_ticker++;}else{brake_ticker=5;}
		//sprintf(msg, "\n\rForward");
	}else if (brake_ticker){
		setMotor(3, 1, SPEED_BRAKE);
		brake_buffer_ticker = 4;
		brake_ticker--;
		//sprintf(msg, "\n\rBrake");
	}else if (adjust_ticker && adjust_ticker<=1){
		setMotor(3, 0, 0);
		adjust_ticker--;
	}else{
		if (adjust_ticker==0){
			adjust_ticker=2;
			if (line==0x1F){if (brake_buffer_ticker==0){line=0;}}
			if (line==0 && brake_buffer_ticker!=0){}else{ 
				if (adjust_l_r){
					if ((line&0x01)==0){Joystick_control(SPEED_RIGHT_2, SPEED_ADJUST); adjust_l_r=0;}
					else if ((line&0x10)==0){Joystick_control(SPEED_LEFT_2, SPEED_ADJUST);}
					else{
						if((line&0x02)==0){Joystick_control(SPEED_RIGHT_1, SPEED_ADJUST); adjust_l_r=0;}
						else if ((line&0x08)==0){Joystick_control(SPEED_LEFT_1, SPEED_ADJUST);}
					}
				}else{
					if ((line&0x10)==0){Joystick_control(SPEED_LEFT_2, SPEED_ADJUST); adjust_l_r=1;}
					else if ((line&0x01)==0){Joystick_control(SPEED_RIGHT_2, SPEED_ADJUST);}
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



