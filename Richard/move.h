#ifndef MOVE_H
#define MOVE_H


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



/*
 * Move the wallaby in a straight line.
 * int inches Number of inches to move
    int speed Speed to move the robot, a number between 0 and 1500
**/
void move(int inches, int speed) {
    mrp(RIGHT, speed, inches * INCH);
    mrp(LEFT, speed, inches * INCH);
    msleep(abs(inches) * INCH); // time take to move == # of ticks
    off(RIGHT);
    off(LEFT);
}



/**
 * Asynchronously move the wallaby.
 * @param (int) inches Number of inches to move
 * @param (int) speed Speed to move the robot, a number between 0 and 1500
*/
void asyncMove(int inches, int speed) {
    mrp(RIGHT, speed, inches * INCH);
    mrp(LEFT, speed, inches * INCH);
}



/**
 * Turns the robot in place by moving both wheels, then waiting until it is
 * finished
 *
 * float direction
 *     Degrees to rotate the robot, positive is right and negative is left
 * int speed
 *     Speed to move the robot, a number between 0 and 1500
*/
void turnInPlace(float direction, int speed) {
    mrp(RIGHT, speed, direction * MOVE_DEGREE);
    mrp(LEFT, speed, -direction * MOVE_DEGREE);
    msleep(fabsf(direction) * MOVE_DEGREE);
    off(RIGHT);
    off(LEFT);
}



/*
 * Turn the robot with one wheel.
 *
 * float direction
 *     Degrees to rotate the robot, positive is right and negative is left
 * int wheel
 *     Set to 1 to use the outside wheel and 0 to use the inside wheel
 * int speed
 *     Speed to move the robot, a number between 0 and 1500
**/
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



/**
 * Moves the servo to a given position, taking a given amount of seconds
 *
 * @param (int) servo Id of the servo to move, between 0 and 3
 * @param (int) startPos The starting position of the servo. If not at this
 * position, it will move to it in the fastest time possible.
 * @param (int) goal Position of the servo to move to, between 0 and 2047
 * @param (float) seconds The amount of time to take to move to the position, in
 * milliseconds.
 */
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



/*
    Calibrate the line follower
    
    int* white
        Returns the white value for the line follower
    int* black
        Returns the black value for the line follower
**/
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



/*
    Follow the left edge of the line.
    int white
        White value for the line follower
    int black
        Black value for the line follower
    int speed
        Base speed the line follower is going at
    int sensitivity
        How much the speed can vary (from speed - sensitivity to speed +
        sensitivity)
    int totalTime
        How long the line follower can run
**/
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
    off(RIGHT);
    off(LEFT);
}



#endif