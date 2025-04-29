#include "RoboticArm.h"

Servo base;
Servo shoulder;
Servo elbow;
Servo wrist;
Servo gripper;

const int basePin = 5;
const int shoulderPin = 6;
const int elbowPin = 9;
const int wristPin = 10;
const int gripperPin = 11;

const int minPWM = 500;
const int maxPWM[] = {2500, 2666};

const int baseIdx = 1;
const int shoulderIdx = 1;
const int elbowIdx = 0;
const int wristIdx = 0;
const int gripperIdx = 0;

void setupRobotArm() {
  Serial.begin(9600);

  pinMode(basePin, OUTPUT);
  pinMode(shoulderPin, OUTPUT);
  pinMode(elbowPin, OUTPUT);
  pinMode(wristPin, OUTPUT);
  pinMode(gripperPin, OUTPUT);

  shoulder.attach(shoulderPin, minPWM, maxPWM[shoulderIdx]);
  base.attach(basePin, minPWM, maxPWM[baseIdx]);
  elbow.attach(elbowPin, minPWM, maxPWM[elbowIdx]);
  wrist.attach(wristPin, minPWM, maxPWM[wristIdx]);
  gripper.attach(gripperPin, minPWM, maxPWM[gripperIdx]);

  base.write(0);
  shoulder.write(0);
  elbow.write(0);
  wrist.write(0);
  gripper.write(0);
}

//HELPER FUNCTIONS


//input angle is the angle you want the shoulder to move to. newAngle accounts for the differences in max PWM with the servo.write() function.
void moveShoulderFast(int inputAngle){
  if(inputAngle < 0 || inputAngle > 270){
    exit(0);
  }

  int newAngle = map(inputAngle, 0, 270, 0, 180);
  
  newAngle = round(newAngle * (8.0 / 9.0));

  shoulder.write(newAngle);
}

//a wrapper function for moveShoulder that goes slower 
void moveShoulder(int inputAngle){
  int totalTime = 1000; //hard coded to 2 seconds

  int currentAngle = map(shoulder.read(), 0, 180, 0, 270);
 // inputAngle = map(inputAngle, 0, 270, 0, 180);
  int delta = inputAngle - currentAngle;
  int sign; 
  int mappedAngle;

  sign = (delta > 0) - (delta < 0);

  delta = abs(delta);
  float tDelay = totalTime / float(delta);
  int newAngle = currentAngle;
  int i = 0;
  
  //delay(5000);
  for (i = 0; i < delta; i++) {

    moveShoulderFast(newAngle);
    delay(tDelay);
    newAngle += sign;
  }

}

//input angle is the angle you want the base to move to. newAngle accounts for the differences in max PWM with the servo.write() function.
void moveBaseFast(int inputAngle){
  //crash program for invalid angles
  if(inputAngle < 0 || inputAngle > 270){
    exit(0);
  }
  int newAngle = map(inputAngle, 0, 270, 0, 180);
  base.write(newAngle);
}

//a wrapper function for moveShoulder that goes slower 
void moveBase(int inputAngle){
  int totalTime = 1000; //hard coded to 2 seconds

  int currentAngle = map(base.read(), 0, 180, 0, 270);
  int delta = inputAngle - currentAngle;
  int sign; 

  if (delta > 0){
    sign = 1;
  }
  else if (delta < 0){
    sign = -1;
  }

  delta = abs(delta);
  float tDelay = totalTime / delta;
  int newAngle = currentAngle + sign;
  int i = 0;
  
  for (i = 0; i < delta; i++) {
    moveBaseFast(newAngle);
    delay(tDelay);
    newAngle += sign;
  }

}

void moveElbowFast(int inputAngle){
  //crash program for invalid angles

  if(inputAngle < 0 || inputAngle > 180){
    exit(0);
  }

  int newAngle = 0;
  if(inputAngle <= 90){
    newAngle = round((inputAngle * (100.0 - 10.0)) / 90.0 + 10.0);
  } else{
    newAngle = round(((inputAngle - 90) * (180 - 100)) / 90 + 100);
  }

  elbow.write(newAngle);
}

