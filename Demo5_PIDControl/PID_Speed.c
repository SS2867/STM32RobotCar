#include "stm32f10x.h"                  // Device header
#include "PID.h"
#include "utils.h"
#include "Motor.h"

extern int SYS_CLK_MULT;
pid speed_control_L;
pid speed_control_l;
pid speed_control_r;
extern int uptime;

float KP = 10;
float KI = 5;
float KD = 0;
float SPEED_TARGET = 8;

void PID_speed_init(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*TIM_InternalClockConfig(TIM3);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1; 
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200*SYS_CLK_MULT - 1; // 100 us per tick
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	// encoder for motor 1 (TIM3 Ch3 *Ch4, PB1)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	// encoder for motor 2 (TIM3 *Ch1 Ch2, PA6)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	
	TIM_Cmd(TIM3, ENABLE);*/
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_Prescaler = 1-1;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = 65536-1;
	timerInitStructure.TIM_ClockDivision = 0;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &timerInitStructure);
	TIM_Cmd(TIM3, ENABLE);
	TIM_TIxExternalClockConfig(TIM3, TIM_TIxExternalCLK1Source_TI1, TIM_ICPolarity_BothEdge, 0xF);
	
	// pid params
	pid_param_init(&speed_control_L);
	pid_set_p_i_d(&speed_control_L,  3, .0, .0);
	set_pid_target(&speed_control_L, 0);
	
	pid_param_init(&speed_control_r);
	pid_set_p_i_d(&speed_control_r,  KP ,KI ,KD);
	set_pid_target(&speed_control_r, SPEED_TARGET);
	
	pid_param_init(&speed_control_l);
	pid_set_p_i_d(&speed_control_l,  KP ,KI ,KD );
	set_pid_target(&speed_control_l, SPEED_TARGET);
}

int get_odom(int motor){
	if (motor==1){
		return TIM_GetCounter(TIM3);
	}
	else if (motor==2){
		return TIM_GetCounter(TIM4);
	}
	else {return 0;}
}

/*float get_speed(int motor){
	int t;
	if (motor==1){
		t = abs(TIM_GetCapture3(TIM3) - TIM_GetCapture4(TIM3));
	}
	else if (motor==2){
		t = abs(TIM_GetCapture1(TIM3) - TIM_GetCapture2(TIM3));
	}
	else {return 0;}
	if (t>=32768){t = 65535-t;}
	return 5.*100/t;  // max speed = 100
}*/


int odom_l = 0; int odom_l_prev = 0;
int odom_r = 0; int odom_r_prev = 0;
void PID_speed_control(){
	int L = pid_run(&speed_control_L, 0);
	if (L>0){setMotor(0, 0, min(L, 100));} else {setMotor(0, 0, 0);}
	
	odom_l_prev = odom_l; odom_r_prev = odom_r;
	odom_l = get_odom(1); odom_r = get_odom(2); 
	
	//set_pid_target(&speed_control_l, uptime*SPEED_TARGET);
	int l = pid_run(&speed_control_l, (odom_l-odom_l_prev+(odom_l_prev>odom_l?65536:0)) );
	if (l>0){setMotor(1, 0, min(l, 100));} else {setMotor(1, 0, 0);}
	
	//set_pid_target(&speed_control_r, uptime*SPEED_TARGET);
	int r = pid_run(&speed_control_r, (odom_r-odom_r_prev+(odom_r_prev>odom_r?65536:0)));
	if (r>0){setMotor(2, 0, min(r, 100));} else {setMotor(2, 0, 0);}

}



