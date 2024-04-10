#ifndef __PID_SPEED_H
#define __PID_SPEED_H

void PID_speed_init(void);
int get_odom(int motor);
//float get_speed(int motor);
void PID_speed_control(void);


#endif
