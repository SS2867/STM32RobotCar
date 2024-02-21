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

void LineTracker_init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7; 
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

int SPEED_STRAIGHT = 30;
int SPEED_CURVE_1 = 20;
int SPEED_CURVE_2 = 15;

int line_prev1 = 0;

void lineTracking(void){
	int line = readLineTracker();
	if (((line&8)==0||(line&0x10)==0) && (line&2)>0){ //left
		setMotor(1, 0, (int)((line&0x10)==0? SPEED_CURVE_2:SPEED_CURVE_1));
		setMotor(2, 0, (int)(SPEED_STRAIGHT));
		sprintf(msg , "\n\r>L line=%X", line);
	}else if ((line&8)>0 && (((line&2)==0)||((line&1)==0))){
		setMotor(2, 0, (int)((line&0x1)==0? SPEED_CURVE_2:SPEED_CURVE_1));
		setMotor(1, 0, (int)(SPEED_STRAIGHT));
		sprintf(msg , "\n\r>R line=%X", line);
	}else{
		setMotor(3, 0, (int)(SPEED_STRAIGHT));
		sprintf(msg , "\n\r>F line=%X", line);
	}


}

