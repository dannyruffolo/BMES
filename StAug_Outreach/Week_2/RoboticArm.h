// RoboticArm.h
#ifndef ROBOTIC_ARM_H
#define ROBOTIC_ARM_H

#include <Arduino.h>
#include <Servo.h>

// Servo objects
extern Servo base;
extern Servo shoulder;
extern Servo elbow;
extern Servo wrist;
extern Servo gripper;

// Pin assignments
extern const int basePin;
extern const int shoulderPin;
extern const int elbowPin;
extern const int wristPin;
extern const int gripperPin;

// PWM configuration
extern const int minPWM;
extern const int maxPWM[];
extern const int baseIdx;
extern const int shoulderIdx;
extern const int elbowIdx;
extern const int wristIdx;
extern const int gripperIdx;

// Setup function
void setupRobotArm();

// Move functions
void moveShoulderFast(int inputAngle);
void moveBaseFast(int inputAngle);
void moveElbowFast(int inputAngle);
void moveWristFast(int inputAngle);
void moveGripperFast(int inputAngle);

void moveShoulder(int inputAngle);
void moveBase(int inputAngle);
void moveElbow(int inputAngle);
void moveWrist(int inputAngle);
void moveGripper(int inputAngle);

// Motion macros
void wave();
void pickUpObject();
void moveObject();

#endif // ROBOTIC_ARM_H
