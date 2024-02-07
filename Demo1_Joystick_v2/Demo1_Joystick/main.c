/*
 * Demo1_BasicRemoteControl_Joystick.c
 *
 * Created: 2024/1/31 17:29:31
 * Author : Steven Song
 */ 

#include <avr/io.h>
#include "avr/interrupt.h"
#include <string.h>
#include <stdio.h>

char tx_buffer[30] = "We are ready!";

void delay_ms(int t){ // timer 0 occupied
	int i=0;
	while(i<t){
		TCNT0 = 0x00;
		TCCR0B = 0x0B; // WGM[2:0]=010: CTC
		OCR0A = 250; // Output compare target
		while( (TIFR0&(1<<OCF0A)) == 0x00 ){}
		TCCR0B = 0x00;
		TIFR0 = (1<<OCF0A);
		i++;
	}
}

void exhaustive_delay_ms(int t){ // timer 0 occupied
	int i=0;
	while(i<t){
		for (unsigned int j=0;j<413;j++){DDRD ^= 0;}
		i++;
	}
}

ISR(USART_UDRE_vect){
	if (tx_buffer[0] != 0){  // If there¡¯s a char to trans
		UDR0 = tx_buffer[0];
	}
	for(int i=0; i<29; i++){  // shift tx_buffer by 1
		tx_buffer[i] = tx_buffer[i+1];
	}
	tx_buffer[29] = 0;
}

void adcinit(){
	// set REFS1 = 0 |REFS0 = 1 (Vref as AVCC pin) | ADLAR = 0(right adjusted) |  MUX4 to MUX0 is 0000 for ADC0
	ADMUX = 0b01000000;
	//enable ADC module, set prescalar of 4 which gives CLK/4
	ADCSRA |= (1<<ADEN) | (0<<ADPS2) | (1<<ADPS1) | (0<<ADPS0);
}

int adcread(unsigned char channel){
	/* set input channel to read */
	ADMUX = 0x40 | (channel & 0x07);   // 0100 0000 | (channel & 0000 0100)
	/* Start ADC conversion */
	ADCSRA |= (1<<ADSC);
	/* Wait until end of conversion by polling ADC interrupt flag */
	while (!(ADCSRA & (1<<ADIF)));
	/* Clear interrupt flag */
	ADCSRA |= (1<<ADIF);
	exhaustive_delay_ms(1);                      /* Wait a little bit */
	/* Return ADC word */
	return ADCW;
}

int readKey(volatile uint8_t* GPIO_Port_Ptr, int GPIO_Pin, int commonState){
	int pressed = 0;
	if ((commonState ^ (*GPIO_Port_Ptr>>GPIO_Pin))&1){
		delay_ms(20);
		while ((commonState ^ (*GPIO_Port_Ptr>>GPIO_Pin))&1){}
		delay_ms(20);
		pressed = 1;
	}
	return pressed;
}

int main(void)
{
	
	UCSR0C = 0x06;
	UBRR0 = 9-1; // 9600 baud rate
	UCSR0B = (0<<RXEN0)|(1<<TXEN0)|(0<<RXCIE0)|(1<<UDRIE0);
	
	TCCR1B = 0x0D;  // Timer 0: with prescaler 64, CTC, 1ms
	OCR1A = 1526;
	TIMSK1 = (1<<OCIE1A);
	
	adcinit();

	sei();
    /* Replace with your application code */
    while (1) 
    {
		if (readKey(&PIND, 2, 1)){strcpy(tx_buffer, "/keyA");}
		if (readKey(&PIND, 3, 1)){strcpy(tx_buffer, "/keyB");}
		if (readKey(&PIND, 4, 1)){strcpy(tx_buffer, "/keyC");}
		if (readKey(&PIND, 5, 1)){strcpy(tx_buffer, "/keyD");}
		if (readKey(&PIND, 6, 1)){strcpy(tx_buffer, "/keyE");}
		if (readKey(&PIND, 7, 1)){strcpy(tx_buffer, "/keyF");}
		if (readKey(&PINB, 0, 1)){strcpy(tx_buffer, "/keyK");}
		/*sprintf(tx_buffer, "/X%02d", (int)(adcread(0)/10.24));
		sprintf(tx_buffer, "/Y%02d", (int)(adcread(1)/10.24));*/
		delay_ms(40);
    }
}

int joystick_check_XY = 0;
int joystick_X = 50;
int joystick_Y = 50;
ISR(TIMER1_COMPA_vect){
	if(joystick_check_XY){
		joystick_X = (int)(adcread(0)/10.24);
		//sprintf(tx_buffer, "/X%02d", joystick_X);
	}else{
		joystick_Y = (int)(adcread(1)/10.24);
		//sprintf(tx_buffer, "/Y%02d", joystick_Y);
	}
	joystick_check_XY = 1-joystick_check_XY;
	if(joystick_X<35 && joystick_Y<80 && joystick_Y>20){ sprintf(tx_buffer, "/L");}
	else if(joystick_Y>70 && joystick_X<80 && joystick_X>20){ sprintf(tx_buffer, "/F");}
	else if(joystick_X>65 && joystick_Y<70 && joystick_Y>20){ sprintf(tx_buffer, "/R");}
	else if(joystick_Y<35 && joystick_X<80 && joystick_X>20){ sprintf(tx_buffer, "/B");}
	else{ sprintf(tx_buffer, "/S");}
}

