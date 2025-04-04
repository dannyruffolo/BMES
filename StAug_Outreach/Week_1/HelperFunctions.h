#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include <Arduino.h>
#include <Servo.h>

// Servo objects
extern Servo elbow;
extern Servo wrist;
extern Servo gripper;

// Servo pin assignments
extern const int elbowPin;
extern const int wristPin;
extern const int gripperPin;

// PWM settings
extern const int minPWM;
extern const int maxPWM;

// PWM index mappings
extern const int elbowIdx;
extern const int wristIdx;
extern const int gripperIdx;

// Function prototypes
void setupRobotArm();
void moveElbow(int inputAngle);
void moveWrist(int inputAngle);
void moveGripper(int inputAngle);
void level1_1();
void level1_2();
void level2_1();
void level2_2();
void level3();
void challenge_1 ();
void challenge_2 ();
void move_to_zero();

#endif  // HELPER_FUNCTIONS_H
