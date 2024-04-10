#include "stm32f10x.h"                  // Device header
#include "Motor.h"
#include "utils.h"
#include <string.h>
#include <stdio.h>
#include "RemoteControl.h"
//#include "LineTracking.h"
#include "parking.h"
#include "PID_Speed.h"
#include "PID.h"

char rx_buffer[40] = "";
char msg[60] = "";
int joystick_x = 50;
int joystick_y = 50;
//extern pid speed_control_L;
extern pid speed_control_l;
extern pid speed_control_r;
int uptime = -2000;
void USART2_IRQHandler() {
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
		char ch = (unsigned char) USART_ReceiveData(USART2); 
		for(int i=0; i<38; i++){rx_buffer[i] = rx_buffer[i+1]; }
		rx_buffer[38] = ch;
		msg[0] = ch;
		
		if (1){//  (rx_buffer[17]=='/'){
			switch(rx_buffer[38]){
				case 'F': setMotor(3, 0, 100); break;
				case 'B': setMotor(3, 1, 100); break;
				//case 'L': adjust_l_r_lock_ticker=-100; break;
				case 'R': pid_param_init(&speed_control_l); pid_param_init(&speed_control_r); uptime = -2000;
					setMotor(3, 0, 0);  sprintf(msg, "\r\n\n==== Restart ====\n\r"); break;
				case 'S': uptime = -9.9e5; setMotor(3, 0, 0); break;
			}
		}
		if (rx_buffer[34]=='/'&&rx_buffer[35]=='k'&&rx_buffer[36]=='e'&&rx_buffer[37]=='y'){
			switch(rx_buffer[38]){
				case 'E': break;
				case 'K': setMotor(3, 0, 0); break;
			}			
		}
		
	}
	if(USART_GetITStatus(USART2, USART_IT_TC) != RESET) {
		if (msg[0] != 0){USART_SendData(USART2, msg[0]);}
		for(int i=0; i<59; i++){msg[i] = msg[i+1];}
		msg[59] = 0;
	}
}

int SYS_CLK_MULT = 9; // =9 if 72 MHz sys CLK, =1 if 8 MHz sys CLK


int sTicks = 0;
void TIM2_IRQHandler(void){
	if (TIM_GetITStatus(TIM2, TIM_IT_Update)!=RESET){
		uptime++; sTicks ++; if (sTicks%1==0){
			if(uptime<=0){TIM_SetCounter(TIM3, 0);TIM_SetCounter(TIM4, 0);}
			if (sTicks==(11*SYS_CLK_MULT)){
				if (uptime>0){PID_speed_control(); 
					sprintf(msg, "\n\r%5.1fs> L %4d/%2d(%+3d R %4d/%2d(%+3d #%.1f %.1f %.1f     ", uptime/1e3,
						(int)get_odom(1), (int)speed_control_l.actual_val, (int)speed_control_l.control_val, 
						(int)get_odom(2), (int)speed_control_r.actual_val, (int)speed_control_r.control_val,
						speed_control_l.integral, speed_control_r.integral, 0.); 
				}else {sprintf(msg,"\r%5.1fs>  ", uptime/1e3);}
				sTicks = 0;
			}
		
		
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

int main(){
	motor_init();
	USART2_Init();
	PID_speed_init();
	
	strcpy(msg , "We are ready!");
	
	
	setMotor(3, 0, 0);
	
	while(1){
		//lineTracking();
		//exhaustive_delay(2);
		
	}
}



