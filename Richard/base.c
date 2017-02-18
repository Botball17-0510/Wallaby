#include <kipr/botball.h>
#include "compass.h"



#define RIGHT 0
#define LEFT 1
#define TOPHAT 0
#define ET 1
#define CAMERA 8
#define WAIT_LIGHT 2


#define HAYBALE_CONF 0


#define NORMAL_SPEED 1500 // max speed

#define INCH 166 // # of ticks to move 1 inch
#define SLEEP_INCH 166 // time to move 1 inch

#define MOVE_DEGREE 7.5 // ticks to move for 1 degree
#define SLEEP_DEGREE 7.5 // time to move 1 inch


#define TRUE 1
#define FALSE 0

// imu_set_compass_params(2.702, -62.319, 143.669, -0.062, -0.009, 1.002, 1.004)




void setServo(int servo, int position) {
    enable_servo(servo);
    set_servo_position(servo, position);
    msleep(100);
    disable_servo(servo);
}



// move given an amount of inches
void move(int inches) {
    mrp(RIGHT, NORMAL_SPEED, inches * INCH);
    mrp(LEFT, NORMAL_SPEED, inches * INCH);
    msleep(abs(inches) * SLEEP_INCH); // time take to move == # of ticks
}



// direction is in degrees negative to go left positive to go right
// turns in place (moves both wheels)
void turnInPlace(float direction, int speed) {
    mrp(RIGHT, NORMAL_SPEED, direction * MOVE_DEGREE);
    mrp(LEFT, NORMAL_SPEED, -direction * MOVE_DEGREE);
    msleep(abs(direction) * SLEEP_DEGREE); // time take to move == # of ticks
}



// direction is in degrees negative to go left positive to go right
// turns only one wheel
void turnOneWheel(float direction, int outside) {
    // dont multiply by two b/c you need to move more for one wheel
    mrp(RIGHT, NORMAL_SPEED, abs(direction + direction) * MOVE_DEGREE);
    mrp(LEFT, NORMAL_SPEED, abs(direction - direction) * MOVE_DEGREE);
    msleep(abs(direction * 2) * SLEEP_DEGREE); // time take to move == # of ticks
}



void lineFollow() {
    // calibrate white (< black)
    printf("Calibrate white\n");
    while(!a_button()) {
        msleep(30);
        printf("%d\n", analog(TOPHAT));
    }
    int white = analog(TOPHAT);
    printf("White: %d\n", white);
    msleep(1000);
    
    // calibrate black (> white)
    printf("Calibrate black\n");
    while(!a_button()) {
        msleep(30);
        printf("%d\n", analog(TOPHAT));
    }
    int black = analog(TOPHAT);
    printf("Black: %d\n", black);
    msleep(1000);
    
    
    // recommended sensitivity >= speed
    const int speed = 700; // base speed; following line going at this speed
    const int sensitivity = 800; // how much the speed can change by
    // this means that speed can at most be (speed + sensitivity) and at least
    // be (speed - sensitivity)
    
    printf("Starting line follower\n");
    while(TRUE) {
        float light = (float)(analog(TOPHAT) - white) / (float)black; // change to number between 0 and 1
        // ensure light is within range from 0 to 1
        if (light < 0) light = 0;
        if (light > 1) light = 1;
        
        light -= 0.5; // change to number between -0.5 and 0.5
        
        // follows on left border
        mav(RIGHT, speed + (light * sensitivity));
        mav(LEFT, speed - (light * sensitivity));
        
        msleep(5);
    }
}



void activity1() {
    // turns in place
    mav(RIGHT, 500);
    mav(LEFT, -500);
    unsigned short distanceData[360];
    int i = 0;
    
    while(i < 360) {
        i++;
        int dis = analog(ET);
        distanceData[i] = i;
        
        printf("%d\n", dis);
        msleep(12.96);
    }
    
    ao();
}



void calibrate_compass() {
    mav(RIGHT, -300);
    mav(LEFT, 300);
    imu_calibrate_compass();
}



void activityCamera() {
    camera_open();
    while(TRUE) {
        camera_update();
        if (get_object_count(HAYBALE_CONF) == 0) {
            printf("No object dected\n");
        } else {
            if (get_object_center_x(HAYBALE_CONF, 0) < 80) {
                printf("left\n");
            } else  {
                printf("right\n");
            }
        }
    }
    camera_close();
}



int main() {
    // wait_for_light(WAIT_LIGHT);
    // shut_down_in(119);
    printf("---------------------------------------------------------\n");
    printf("Select a program\n");
    printf("Program A: camera\n");
    printf("Program B: proximity sensor\n");
    printf("Program C: compass\n");
    printf("---------------------------------------------------------\n");
    
    while(TRUE) {
        if (a_button()) {
            printf("Please wait...\n");
            msleep(500);
            printf("Starting program A!\n");
            activityCamera();
            break;
        }
        if (b_button()) {
            printf("Please wait...\n");
            msleep(500);
            printf("Starting program B!\n");
            activity1();
            break;
        }
        if (c_button()) {
            printf("Please wait...\n");
            msleep(500);
            printf("Starting program C!\n");
            calibrate_compass();
            break;
        }
        msleep(50);
    }
    
    printf("program finished\n");
    return 0;
}
