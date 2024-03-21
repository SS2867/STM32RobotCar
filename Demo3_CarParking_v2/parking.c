#include "stm32f10x.h"                  // Device header
#include "utils.h"
#include "Motor.h"
#include <stdio.h>

extern char msg[];

void rangeFinderInit(void){
	// A0 Trig -> PA15 TIM2 FullRemap Ch1 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)0);	
	
	
	// PA1 Echo -> PA6 TIM3 Ch1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM3);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 330/1 - 1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0x0;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);
	
	TIM_Cmd(TIM3, ENABLE);
}

int readDistance(void){
	GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)1);
	exhaustive_delay_us(100);
	GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)0);
	exhaustive_delay_us (500);
	return TIM_GetCapture2(TIM3);
}

int parking_brake_ticker = 30;
int parking_state = 1;
void parking(int d1, int d2){
	if ((parking_state==1) && (d1>180) && (d2>180)){parking_state=0;}
	else if ((parking_state==0) && (d1<165) && (d2<165)){parking_state=1;}
	if (parking_state==0){
		setMotor(1, 0, 100 );
		setMotor(2, 0, 94 );
		if(parking_brake_ticker<25*9){parking_brake_ticker++;}
	}else {
		if (parking_brake_ticker ){
			setMotor(3, 1, 30);
			parking_brake_ticker --;
		}else{setMotor(3, 0, 0);}
	}
}



