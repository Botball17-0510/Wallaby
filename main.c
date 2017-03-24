#include <kipr/botball.h>
#include "move.h"



#define ARM 0
#define CLAW 1


// arm positions (by degree)
#define ARM_135 100
#define ARM_90 350
#define ARM_45 650
#define ARM_30 1000
#define ARM_0 1300
#define ARM_N10 1380 // negative 10
#define ARM_N30 1500 // negative 30

#define CLAW_OPEN 600
#define CLAW_CLOSED 200



int main() {
    // calibrate line follower
    int white = 2220;
    int black = 3630;
    // default values should be fine
    // lineCalibrate(&white, &black);
    
    
    // set initial claw position to grab poms
    set_servo_position(CLAW, CLAW_OPEN);
    set_servo_position(ARM, ARM_N10);
    enable_servo(CLAW);
    enable_servo(ARM);
    msleep(700);
    
    move(4, 700);
    
    // grab pom
    slowServo(CLAW, CLAW_OPEN, CLAW_CLOSED, 10);
    msleep(400);
    slowServo(ARM, ARM_N10, ARM_90, 200);

    // move forward: slowly at first, then faster
    move(5, 800);
    move(10, 1500);
    // set arm down
    set_servo_position(ARM, ARM_0);


    // line follow until top of ramp
    lineFollow(white, black, 400, 1400, 8500);
    move(10, 1500);
    // set arm down
    set_servo_position(ARM, ARM_N30);
    msleep(500);
    // open claw
    set_servo_position(CLAW, CLAW_OPEN);
    
    // move over top of ramp
    asyncMove(10, 700);
    // slowly move up arm
    slowServo(ARM, ARM_N30, ARM_N10, 1300);
    msleep(500);
    
    // move back a bit
    asyncMove(-10, 700);
    slowServo(ARM, ARM_N10, ARM_90, 1000);
    msleep(1000);
    // move arm down and move forward
    slowServo(ARM, ARM_90, ARM_N10, 1000);
    msleep(500);
    move(10, 700);
    
    // turn left a bit to get light sensor on line
    turnOneWheel(-25, 1, 700);
    

    // line follow until planter bin (it's okay if it runs over a few poms)
    lineFollow(white, black, 400, 600, 2700);
    msleep(500);
    // move back a bit
    move(-3, 700);
    
    // close claw
    slowServo(CLAW, CLAW_OPEN, CLAW_CLOSED, 400);
    msleep(800);
    // raise arm
    slowServo(ARM, ARM_N10, ARM_45, 1000);
    msleep(1000);
    
    // move forward
    move(10, 800);
    msleep(500);
    // drop poms
    set_servo_position(CLAW, CLAW_OPEN);
    msleep(500);
    set_servo_position(ARM, ARM_135);
    msleep(500);
    
    // move back
    move(-20, 700);
    
    
    // REPEAT
    // set arm down
    set_servo_position(ARM, ARM_N10);
    msleep(500);
    // line follow again
    lineFollow(white, black, 400, 600, 1000);
    msleep(500);
    // move back a bit
    // move(-3, 700);
    
    // close claw
    set_servo_position(CLAW, CLAW_CLOSED);
    msleep(500);
    // raise arm
    slowServo(ARM, ARM_N10, ARM_45, 1000);
    msleep(1000);
    // move forward
    move(5, 800);
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