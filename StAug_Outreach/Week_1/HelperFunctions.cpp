#include "HelperFunctions.h"

// Create servo objects
Servo elbow;
Servo wrist;
Servo gripper;

// Set pins
const int elbowPin = 9;
const int wristPin = 10;
const int gripperPin = 11;

// Initialize variables
const int minPWM = 500;  // in microseconds
const int maxPWM = 2500;  // in microseconds

// Function to initialize servos
void setupRobotArm() {
    Serial.begin(9600);

    // Set all pins to output mode
    pinMode(elbowPin, OUTPUT);
    pinMode(wristPin, OUTPUT);
    pinMode(gripperPin, OUTPUT);

    // Attach all servos with appropriate PWM ranges
    elbow.attach(elbowPin, minPWM, maxPWM);
    wrist.attach(wristPin, minPWM, maxPWM);
    gripper.attach(gripperPin, minPWM, maxPWM);

    // Move all servos to 0 degrees to start
    elbow.write(0);
    wrist.write(0);
    gripper.write(0);
}

//Servo moving helper functions
void moveElbow(int inputAngle){
  //crash program for invalid angles
  if(inputAngle < 0 || inputAngle > 180){
    exit(0);
  }
  elbow.write(inputAngle);
}

void moveWrist(int inputAngle){
  //crash program for invalid angles
  if(inputAngle < 0 || inputAngle > 180){
    exit(0);
  }
  wrist.write(inputAngle);
}

void moveGripper(int inputAngle){
  //crash program for invalid angles
  if(inputAngle < 0 || inputAngle > 180){
    exit(0);
  }
  gripper.write(inputAngle);
}

//preset movement function
void level1_1() {
  moveElbow(0);

  moveElbow(180);
}

void level1_2() {
  moveElbow(0);
  moveWrist(0);
  moveGripper(0);

  moveElbow(90);

  moveWrist(120);

  moveGripper(160);
}

void level2_1() {
  moveElbow(0);
  moveWrist(0);
  delay(1000);

  moveElbow(130);
  moveWrist(160);
  delay(1000);
}

void level2_2() {
  moveElbow(0);
  moveGripper(0);
  moveWrist(0);

  delay(1000);
  moveElbow(90);
  
  delay(2000);
  moveWrist(120);

  delay(3000);
  moveGripper(175);

  delay(500);
}

void level3() {
  moveElbow(0);
  moveWrist(180);
  delay(500);

  moveElbow(180);
  moveWrist(0);
  delay(500);
}

void challenge_1 () {
  int h = 0;
  int i = 0;
  int j = 0;
  int k = 0;
  
  for(h = 0; h <= 280; h += 10) {
    // Move the gripper (i) if it's less than 180
    if(i < 180) {
      i += 10;
      moveGripper(i);
      delay(100);  // Delay to give time for the servo to move
    }

    // Move the wrist (j) if gripper (i) is at least 50
    if(i >= 50 && j < 180) {
      j += 10;
      moveWrist(j);
      delay(100);
    }

    // Move the elbow (k) if wrist (j) is at least 50
    if(j >= 50) {
      k += 10;
      moveElbow(k);
      delay(100);
    }
  }

  // Move servos in reverse direction with independent movement
  for(h = 280; h >= 0; h -= 10) {
    if(k > 0) {
      k -= 10;
      moveElbow(k);
      delay(100);
    }

    if(j > 50) {
      j -= 10;
      moveWrist(j);
      delay(100);
    }

    if(i > 0) {
      i -= 10;
      moveGripper(i);
      delay(100);
    }
  }
}

void challenge_2 () {
  
}

void move_to_zero(){
  delay(1000);
  moveElbow(0);
  moveWrist(0);
  moveGripper(0);
  delay(1000);
}
