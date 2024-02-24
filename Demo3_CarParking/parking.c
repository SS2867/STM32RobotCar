#include "stm32f10x.h"                  // Device header
#include "utils.h"

void rangeFinderInit(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)0);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

int readDistance(void){
	GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)1);
	exhaustive_delay_us(50);
	GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)0);
	int i = 0;
	for (i = 0; (i<450 && GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)==RESET );i++){
		exhaustive_delay_us(200);
	}
	for (i = 0; (i<450 && GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)==SET );i++){
		exhaustive_delay_us(200);
	}
	
	return i;
}