void moveElbow(int inputAngle){
  int totalTime = 500; //hard coded to 2 seconds

  int currentAngle = elbow.read();
  int delta = inputAngle - currentAngle;
  int sign; 

  if (delta > 0){
    sign = 1;
  }
  else if (delta < 0){
    sign = -1;
  }

  delta = abs(delta);
  float tDelay = totalTime / delta;
  int newAngle = currentAngle + sign;
  int i = 0;
  
  for (i = 0; i < delta; i++) {
    moveElbowFast(newAngle);
    delay(tDelay);
    newAngle += sign;
  }

}

void moveWristFast(int inputAngle){
  //crash program for invalid angles
  if(inputAngle < 0 || inputAngle > 180){
    exit(0);
  }

  int newAngle = round(((inputAngle * 75) / 90) + 50);
  wrist.write(newAngle);
}

void moveWrist(int inputAngle){
  int totalTime = 500; //hard coded to 2 seconds

  int currentAngle = wrist.read();
  int delta = inputAngle - currentAngle;
  int sign; 

  if (delta > 0){
    sign = 1;
  }
  else if (delta < 0){
    sign = -1;
  }

  delta = abs(delta);
  float tDelay = totalTime / delta;
  int newAngle = currentAngle + sign;
  int i = 0;
  
  for (i = 0; i < delta; i++) {
    moveWristFast(newAngle);
    delay(tDelay);
    newAngle += sign;
  }

}

void moveGripperFast(int inputAngle){
  //crash program for invalid angles
  if(inputAngle < 0 || inputAngle > 180){
    exit(0);
  }
  gripper.write(inputAngle);
}

void moveGripper(int inputAngle){
  int totalTime = 500; //hard coded to 2 seconds

  int currentAngle = gripper.read();
  int delta = inputAngle - currentAngle;
  int sign; 

  if (delta > 0){
    sign = 1;
  }
  else if (delta < 0){
    sign = -1;
  }

  delta = abs(delta);
  float tDelay = totalTime / delta;
  int newAngle = currentAngle + sign;
  int i = 0;
  
  for (i = 0; i < delta; i++) {
    moveGripperFast(newAngle);
    delay(tDelay);
    newAngle += sign;
  }

}

void wave(){
  moveBase(0);
  moveShoulder(100);
  moveElbow(60);
  moveWrist(75);
  moveGripper(30);
  delay(1500);
  for(int i=0; i<3; i++){
    moveWrist(30);
    delay(500);
    moveWrist(90);
    delay(500);
  }
  delay(1000);
  moveBase(90);
  delay(100);
  for(int i=0; i<3; i++){
    moveWrist(30);
    delay(500);
    moveWrist(90);
    delay(500);
  }
  delay(5000);
}

void pickUpObject(){
  moveBase(0);
  moveShoulder(90);
  moveElbow(45);
  moveWrist(45);
  moveGripper(180);
  moveShoulder(145);
  moveElbow(75);
  moveWrist(120);
  moveGripper(0);
  delay(500);
  moveGripper(180);
  moveShoulder(90);
  moveElbow(45);
  moveWrist(45);
  delay(500);
  moveBase(90);
  moveShoulder(145);
  moveElbow(75);
  moveWrist(120);
  delay(500);
  moveGripper(0);
  delay(1000);
}

void moveObject(){
  moveBase(0);
  moveGripper(0);
  delay(1000);
  moveShoulder(130);
  moveElbow(32);
  moveWrist(105);
  delay(1000);
  moveGripper(180);
  delay(1000);
  moveShoulder(80);
  moveWrist(50);
  moveBase(120);
  delay(1000);
  moveShoulder(130);
  moveWrist(105);
  delay(1000);
  moveGripper(0);
  delay(1000);
  moveShoulder(80);
  moveWrist(50);
  delay(1000);
}