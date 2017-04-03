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
 * @param totalTime   How long the line follower can run (set to 12345 for touch
 *                    sensor mode)
 */
void lineFollow(int white, int black, int speed, int sensitivity, int totalTime);



#endif
