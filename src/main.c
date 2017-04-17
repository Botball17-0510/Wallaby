#include <kipr/botball.h>
#include "move.h"


#define ARM 0
#define CLAW 1

#define LIGHT 0 //LIGHT SENSOR FOR LIGHT START

// arm positions (by degree)
#define ARM_135 2047
#define ARM_90 1650
#define ARM_45 1300
#define ARM_30 1000
#define ARM_0 850
#define ARM_N10 720 // negative 10
#define ARM_N30 600 // negative 30

#define CLAW_OPEN 1200
#define CLAW_CLOSED 300

/* wire position
 * - one that goes around the light sensor: 2nd port and red facing
 * - the other one: 4th port and black facing
 * ignore RIGHT/LEFT #define on move.c
 */



 void dropDemPoms(int white, int black, int time) {
     // line follow until planter bin (it's okay if it runs over a few poms)
     printf("before line follow\n");
     // lineFollow(white, black, 400, 600, 12345); // touch mode
     lineFollow(white, black, 400, 600, time);	//3400 time
     printf("after line follow\n");


     // pick up poms
     slowServo(CLAW, CLAW_OPEN, CLAW_CLOSED, 700);
     msleep(500);

     mavDrive(-3, 500);
     msleep(500);

     slowServo(ARM, ARM_0, ARM_45, 1000);
     msleep(1000);

     mavDrive(18, 500);

     // drop poms
     set_servo_position(CLAW, CLAW_OPEN);
     msleep(500);
     slowServo(ARM, ARM_45, ARM_135, 500);
     msleep(1000);
 }




int main() {
    // wait until light
    // wait_for_light();


    // calibrate line follower
    int white = 1710;
    int black = 3640;
    // int white = 220;
    // int black = 3000;
    // default values should be fine
    // lineCalibrate(&white, &black);

  	set_servo_position(CLAW, CLAW_OPEN);	//enable servos to starting positions
    set_servo_position(ARM, ARM_135);
    enable_servo(CLAW);
  	enable_servo(ARM);

  	//wait_for_light(LIGHT);	//IMPLEMENT BEFORE TOURNAMENT
  	shut_down_in(119);
    mavDrive(-2, 700);	//square up and wait for Create
    //msleep(20000);


    // set initial claw position to grab poms
    set_servo_position(CLAW, CLAW_OPEN);
    set_servo_position(ARM, ARM_N10);
    msleep(700);

    mavDrive(2, 700);

    // grab pom
    slowServo(CLAW, CLAW_OPEN, CLAW_CLOSED, 10);
    msleep(400);
    slowServo(ARM, ARM_N10, ARM_90, 100);

    mavDrive(17, 1000);
    // set arm down
    slowServo(ARM, ARM_90, ARM_0, 500);
    mavDrive(3, 1000);


    // line follow until top of ramp
    lineFollow(white, black, 1000, 400, 3000);


    // move forward to push poms into claw area
    mavDrive(17, 1000);
    slowServo(CLAW, CLAW_CLOSED, CLAW_OPEN, 500);
  	msleep(500);


    // raise arm and move back
    slowServo(ARM, ARM_0, ARM_90, 800);
    msleep(1000);
    mavDrive(-9, 700);
    // move arm down
    slowServo(ARM, ARM_90, ARM_N10, 600);
    msleep(500);

    dropDemPoms(white, black, 3400);

    // move back
    mavDrive(-15, 700);	//PROBLEM - ROBOT VEERS. NOTE: 1500 power is outside of parameter limit
    msleep(500);
    slowServo(ARM, ARM_135, ARM_N10, 500);
    msleep(500);


    /* REPEAT */
    dropDemPoms(white, black, 2000);

    // clean up
    disable_servo(ARM);
    disable_servo(CLAW);

    return 0;
}
