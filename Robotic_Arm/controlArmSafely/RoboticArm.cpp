#include <Arduino.h>
#include <Servo.h>
#include "RoboticArm.h"

Servo base, shoulder, elbow, wrist, gripper;
const int basePin = 5, shoulderPin = 6, elbowPin = 9, wristPin = 10, gripperPin = 11;
const int minPWM = 500;
const int maxPWM[] = {2500, 2666};
float armLengths[] = {6 + (5.0 / 8.0), 4.75, 5 + (7.0 / 8.0)};
float box[] = {-5.0, -3.0};

void setupRobotArm() {
  Serial.begin(9600);
  pinMode(basePin, OUTPUT);
  pinMode(shoulderPin, OUTPUT);
  pinMode(elbowPin, OUTPUT);
  pinMode(wristPin, OUTPUT);
  pinMode(gripperPin, OUTPUT);

  shoulder.attach(shoulderPin, minPWM, maxPWM[1]);
  base.attach(basePin, minPWM, maxPWM[1]);
  elbow.attach(elbowPin, minPWM, maxPWM[0]);
  wrist.attach(wristPin, minPWM, maxPWM[0]);
  gripper.attach(gripperPin, minPWM, maxPWM[0]);

  shoulder.write(0);
  base.write(0);
  elbow.write(0);
  wrist.write(0);
  gripper.write(0);
}

bool checkSafe(Angles angles, float arm_lengths[], float box[], float &Elbow_x, float &Elbow_y, float &Hand_x, float &Hand_y) {
  float theta1 = angles.ShoulderVertical;
  float theta2 = angles.Elbow;
  float theta3 = angles.Hand;

  float L_shoulder = arm_lengths[0];
  float L_elbow = arm_lengths[1];
  float L_wrist = arm_lengths[2];

  Elbow_x = -L_shoulder * cos(radians(theta1));
  Elbow_y = L_shoulder * sin(radians(theta1));

  float phi2 = 90 - theta1;
  float phi3 = theta2 - phi2;
  float Wrist_x = Elbow_x - L_elbow * sin(radians(phi3));
  float Wrist_y = Elbow_y - L_elbow * cos(radians(phi3));

  float phi4 = 90 - phi3;
  float phi5 = theta3 - phi4;
  float phi_w = 180 - phi5;

  Hand_x = Wrist_x - L_wrist * sin(radians(phi_w));
  Hand_y = Wrist_y - L_wrist * cos(radians(phi_w));

  float box_width = box[0];
  float box_height = box[1];

  bool condition_1 = (Hand_x > box_width && Hand_x < -box_width);
  bool condition_2 = (Hand_y > box_height && Hand_y < 0);
  bool condition_3 = (Hand_y < box_height);

  return !((condition_1 && condition_2) || condition_3);
}

void moveShoulder(int inputAngle) {
  Angles angles = {inputAngle, 0, 0};
  float Hand_x, Hand_y, Elbow_x, Elbow_y;
  if (!checkSafe(angles, armLengths, box, Elbow_x, Elbow_y, Hand_x, Hand_y)) {
    Serial.println("Error: Shoulder angle would cause collision");
    while (true);
  }
  int newAngle = map(inputAngle, 0, 270, 0, 180);
  shoulder.write(newAngle);
}

void moveBase(int inputAngle) {
  Angles angles = {0, 0, 0};
  float Hand_x, Hand_y, Elbow_x, Elbow_y;
  if (!checkSafe(angles, armLengths, box, Elbow_x, Elbow_y, Hand_x, Hand_y)) {
    Serial.println("Error: Base angle would cause collision");
    while (true);
  }
  int newAngle = map(inputAngle, 0, 270, 0, 180);
  base.write(newAngle);
}

void moveElbow(int inputAngle) {
  Angles angles = {0, inputAngle, 0};
  float Hand_x, Hand_y, Elbow_x, Elbow_y;
  if (!checkSafe(angles, armLengths, box, Elbow_x, Elbow_y, Hand_x, Hand_y)) {
    Serial.println("Error: Elbow angle would cause collision");
    while (true);
  }
  elbow.write(inputAngle);
}

void moveWrist(int inputAngle) {
  Angles angles = {0, 0, inputAngle};
  float Hand_x, Hand_y, Elbow_x, Elbow_y;
  if (!checkSafe(angles, armLengths, box, Elbow_x, Elbow_y, Hand_x, Hand_y)) {
    Serial.println("Error: Wrist angle would cause collision");
    while (true);
  }
  wrist.write(inputAngle);
}

void moveGripper(int inputAngle) {
  if (inputAngle < 0 || inputAngle > 180) {
    Serial.println("Error: Invalid gripper angle");
    while (true);
  }
  gripper.write(inputAngle);
}

void moveShoulderSlowly(int inputAngle) {
  int totalTime = 1000;
  int currentAngle = map(shoulder.read(), 0, 180, 0, 270);
  int delta = inputAngle - currentAngle;
  int sign = (delta > 0) - (delta < 0);
  delta = abs(delta);
  float tDelay = totalTime / float(delta);
  int newAngle = currentAngle;
  for (int i = 0; i < delta; i++) {
    moveShoulder(newAngle);
    delay(tDelay);
    newAngle += sign;
  }
}

void moveBaseSlowly(int inputAngle) {
  int totalTime = 1000;
  int currentAngle = map(base.read(), 0, 180, 0, 270);
  int delta = inputAngle - currentAngle;
  int sign = (delta > 0) - (delta < 0);
  delta = abs(delta);
  float tDelay = totalTime / float(delta);
  int newAngle = currentAngle;
  for (int i = 0; i < delta; i++) {
    moveBase(newAngle);
    delay(tDelay);
    newAngle += sign;
  }
}

void moveElbowSlowly(int inputAngle) {
  int totalTime = 1000;
  int currentAngle = elbow.read();
  int delta = inputAngle - currentAngle;
  int sign = (delta > 0) - (delta < 0);
  delta = abs(delta);
  float tDelay = totalTime / float(delta);
  int newAngle = currentAngle;
  for (int i = 0; i < delta; i++) {
    moveElbow(newAngle);
    delay(tDelay);
    newAngle += sign;
  }
}

void moveWristSlowly(int inputAngle) {
  int totalTime = 1000;
  int currentAngle = wrist.read();
  int delta = inputAngle - currentAngle;
  int sign = (delta > 0) - (delta < 0);
  delta = abs(delta);
  float tDelay = totalTime / float(delta);
  int newAngle = currentAngle;
  for (int i = 0; i < delta; i++) {
    moveWrist(newAngle);
    delay(tDelay);
    newAngle += sign;
  }
}

void moveGripperSlowly(int inputAngle) {
  int totalTime = 1000;
  int currentAngle = gripper.read();
  int delta = inputAngle - currentAngle;
  int sign = (delta > 0) - (delta < 0);
  delta = abs(delta);
  float tDelay = totalTime / float(delta);
  int newAngle = currentAngle;
  for (int i = 0; i < delta; i++) {
    moveGripper(newAngle);
    delay(tDelay);
    newAngle += sign;
  }
}