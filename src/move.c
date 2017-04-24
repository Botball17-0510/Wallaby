#ifdef __GNUC__
#include "kipr/botball.h"
#endif


#include <stdlib.h>
#include <stdio.h>
#include <math.h>



// #define WHITE 1710
// #define BLACK 3640
#define WHITE 2600
#define BLACK 3700


#define RIGHT 2
#define LEFT 1
#define TOPHAT 0
#define TOUCH 1

#define INCH 166 // # of ticks to move 1 inch
#define MOVE_DEGREE 7.5 // # of ticks to move the robot 1 degree with both wheels



void move(int inches, int speed) {
    // clear position counter
    // cmpc(RIGHT);
    // cmpc(LEFT);

    // if want to go backwards, set speed negative
    if (inches < 0) speed *= -1;
    // distance still has to be negative, not positive

    mrp(RIGHT, speed, inches * INCH);
    mrp(LEFT, speed, inches * INCH);
    // a little less time if at max speed; figure out factor later
    msleep((float)abs(inches) * (float)INCH * (speed == 1500 ? 0.7 : 1.1)); // time take to move == # of ticks

  	//msleep(500);	//TEST - mrp may not have completed by the first msleep
  	freeze(RIGHT);
    freeze(LEFT);
}

void motDrive(float distance, int speed) {	//only works for forward
	long leftDistance = gmpc(LEFT) + (distance * INCH);	//INCH TO TICKS
	long rightDistance = gmpc(RIGHT) + (distance * INCH);
	motor(LEFT, speed);
	motor(RIGHT, speed);
	while(gmpc(LEFT) < leftDistance && gmpc(RIGHT) < rightDistance){
		if (gmpc(LEFT) >= leftDistance) {
          	freeze(LEFT);
        }
		if (gmpc(RIGHT) >= rightDistance) {
          	freeze(RIGHT);
        }
	}
	freeze(LEFT);
  	freeze(RIGHT);
}

void mavDrive(float distance, int speed) {
	long leftDistance = gmpc(LEFT) + (distance * INCH);	//INCH TO TICKS
	long rightDistance = gmpc(RIGHT) + (distance * INCH);
  	if(distance < 0) {
     	speed = speed * -1;
    }
  	mav(LEFT, speed);
	mav(RIGHT, speed);	//correcting left veer
  	if(distance > 0) {
    	while(gmpc(LEFT) < leftDistance && gmpc(RIGHT) < rightDistance){
			if (gmpc(LEFT) >= leftDistance) {
          		freeze(LEFT);
        	}
			if (gmpc(RIGHT) >= rightDistance) {
          		freeze(RIGHT);
        	}
		}
    }
  	else {
      	while(gmpc(LEFT) > leftDistance && gmpc(RIGHT) > rightDistance){
			if (gmpc(LEFT) <= leftDistance) {
          		freeze(LEFT);
        	}
			if (gmpc(RIGHT) <= rightDistance) {
          		freeze(RIGHT);
        	}
		}
    }
	freeze(LEFT);
  	freeze(RIGHT);
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



void lineFollow(int speed, int sensitivity, int totalTime) {

    int timeElapsed = 0;

    while(1) {
        // change to number between 0 and 1
        float light = (float)(analog(TOPHAT) - WHITE) / (float)(BLACK - WHITE);
        // ensure light is within range from 0 to 1
        if (light < 0) light = 0;
        if (light > 1) light = 1;

        // move straight if between 0.4 and 0.6
        if (light > 0.4 && light < 0.6) light = 0.5;

        light -= 0.5; // change to number between -0.5 and 0.5

        mav(LEFT, speed - (light * sensitivity));
        mav(RIGHT, speed + (light * sensitivity));

        timeElapsed += 5;
        if (timeElapsed > totalTime) break;

        if (digital(TOUCH)) break;

        msleep(5);
    }
    //off(RIGHT);
    //off(LEFT);
  	freeze(RIGHT);
  	freeze(LEFT);
}



void lineCalibrate(int* white, int* black) {
    // calibrate white
    while(!right_button()) {
        msleep(10);
        printf("White: %d\n", analog(TOPHAT));
    }
    *white = analog(TOPHAT);
    msleep(500);

    // calibrate black
    while(!right_button()) {
        msleep(10);
        printf("Black: %d\n", analog(TOPHAT));
    }
    *black = analog(TOPHAT);

    printf("White: %d, black: %d", *white, *black);
    printf("Press right button to continue:");
    while(!right_button()) {
        msleep(10);
    }
}
