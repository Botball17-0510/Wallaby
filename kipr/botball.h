#ifndef BOTBALL_H
#define BOTBALL_H
/* bunch of useless botball stuff, just defining them here so gcc won't complain */

void mrp(int x, int y, int z);
void msleep(int x);
void set_servo_position(int x, int y);
void enable_servo(int x);
void disable_servo(int x);
int a_button();
int analog(int x);
void mav(int x, int y);
void off(int x);

#endif
