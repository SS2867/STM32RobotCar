#ifndef __PID_H
#define __PID_H

typedef struct{
	float target_val;
	float actual_val;
	float control_val;
	float err;
	float err_prev;
	float Kp, Ki, Kd;
	float integral;
} pid;

void pid_param_init(pid *ctrl);
void pid_set_p_i_d(pid *ctrl, float p, float i, float d);
void set_pid_target(pid *ctrl, float target);
float get_pid_target(pid *ctrl);
float pid_run(pid *ctrl, float actual_val);




#endif

