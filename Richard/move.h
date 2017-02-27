#ifndef MOVE_H
#define MOVE_H



#define RIGHT 0
#define LEFT 1
#define TOPHAT 0

#define INCH 166 // # of ticks to move 1 inch
#define MOVE_DEGREE 7.5 // # of ticks to move the robot 1 degree with both wheels

#define NORMAL_SPEED 1500 // max speed


#define TRUE 1
#define FALSE 0


/*
    Move the wallaby in a straight line.
    
    int inches
        number of inches to move
    int speed
        speed to move at
**/
void move(int inches, int speed) {
    mrp(RIGHT, speed, inches * INCH);
    mrp(LEFT, speed, inches * INCH);
    msleep(abs(inches) * INCH); // time take to move == # of ticks
}



/*
    Turns the robot in place by moving both wheels, then waiting until it is
    finished
    
    float direction
        degrees to rotate the robot, positive is right and negative is left
    int speed
        speed to move the robot, a number between 0 and 1500
**/
void turnInPlace(float direction, int speed) {
    mrp(RIGHT, speed, direction * MOVE_DEGREE);
    mrp(LEFT, speed, -direction * MOVE_DEGREE);
    msleep(abs(direction) * MOVE_DEGREE);
}



/*
    Turn the robot with one wheel.
    
    float direction
        degrees to rotate the robot, positive is right and negative is left
    int wheel
        1 uses outside wheel, 0 uses inside wheel
    int speed
        speed to move the robot, a number between 0 and 1500
**/
void turnOneWheel(float direction, int outside, int speed) {
    if (outside) {
        // left moves on right turn
        // right moves on left turn
        mrp(RIGHT, speed, direction - abs(direction) * MOVE_DEGREE);
        mrp(LEFT, speed, direction + abs(direction) * MOVE_DEGREE);
    } else {
        // right moves backward on right turn
        // left moves backward on left turn
        mrp(RIGHT, speed, -direction - abs(direction) * MOVE_DEGREE);
        mrp(LEFT, speed, direction - abs(direction) * MOVE_DEGREE);
    }
    msleep(abs(direction * 2) * MOVE_DEGREE);
}



/*
    Move the servo to a position (warning: delays 0.1 seconds)
    int servo
        id of the servo to move
    int position
        position to move the servo, between
**/
void setServo(int servo, int position) {
    enable_servo(servo);
    set_servo_position(servo, position);
    msleep(100);
    disable_servo(servo);
}



/*
    Calibrate the line follower
    int* white
        the white value for the line follower
    int* black
        the black value for the line follower
**/
void lineCalibrate(int* white, int* black) {
    // calibrate white (< black)
    printf("Calibrate white\n");
    while(!a_button()) {
        msleep(30);
        printf("%d\n", analog(TOPHAT));
    }
    *white = analog(TOPHAT);
    printf("White: %d\n", *white);
    msleep(1000);

    // calibrate black (> white)
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
    Follow the right edge of the line.
    int white
        White value for the line follower
    int black
        Black value for the line follower
    int speed
        base speed the line follower is going at
    int sensitivity
        how much the speed can vary
    int totalTime
        how long the line follower can run
**/
void lineFollow(int white, int black, int speed, int sensitivity, int totalTime) {
    int timeElapsed = 0;
    
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
        timeElapsed += 5;
        if (timeElapsed > totalTime) {
            break;
        }
    }
}




#endif