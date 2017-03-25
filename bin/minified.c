#include <kipr/botball.h>
#define MINIFIED
#ifndef MOVE_H
#define MOVE_H



/**
 * Move the wallaby in a straight line.
 * @param inches Number of inches to move
 * @param speed Speed to move the robot, a number between 0 and 1500
*/
void move(int inches, int speed);


/**
 * Asynchronously move the wallaby.
 * @param inches Number of inches to move
 * @param speed Speed to move the robot, a number between 0 and 1500
*/
void asyncMove(int inches, int speed);


/**
 * Turns the robot in place by moving both wheels, then waiting until it is
 * finished
 * @param direction Degrees to rotate the robot, positive is right and negative is left
 * @param speed     Speed to move the robot, a number between 0 and 1500
 */
void turnInPlace(float direction, int speed);


/**
 * Turn the robot with one wheel.
 * @param direction Degrees to rotate the robot, positive is right and negative is left
 * @param outside   Set to 1 to use the outside wheel and 0 to use the inside wheel
 * @param speed     Speed to move the robot, a number between 0 and 1500
 */
void turnOneWheel(float direction, int outside, int speed);


/**
 * Moves the servo to a given position, taking a given amount of seconds
 * @param servo        Id of the servo to move, between 0 and 3
 * @param startPos     The starting position of the servo. If not at this
 *                     position, it will move to it in the fastest time possible.
 * @param goal         Position of the servo to move to, between 0 and 2047
 * @param milliseconds The amount of time to take to move to the position, in
 *                     milliseconds.
 */
void slowServo(int servo, int startPos, int goal, int milliseconds);


/**
 * Calibrate the line follower
 * @param white Returns the white value for the line follower
 * @param black Returns the black value for the line follower
 */
void lineCalibrate(int* white, int* black);


/**
 * Follow the left edge of the line.
 * @param white       White value for the line follower
 * @param black       Black value for the line follower
 * @param speed       Base speed the line follower is going at
 * @param sensitivity How much the speed can vary (from speed - sensitivity to
 *                    speed + sensitivity)
 * @param totalTime   How long the line follower can run
 */
void lineFollow(int white, int black, int speed, int sensitivity, int totalTime);



#endif
#ifndef MINIFIED
#include <kipr/botball.h>
#include "move.h"
#endif


#define ARM 0
#define CLAW 1


// arm positions (by degree)
#define ARM_135 2047
#define ARM_90 1650
#define ARM_45 1300
#define ARM_30 1000
#define ARM_0 900
#define ARM_N10 780 // negative 10
#define ARM_N30 600 // negative 30

#define CLAW_OPEN 1200
#define CLAW_CLOSED 550



