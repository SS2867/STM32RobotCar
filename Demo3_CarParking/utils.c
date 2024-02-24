#include "stm32f10x.h"                  // Device header
#include <cstring>
#include <cmath>
#include <stdlib.h>

int max(int a, int b){return (a>b)? a:b;}
int min(int a, int b){return (a<b)? a:b;}

void exhaustive_delay(int t) {           // delay by t ms
    int i; int j;
    for (i = 0; i < t; i++) {
        for (j=0;j<2666 ;j++)
        { GPIOA->BSRR ^= 0x00;} // do something to PA0 	 		
    }
}

void exhaustive_delay_us(uint32_t xus){
    int i; int j;
    for (i = 0; i < (xus/49); i++) {
        for (j=0;j<7 ;j++)
        { GPIOA->BSRR ^= 0x00;} // do something to PA0 	 		
    }
}


char* str_slice(char src[], int src_start, int length) {
    char* sliced = (char*) malloc((length + 1) * sizeof(char));
    memcpy(sliced, src + src_start, length);
    sliced[length] = '\0';
    return sliced;
}

int str_is_equal(char str1[], char str2[]){
    return strcmp(str1, str2) == 0;
}

int checkPositiveInteger(char str[]) {
	int num = 0;
	int length = strlen(str);
	if (length == 0){return -1;}
	for (int i=0; i<length; i++){
		if (str[length-i-1]<48 || str[length-i-1]>57) {return -1;}
		num += pow(10, i) * (str[length-i-1]-48);
	}
	return num;
}

int checkPositiveInteger_2char(char ch1, char ch2) {
	if (ch1<48 || ch1>57 || ch2<48 || ch2>57) {return -1;}
	int num = 10*(ch1-48) + (ch2-48);
	return num;
}

void USART2_Init(){
	//USART2 TX RX
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// Tx pin
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// Rx pin
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	// Enable the USART2 TX Interrupt 
	USART_ITConfig(USART2, USART_IT_TC, ENABLE );
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// Enable the USART2 RX Interrupt
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE );
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}



