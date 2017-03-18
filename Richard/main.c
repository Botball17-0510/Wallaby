#include <kipr/botball.h>
#include "move.h"



#define ARM 0
#define CLAW 1


// arm positions (by degree)
#define ARM_135 100
#define ARM_90 350
#define ARM_45 650
#define ARM_30 1000
#define ARM_0 1200
#define ARM_N10 1350 // negative 10
#define ARM_N30 1500 // negative 30

#define CLAW_OPEN 500
#define CLAW_CLOSED 220



int main() {
    // calibrate line follower
    int white = 2220;
    int black = 3630;
    // default values should be fine
    // lineCalibrate(&white, &black);
    
    
    // setup arm position
    set_servo_position(ARM, ARM_135);
    enable_servo(ARM);
    // setup claw position
    enable_servo(CLAW);
    set_servo_position(CLAW, CLAW_CLOSED);
    msleep(1000);

    // move forward
    move(22, 1500);
    // set arm down
    set_servo_position(ARM, ARM_0);


    // line follow until top of ramp
    lineFollow(white, black, 400, 1400, 9000);
    // open claw
    set_servo_position(CLAW, CLAW_OPEN);
    move(3, 1500);
    // set arm down
    set_servo_position(ARM, ARM_N30);
    
    // move over top of ramp
    asyncMove(15, 700);
    // slowly move up arm
    slowServo(ARM, ARM_N30, ARM_N10, 1300);
    

    // line follow until planter bin (it's okay if it runs over a few poms)
    lineFollow(white, black, 400, 600, 2500);
    msleep(500);
    
    // raise arm
    slowServo(ARM, ARM_N10, ARM_90, 300);
    msleep(1000);
    // move back
    move(-13, 700);
    
    
    // REPEAT
    // set arm down
    set_servo_position(ARM, ARM_N10);
    msleep(500);
    // line follow again
    lineFollow(white, black, 400, 600, 2500);
    msleep(500);
    
    // close claw
    set_servo_position(CLAW, CLAW_CLOSED);
    msleep(500);
    // raise arm
    slowServo(ARM, ARM_N10, ARM_90, 1000);
    msleep(500);
    // move forward
    move(20, 700);
    msleep(500);
    // drop poms
    set_servo_position(ARM, ARM_45);
    msleep(500);
    set_servo_position(CLAW, CLAW_OPEN);
    msleep(1000);
    
    
    // clean up
    disable_servo(ARM);
    disable_servo(CLAW);

    return 0;
}