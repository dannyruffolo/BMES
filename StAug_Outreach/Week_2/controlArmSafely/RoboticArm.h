#ifndef ROBOTIC_ARM_H
#define ROBOTIC_ARM_H

#include <Servo.h>

struct Angles {
  float ShoulderVertical;
  float Elbow;
  float Hand;
};

extern Servo base, shoulder, elbow, wrist, gripper;
extern const int basePin, shoulderPin, elbowPin, wristPin, gripperPin;
extern const int minPWM;
extern const int maxPWM[];
extern float armLengths[];
extern float box[];

void setupRobotArm();
bool checkSafe(Angles angles, float arm_lengths[], float box[], float &Elbow_x, float &Elbow_y, float &Hand_x, float &Hand_y);

void moveBase(int inputAngle);
void moveShoulder(int inputAngle);
void moveElbow(int inputAngle);
void moveWrist(int inputAngle);
void moveGripper(int inputAngle);

void moveBaseSlowly(int inputAngle);
void moveShoulderSlowly(int inputAngle);
void moveElbowSlowly(int inputAngle);
void moveWristSlowly(int inputAngle);
void moveGripperSlowly(int inputAngle);

#endif