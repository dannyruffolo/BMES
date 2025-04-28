#include <Servo.h>

//create servo objects
Servo base;
Servo shoulder;
Servo elbow;
Servo wrist;
Servo gripper;

//set pins
const int basePin = 5;
const int shoulderPin = 6;
const int elbowPin = 9;
const int wristPin = 10;
const int gripperPin = 11;

//initialize variables
const int minPWM = 500; //in microseconds
const int maxPWM[] = {2500, 2666}; // in microseconds

//indexes for maxPWM
const int baseIdx = 1;
const int shoulderIdx = 1;
const int elbowIdx = 0;
const int wristIdx = 0;
const int gripperIdx = 0;



void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);


 //set all pins to output mode
  pinMode(basePin, OUTPUT);
  pinMode(shoulderPin, OUTPUT);
  pinMode(elbowPin, OUTPUT);
  pinMode(wristPin, OUTPUT);
  pinMode(gripperPin, OUTPUT);

  //attach all pins as servos and initialize the min and max PWM ranges
  shoulder.attach(shoulderPin, minPWM, maxPWM[shoulderIdx]);
  base.attach(basePin, minPWM, maxPWM[baseIdx]);
  elbow.attach(elbowPin, minPWM, maxPWM[elbowIdx]);
  wrist.attach(wristPin, minPWM, maxPWM[wristIdx]);
  gripper.attach(gripperPin, minPWM, maxPWM[gripperIdx]);

  //move all servos to 0 degrees to start
  shoulder.write(0);
  base.write(0);
  elbow.write(0);
  wrist.write(0);
  gripper.write(0);
}

//HELPER FUNCTIONS


//input angle is the angle you want the shoulder to move to. newAngle accounts for the differences in max PWM with the servo.write() function.
void moveShoulder(int inputAngle){
  //crash program for invalid angles
  if(inputAngle < 0 || inputAngle > 270){
    exit(0);
  }

  int newAngle = map(inputAngle, 0, 270, 0, 180);
  shoulder.write(newAngle);
}

//a wrapper function for moveShoulder that goes slower 
void moveShoulderSlowly(int inputAngle){
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

    moveShoulder(newAngle);
    delay(tDelay);
    newAngle += sign;
  }

}

//input angle is the angle you want the base to move to. newAngle accounts for the differences in max PWM with the servo.write() function.
void moveBase(int inputAngle){
  //crash program for invalid angles
  if(inputAngle < 0 || inputAngle > 270){
    exit(0);
  }
  int newAngle = map(inputAngle, 0, 270, 0, 180);
  base.write(newAngle);
}

//a wrapper function for moveShoulder that goes slower 
void moveBaseSlowly(int inputAngle){
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
    moveBase(newAngle);
    delay(tDelay);
    newAngle += sign;
  }

}

void moveElbow(int inputAngle){
  //crash program for invalid angles

  if(inputAngle < 0 || inputAngle > 180){
    exit(0);
  }
  elbow.write(inputAngle);
}

void moveElbowSlowly(int inputAngle){
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
    moveElbow(newAngle);
    delay(tDelay);
    newAngle += sign;
  }

}

void moveWrist(int inputAngle){
  //crash program for invalid angles
  if(inputAngle < 0 || inputAngle > 180){
    exit(0);
  }
  wrist.write(inputAngle);
}

void moveWristSlowly(int inputAngle){
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
    moveWrist(newAngle);
    delay(tDelay);
    newAngle += sign;
  }

}

void moveGripper(int inputAngle){
  //crash program for invalid angles
  if(inputAngle < 0 || inputAngle > 180){
    exit(0);
  }
  gripper.write(inputAngle);
}

void moveGripperSlowly(int inputAngle){
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
    moveGripper(newAngle);
    delay(tDelay);
    newAngle += sign;
  }

}



void wave(){

  //initial positions
  moveBaseSlowly(0);
  moveShoulderSlowly(100);
  moveElbow(60);
  moveWrist(75);
  moveGripper(30);
  delay(1500);


  //move wrist up and down three times
  for(int i= 0; i<3; i++){
    moveWrist(30);
    delay(500);
    moveWrist(90);
    delay(500);
  }

  //pause for one second
  delay(1000);

  //rotate base
  moveBase(90);
  delay(100);

  //move up and down three times again
  for(int i= 0; i<3; i++){
    moveWrist(30);
    delay(500);
    moveWrist(90);
    delay(500);
  }

  //long pause
  delay(5000);
 
}

void pickUpObject(){
  moveBaseSlowly(0);
  moveShoulderSlowly(90);
  moveElbowSlowly(45);
  moveWristSlowly(45);
  moveGripperSlowly(180);


  moveShoulderSlowly(145);
  moveElbowSlowly(75);
  moveWristSlowly(120);
  moveGripperSlowly(0);

  delay(500);

  moveGripperSlowly(180);


  moveShoulderSlowly(90);
  moveElbowSlowly(45);
  moveWristSlowly(45);

  delay(500);

  moveBaseSlowly(90);

  moveShoulderSlowly(145);
  moveElbowSlowly(75);
  moveWristSlowly(120);

  delay(500);

  moveGripperSlowly(0);

  delay(1000);


}


void testShoulder(){

  moveShoulderSlowly(0);
  delay(500);
  moveShoulderSlowly(90);
  delay(500);
  moveShoulderSlowly(130);
  delay(500);
}


void loop() {
  wave();
  delay(1000);

  pickUpObject();
  delay(1000);
}