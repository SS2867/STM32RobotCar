#include "stm32f10x.h"                  // Device header
#include "Motor.h"
#include "utils.h"
#include <string.h>
#include <stdio.h>
#include "RemoteControl.h"
#include "LineTracking.h"
#include "parking.h"

char rx_buffer[20] = "";
char msg[20] = "";
int joystick_x = 50;
int joystick_y = 50;
extern int adjust_l_r_lock_ticker;
void USART2_IRQHandler() {
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
		char ch = (unsigned char) USART_ReceiveData(USART2); 
		for(int i=0; i<18; i++){rx_buffer[i] = rx_buffer[i+1]; }
		rx_buffer[18] = ch;
		msg[0] = ch;
		
		if (1){//  (rx_buffer[17]=='/'){
			switch(rx_buffer[18]){
				case 'F': setMotor(3, 0, 100); break;
				case 'B': setMotor(3, 1, 100); break;
				case 'L': //adjust_l_r_lock_ticker=-100; break;
				case 'R': //adjust_l_r_lock_ticker=100; break;
				case 'S': readDistance(); break;
			}
		}
		if (rx_buffer[14]=='/'&&rx_buffer[15]=='k'&&rx_buffer[16]=='e'&&rx_buffer[17]=='y'){
			switch(rx_buffer[18]){
				case 'E': break;
				case 'K': setMotor(3, 0, 0); break;
			}			
		}
		
	}
	if(USART_GetITStatus(USART2, USART_IT_TC) != RESET) {
		if (msg[0] != 0){USART_SendData(USART2, msg[0]);}
		for(int i=0; i<19; i++){msg[i] = msg[i+1];}
		msg[19] = 0;
	}
}

int SYS_CLK_MULT = 9;

extern int state;
int sTicks = 0;
extern int d[5];
extern int adjust_l_r;
void TIM2_IRQHandler(void){
	if (TIM_GetITStatus(TIM2, TIM_IT_Update)!=RESET){
		sTicks ++; if (sTicks%1==0){
			if (sTicks==(6*SYS_CLK_MULT)){
				for(int i=0; i<4;i++) { d[i]=d[i+1]; }
				d[4] = readDistance();
				
				sprintf(msg, "\r>%5dcm %d %2X %d", d[4], state, readLineTracker(), adjust_l_r); 
				
				sTicks = 0;
			}
			routeRelay2();
		
		
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

int main(){
	motor_init();
	USART2_Init();
	LineTracker_init();
	rangeFinderInit();
	strcpy(msg , "We are ready!");
	
	setMotor(3, 0, 0);
	while(1){
		//lineTracking();
		//exhaustive_delay(2);
	
	}
}



