/*
 * Demo1_BasicRemoteControl_Joystick.c
 *
 * Created: 2024/1/31 17:29:31
 * Author : Steven Song
 */ 

#include <avr/io.h>
#include "avr/interrupt.h"

void delay_ms(int t){ // timer 0 occupied
	TCNT0 = 0x00;
	for (int i=0; i<t; i++)
	{
		TCCR0B = 0x0B; // WGM[2:0]=010: CTC
		OCR0A = 250; // Output compare target
		while( (TIFR0&(1<<OCF0A)) == 0x00 ){}
		TCCR0B = 0x00;
		TIFR0 = (1<<OCF0A);
	}
}


int readKey(int GPIO_Port, int GPIO_Pin, int commonState){
	int pressed = 0;
	/*(if (commonState ^ GPIO_ReadInputDataBit(GPIOx, GPIO_Pin)){
		DelayMs(40);
		while (commonState ^ GPIO_ReadInputDataBit(GPIOx, GPIO_Pin)){}
		DelayMs(40);
		pressed = 1;
	}*/
	return pressed;
}


int main(void)
{
	
	DDRD = 0x04;
    /* Replace with your application code */
    while (1) 
    {
		PORTD ^= 0x04;
		delay_ms(1000);
    }
}

