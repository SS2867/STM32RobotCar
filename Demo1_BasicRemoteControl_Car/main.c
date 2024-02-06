#include "stm32f10x.h"                  // Device header
#include "Motor.h"
#include "utils.h"
#include <string.h>
#include <stdio.h>

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
		
		/*int opand = checkPositiveInteger(str_slice(rx_buffer, 18-2+1, 2));
		if (opand!=-1){ 
			if(str_is_equal(str_slice(rx_buffer, 18-4+1, 2), "/X")){
				joystick_x = opand; Joystick_control(joystick_x, joystick_y);
				sprintf(msg, "//Set joystick X %d//", opand);
			}else if (str_is_equal(str_slice(rx_buffer, 18-4+1, 2), "/Y")){
				joystick_y = opand; Joystick_control(joystick_x, joystick_y);
				sprintf(msg, "//Set joystick Y %d//", opand);
			}		
		}*/
		int opand = checkPositiveInteger_2char(rx_buffer[17], rx_buffer[18]);
		if (opand!=-1 && rx_buffer[15]=='/'){ 
			if(rx_buffer[16]=='X'){
				joystick_x = opand; Joystick_control(joystick_x, joystick_y);
				sprintf(msg, "//Set joystick X %d//", opand);
			}else if (rx_buffer[16]=='Y'){
				joystick_y = opand; Joystick_control(joystick_x, joystick_y);
				sprintf(msg, "//Set joystick Y %d//", opand);
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
		sTicks ++; if (sTicks>=1000){sprintf(msg, "D"); sTicks = 0;}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		
		
	}
}

int main(){
	motor_init();
	USART2_Init();
	strcpy(msg , "We are ready!");
	while(1){
		
	}
}



