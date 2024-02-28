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

/*int SPEED_STRAIGHT = 30;
int SPEED_TURN_1 = 30 ;
int SPEED_TURN_2 = 30 ;
int SPEED_CURVE_TO_LEFT_1 = 20;
int SPEED_CURVE_TO_LEFT_2 = 15;
int SPEED_CURVE_TO_RIGHT_1 = 20;
int SPEED_CURVE_TO_RIGHT_2 = 15;*/

int SPEED_STRAIGHT = 18;
int SPEED_TURN_1 = 20 ;
int SPEED_TURN_2 = 20 ;
int SPEED_CURVE_TO_LEFT_1 = 17;
int SPEED_CURVE_TO_LEFT_2 = 14;
int SPEED_CURVE_TO_RIGHT_1 = 17;
int SPEED_CURVE_TO_RIGHT_2 = 15 ;

void set_outer_param(){
	SPEED_STRAIGHT = 18;
	SPEED_TURN_1 = 20 ;
	SPEED_TURN_2 = 20 ;
	SPEED_CURVE_TO_LEFT_1 = 17;
	SPEED_CURVE_TO_LEFT_2 = 14;
	SPEED_CURVE_TO_RIGHT_1 = 17;
	SPEED_CURVE_TO_RIGHT_2 = 15 ;
}
void set_inner_param(){
	SPEED_STRAIGHT = 18;
	SPEED_TURN_1 = 19 ;
	SPEED_TURN_2 = 19 ;
	SPEED_CURVE_TO_LEFT_1 = 15;
	SPEED_CURVE_TO_LEFT_2 = 11;
	SPEED_CURVE_TO_RIGHT_1 = 15;
	SPEED_CURVE_TO_RIGHT_2 = 11 ;
}


void LineTracker_init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA, ENABLE);
		
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7; 
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	setMotor(3, 0, SPEED_STRAIGHT);
}

int line_prev[3] = {0, 0, 0};
int state = 2;
int ticker = 0;
void lineTracking(void){
	int line = readLineTracker();
	if (line_prev[2]!=line){
		for (int i=0; i<2; i++){line_prev[i]=line_prev[i+1];}
		line_prev[2] = line;
	}
	if (((line&8)==0||(line&0x10)==0) && (line&2)>0){ //left
		if(state!=1){setMotor(3, 0, 35); if(ticker==0){ticker=1;} ticker--;}
		else{
			setMotor(1, 0, (int)((line&0x10)==0? SPEED_CURVE_TO_LEFT_2:SPEED_CURVE_TO_LEFT_1));
			setMotor(2, 0, (int)((line&0x10)==0? SPEED_TURN_2: SPEED_TURN_1));
		}
		sprintf(msg , "\n\r>L line=%X", line);
		state = 1;
	}else if ((line&8)>0 && (((line&2)==0)||((line&1)==0))){
		if(state!=3){setMotor(3, 0, 35); if(ticker==0){ticker=1;} ticker--;}
		else{
			setMotor(2, 0, (int)((line&0x1)==0? SPEED_CURVE_TO_RIGHT_2:SPEED_CURVE_TO_RIGHT_1));
			setMotor(1, 0, (int)((line&0x1)==0? SPEED_TURN_2: SPEED_TURN_1));
		} 
		sprintf(msg , "\n\r>R line=%X", line);
		state = 3;
	}else if (line==0x1F){
		if (state==1){
			setMotor(1, 0, (int)(SPEED_CURVE_TO_LEFT_2));
			setMotor(2, 0, (int)(SPEED_TURN_2));
		}else if (state==2){
			setMotor(2, 0, (int)(SPEED_CURVE_TO_RIGHT_2));
			setMotor(1, 0, (int)(SPEED_TURN_2));		
		}
	}else{	
		if(state!=2){setMotor(3, 0, 30);}
		setMotor(3, 0, SPEED_STRAIGHT);
		sprintf(msg , "\n\r>F line=%X", line);
		state = 2;
	}


}

