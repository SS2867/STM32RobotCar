#include "stm32f10x.h"                  // Device header
#include "Motor.h"
#include "utils.h"
#include <string.h>
#include <stdio.h>
#include "RemoteControl.h"
#include "LineTracking.h"

char rx_buffer[20] = "";
char msg[20] = "";
int joystick_x = 50;
int joystick_y = 50;
void USART2_IRQHandler() {
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
		char ch = (unsigned char) USART_ReceiveData(USART2); 
		for(int i=0; i<18; i++){rx_buffer[i] = rx_buffer[i+1]; }
		rx_buffer[18] = ch;
		msg[0] = ch;
		
		/*int opand = checkPositiveInteger_2char(rx_buffer[17], rx_buffer[18]);
		if (opand!=-1 && rx_buffer[15]=='/'){ 
			if(rx_buffer[16]=='X'){
				joystick_x = opand; Joystick_control(joystick_x, joystick_y);
				sprintf(msg, "//Set joystick X %d//", opand);
			}else if (rx_buffer[16]=='Y'){
				joystick_y = opand; Joystick_control(joystick_x, joystick_y);
				sprintf(msg, "//Set joystick Y %d//", opand);
			}		
		}*/
		if (rx_buffer[17]=='/'){
			switch(rx_buffer[18]){
				case 'F': setMotor(3, 0, 100); break;
				case 'B': setMotor(3, 1, 100); break;
				case 'L': setMotor(3, 2, 100); break;
				case 'R': setMotor(3, 3, 100); break;
				case 'S': setMotor(3, 0, 0); break;
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




int sTicks = 0;
void TIM2_IRQHandler(void){
	if (TIM_GetITStatus(TIM2, TIM_IT_Update)!=RESET){
		sTicks ++; if (sTicks>=5){
			sprintf(msg, "-%X", readLineTracker()); sTicks = 0;
			lineTracking();
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

int main(){
	motor_init();
	USART2_Init();
	LineTracker_init();
	strcpy(msg , "We are ready!");
	
	setMotor(3, 0, 100);
  //setMotor(3, 0, 30);
	while(1){
		//lineTracking();
		//exhaustive_delay(2);
	
	}
}



