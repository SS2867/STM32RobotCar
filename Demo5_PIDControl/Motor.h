#ifndef __MOTOR_H
#define __MOTOR_H
void motor_init(void);
void setMotor(int motor, int direction, int speed);
void Joystick_control(int x, int y);

#endif
