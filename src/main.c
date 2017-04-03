#include <kipr/botball.h>
#include "move.h"


#define ARM 0
#define CLAW 1


// arm positions (by degree)
#define ARM_135 2047
#define ARM_90 1650
#define ARM_45 1300
#define ARM_30 1000
#define ARM_0 850
#define ARM_N10 750 // negative 10
#define ARM_N30 600 // negative 30

#define CLAW_OPEN 1200
#define CLAW_CLOSED 400

/* wire position
 * - one that goes around the light sensor: left port and black facing
 * - the other one: right port and red facing
 */



 void dropDemPoms() {
     int white = 2220;
     int black = 3630;
     
     
     // line follow until planter bin (it's okay if it runs over a few poms)
     printf("before line follow\n");
     // lineFollow(white, black, 400, 600, 12345); // touch mode
     lineFollow(white, black, 400, 600, 3200);
     printf("after line follow\n");
     
     
     // pick up poms
     slowServo(CLAW, CLAW_OPEN, CLAW_CLOSED, 500);
     msleep(500);
     slowServo(ARM, ARM_0, ARM_45, 1000);
     msleep(1000);
     
     move(10, 500);
     
     // drop poms
     set_servo_position(CLAW, CLAW_OPEN);
     msleep(500);
     slowServo(ARM, ARM_45, ARM_135, 500);
     msleep(1000);
 }




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
    
    move(2, 700);
    
    // grab pom
    slowServo(CLAW, CLAW_OPEN, CLAW_CLOSED, 10);
    msleep(400);
    slowServo(ARM, ARM_N10, ARM_90, 100);

    move(17, 1500);
    // set arm down
    slowServo(ARM, ARM_90, ARM_0, 500);
    move(3, 1500);


    // line follow until top of ramp
    lineFollow(white, black, 400, 1200, 8400);
    
    
    move(13, 1000);
    set_servo_position(CLAW, CLAW_OPEN);
    
    
    // raise arm and move back
    slowServo(ARM, ARM_N10, ARM_90, 1000);
    msleep(1000);
    move(-8, 700);
    // move arm down
    slowServo(ARM, ARM_90, ARM_N10, 1000);
    msleep(500);
    
    // turn left a bit to get light sensor on line
    turnOneWheel(-10, 1, 700);
    
    dropDemPoms();
    
    // move back
    move(-25, 1500);
    msleep(500);
    set_servo_position(ARM, ARM_0);
    
    
    /* REPEAT */
    dropDemPoms();
    
    // clean up
    disable_servo(ARM);
    disable_servo(CLAW);

    return 0;
}
