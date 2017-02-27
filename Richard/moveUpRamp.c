#include <kipr/botball.h>
#include "move.h"



#define ARM 0

#define NORMAL_SPEED 1500 // max speed



int main() {
  set_servo_position(ARM, 1100);
  enable_servo(ARM);
  
  move(30, 700);
  
  set_servo_position(ARM, 1800);
  
  
  int white = 2220;
  int black = 3630;
  //lineCalibrate(&white, &black);
  
  lineFollow(white, black, 800, 300, 4500);
  
  move(10, 1500);
  
  lineFollow(white, black, 600, 400, 4000);
  
  return 0;
}