int main() {
    printf("*******\n");
    printf("*******\n");
    printf("*******\n");
    printf("*******\n");
    printf("*******\n");
    printf("*******\n");
    printf("*******\n");
    printf("*******\n");
    printf("*******\n");
    return 0;
    
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
    slowServo(ARM, ARM_N10, ARM_90, 200);

    // move forward: slowly at first, then faster
    move(5, 800);
    move(10, 1500);
    // set arm down
    set_servo_position(ARM, ARM_30);
    msleep(300);
    set_servo_position(ARM, ARM_0);


    // line follow until top of ramp
    lineFollow(white, black, 400, 1400, 8500);
    // open claw
    set_servo_position(CLAW, CLAW_OPEN);
    move(10, 1500);
    // set arm down
    set_servo_position(ARM, ARM_N30);
    msleep(500);
    
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
    turnOneWheel(-5, 1, 700);
    

    // line follow until planter bin (it's okay if it runs over a few poms)
    lineFollow(white, black, 400, 600, 2000);
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
    move(5, 800);
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
#ifdef __GNUC__
#include "kipr/botball.h"
#endif


#include <stdlib.h>
#include <stdio.h>
#include <math.h>



#define RIGHT 0
#define LEFT 1
#define TOPHAT 0

#define INCH 166 // # of ticks to move 1 inch
#define MOVE_DEGREE 7.5 // # of ticks to move the robot 1 degree with both wheels



void move(int inches, int speed) {
    // clear position counter
    cmpc(RIGHT);
    cmpc(LEFT);
    
    // if want to go backwards, set speed negative
    if (inches < 0) speed *= -1;
    // distance still has to be negative, not positive
    
    mrp(RIGHT, speed, inches * INCH);
    mrp(LEFT, speed, inches * INCH);
    // a little less time if at max speed; figure out factor later
    msleep((float)abs(inches) * (float)INCH * (speed == 1500 ? 0.7 : 1)); // time take to move == # of ticks
    off(RIGHT);
    off(LEFT);
}



void asyncMove(int inches, int speed) {
    // if want to go backwards, set speed negative
    if (inches < 0) speed *= -1;
    // distance still has to be negative, not positive
    
    mrp(RIGHT, speed, inches * INCH);
    mrp(LEFT, speed, inches * INCH);
}



void turnInPlace(float direction, int speed) {
    mrp(RIGHT, speed, direction * MOVE_DEGREE);
    mrp(LEFT, speed, -direction * MOVE_DEGREE);
    msleep(fabsf(direction) * MOVE_DEGREE);
    off(RIGHT);
    off(LEFT);
}



void turnOneWheel(float direction, int outside, int speed) {
    if (outside) {
        // left moves on right turn
        // right moves on left turn
        mrp(RIGHT, speed, direction - fabsf(direction) * MOVE_DEGREE);
        mrp(LEFT, speed, direction + fabsf(direction) * MOVE_DEGREE);
    } else {
        // right moves backward on right turn
        // left moves backward on left turn
        mrp(RIGHT, speed, -direction - fabsf(direction) * MOVE_DEGREE);
        mrp(LEFT, speed, direction - fabsf(direction) * MOVE_DEGREE);
    }
    msleep(fabsf(direction * 2) * MOVE_DEGREE);
    off(RIGHT);
    off(LEFT);
}



void slowServo(int servo, int startPos, int goal, int milliseconds) {
    set_servo_position(servo, startPos);
    int goalRelative = goal - startPos;
    // how far to move the servo every time
    float oneMove = (float)(goalRelative) / (milliseconds / 5.0); // 5 ms delay
    //                      ^-- delta        ^-- number of loops
    
    
    float pos;
    for (pos = 0; fabsf(pos) < abs(goalRelative); pos += oneMove) {
        set_servo_position(servo, (int)(pos + startPos));
        msleep(5);
    }
    set_servo_position(servo, goal);
}



void lineCalibrate(int* white, int* black) {
    // calibrate white
    printf("Calibrate white\n");
    while(!a_button()) {
        msleep(30);
        printf("%d\n", analog(TOPHAT));
    }
    *white = analog(TOPHAT);
    printf("White: %d\n", *white);
    msleep(1000);

    // calibrate black
    printf("Calibrate black\n");
    while(!a_button()) {
        msleep(30);
        printf("%d\n", analog(TOPHAT));
    }
    *black = analog(TOPHAT);
    printf("Black: %d\n", *black);
    msleep(1000);
}



void lineFollow(int white, int black, int speed, int sensitivity, int totalTime) {
    int timeElapsed = 0;
    
    while(1) {
        // change to number between 0 and 1
        float light = (float)(analog(TOPHAT) - white) / (float)(black - white);
        // ensure light is within range from 0 to 1
        if (light < 0) light = 0;
        if (light > 1) light = 1;
        
        // move straight if between 0.4 and 0.6
        if (light > 0.4 && light < 0.6) light = 0.5;

        light -= 0.5; // change to number between -0.5 and 0.5

        mav(RIGHT, speed - (light * sensitivity));
        mav(LEFT, speed + (light * sensitivity));

        timeElapsed += 5;
        if (timeElapsed > totalTime) break;
        
        msleep(5);
    }
    // off(RIGHT);
    // off(LEFT);
}
