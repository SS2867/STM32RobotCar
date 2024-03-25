#ifndef __PID_SPEED_H
#define __PID_SPEED_H

void PID_speed_init(void);
float get_speed(int motor);
void PID_speed_control(void);

#endif
