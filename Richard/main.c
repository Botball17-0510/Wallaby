#include <kipr/botball.h>
#include "move.h"



#define ARM 0
#define CLAW 1

// arm positions (by degree)
#define ARM_135 100
#define ARM_90 400
#define ARM_45 650
#define ARM_30 1000
#define ARM_0 1200
#define ARM_n10 1350 // negative 10

#define CLAW_OPEN 1300
#define CLAW_CLOSED 1800


#define NORMAL_SPEED 1500 // max speed



int main() {
    int i;
    
    // calibrate line follower
    int white = 2220;
    int black = 3630;
    // default values should be fine
    // lineCalibrate(&white, &black);
    
    
    // setup arm position
    set_servo_position(ARM, ARM_135);
    enable_servo(ARM);
    // setup claw position
    set_servo_position(CLAW, CLAW_CLOSED);
    enable_servo(CLAW);
    msleep(700);

    // move forward
    move(30, 700);
    // set arm down
    set_servo_position(ARM, ARM_0);


    // line follow until top of ramp
    lineFollow(white, black, 400, 600, 8000);
    
    // open claw
    set_servo_position(CLAW, CLAW_OPEN);
    msleep(700);
    // move over top of ramp (robot sometimes gets stuck here)
    move(15, 700);
    // set arm down
    set_servo_position(ARM, ARM_n10);
    msleep(700);
    

    // line follow until planter bin (it's okay if it runs over a few poms)
    lineFollow(white, black, 400, 600, 1500);
    
    // raise arm
    msleep(1000);
    for (i = 0; i < 45; i++) {
        set_servo_position(ARM, (ARM_0 - ARM_45) * i/45 + ARM_45);
        msleep(15);
    }
    msleep(1000);
    // move back
    move(-13, 700);
    
    // set arm down
    set_servo_position(ARM, ARM_n10);
    msleep(700);
    // line follow again
    lineFollow(white, black, 400, 600, 2000);
    
    // close claw
    msleep(1000);
    set_servo_position(CLAW, CLAW_CLOSED);
    msleep(1000);
    // raise arm
    for (i = 0; i < 45; i++) {
        set_servo_position(ARM, (ARM_0 - ARM_45) * i/45 + ARM_45);
        msleep(15);
    }
    set_servo_position(ARM, ARM_45);
    msleep(1500);
    // move forward
    move(5, 700);
    // drop poms
    set_servo_position(CLAW, CLAW_OPEN);
    msleep(700);
    
    
    // clean up
    disable_servo(ARM);
    disable_servo(CLAW);

    return 0;
}