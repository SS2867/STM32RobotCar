#include "stm32f10x.h"                  // Device header


typedef struct{
	float target_val;
	float actual_val;
	float control_val;
	float err;
	float err_prev;
	float Kp, Ki, Kd;
	float integral;
} pid;

void pid_param_init(pid* ctrl){
	ctrl->err = 0;
	ctrl->err_prev = 0;
	ctrl->integral = 0;
	ctrl->control_val = 0;
}

void pid_set_p_i_d(pid* ctrl, float p, float i, float d){
	if(p>=0){ctrl->Kp = p;}
	if(i>=0){ctrl->Ki = i;}
	if(d>=0){ctrl->Kd = d;}
}

void set_pid_target(pid* ctrl, float target){
	ctrl->target_val = target;
}
float get_pid_target(pid* ctrl){
	return ctrl->target_val ;
}

float pid_run(pid* ctrl, float actual_val){
	ctrl->err_prev = ctrl->err;
	ctrl->actual_val = actual_val;
	ctrl->err = ctrl->target_val - actual_val;
	ctrl->integral += ctrl->err;
	ctrl->control_val = (ctrl->Kp)*(ctrl->err) + (ctrl->Ki)*(ctrl->integral) +
		(ctrl->Kd)*((ctrl->err) - (ctrl->err_prev));
	return ctrl->control_val;
}



