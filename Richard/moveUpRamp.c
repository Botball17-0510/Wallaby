#include <kipr/botball.h>
#include "move.h"



#define ARM 0
#define CLAW 1

// arm positions (by degree)
#define ARM_135 250
#define ARM_90 600
#define ARM_45 1000
#define ARM_0 1450
#define ARM_n10 1600 // negative 10

#define CLAW_OPEN 1300
#define CLAW_CLOSED 1650


#define NORMAL_SPEED 1500 // max speed



int main() {
    // setup arm
    set_servo_position(ARM, ARM_135);
    enable_servo(ARM);
    // setup claw
    set_servo_position(CLAW, CLAW_CLOSED);
    enable_servo(CLAW);
    msleep(700);

    // move forward
    move(30, 700);
    // set arm down
    set_servo_position(ARM, ARM_0);


    // line follower calibrate
    int white = 2220;
    int black = 3630;
    //lineCalibrate(&white, &black);

    // follow until top of ramp
    lineFollow(white, black, 400, 600, 5500);
    
    // open claw
    set_servo_position(CLAW, CLAW_OPEN);
    msleep(700);
    // move over top of ramp
    move(20, 700);
    // set arm down
    set_servo_position(ARM, ARM_n10);
    

    // line follow until planter bin
    lineFollow(white, black, 400, 600, 3000);
    
    // close claw
    set_servo_position(CLAW, CLAW_CLOSED);
    msleep(700);
    // raise arm
    set_servo_position(ARM, ARM_45);
    msleep(700);
    // move forward
    move(15, 1000);
    // drop poms
    set_servo_position(CLAW, CLAW_OPEN);
    msleep(700);
    
    
    disable_servo(ARM);
    disable_servo(CLAW);

    return 0;
}