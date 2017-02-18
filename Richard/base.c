#include <kipr/botball.h>
#include "compass.h"
#include "move.h"



#define RIGHT 0
#define LEFT 1
#define TOPHAT 0
#define ET 1
#define CAMERA 8
#define WAIT_LIGHT 2


#define HAYBALE_CONF 0


#define NORMAL_SPEED 1500 // max speed


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
    printf("Program B: line follower\n");
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
            int black;
            int white;
            lineCalibrate(&black, &white);
            lineFollow(black, white);
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
