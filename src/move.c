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
