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

// Arm lengths (in cm) - adjust these values based on your arm's dimensions
float armLengths[] = {6 + (5.0/8.0), 4.75, 5 + (7.0/8.0)};  // Shoulder, Elbow, Wrist
float box[] = {-5.0, -3.0};  // Box dimensions (width, height)

// Structs for angles
struct Angles {
  float ShoulderVertical;
  float Elbow;
  float Hand;
};

// checkSafe function (Arduino translation)
bool checkSafe(Angles angles, float arm_lengths[], float box[], float &Hand_x, float &Hand_y) {
  float theta1 = angles.ShoulderVertical;
  float theta2 = angles.Elbow;
  float theta3 = angles.Hand;
  
  // Calculate the (x, y) position of the elbow joint
  float Elbow_x = arm_lengths[0] * cos(radians(theta1));
  float Elbow_y = arm_lengths[0] * sin(radians(theta1));

  // Calculate the (x, y) position of the wrist joint
  float Wrist_x = Elbow_x + arm_lengths[1] * cos(radians(theta1 + theta2));
  float Wrist_y = Elbow_y + arm_lengths[1] * sin(radians(theta1 + theta2));

  // Calculate the (x, y) position of the hand (end effector)
  Hand_x = Wrist_x + arm_lengths[2] * cos(radians(theta1 + theta2 + theta3));
  Hand_y = Wrist_y + arm_lengths[2] * sin(radians(theta1 + theta2 + theta3));

  // Check if the hand (end effector) is within the defined box
  bool condition_1 = Hand_x > -box[0]/2 && Hand_x < box[0]/2;
  bool condition_2 = Hand_y > 0 && Hand_y < box[1];

  return !(condition_1 && condition_2);  // If inside the box, it is not safe
}

void setup() {
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

  // Initialize all servos at 0 degrees
  shoulder.write(0);
  base.write(0);
  elbow.write(0);
  wrist.write(0);
  gripper.write(0);
}

void moveShoulder(int inputAngle) {
  Angles angles = {inputAngle, 0, 0};  // Assume other angles are 0 for safety check
  float Hand_x, Hand_y;

  if (checkSafe(angles, armLengths, box, Hand_x, Hand_y)) {
    Serial.println("Error: Invalid angle, arm would hit something!");
    exit(0);  // Stop the program if not safe
  }
  
  int newAngle = map(inputAngle, 0, 270, 0, 180);
  shoulder.write(newAngle);
}

void moveBase(int inputAngle) {
  Angles angles = {0, 0, 0};  // Assume other angles are 0 for safety check
  float Hand_x, Hand_y;

  if (checkSafe(angles, armLengths, box, Hand_x, Hand_y)) {
    Serial.println("Error: Invalid angle, arm would hit something!");
    exit(0);  // Stop the program if not safe
  }

  int newAngle = map(inputAngle, 0, 270, 0, 180);
  base.write(newAngle);
}

void moveElbow(int inputAngle) {
  Angles angles = {0, inputAngle, 0};  // Assume other angles are 0 for safety check
  float Hand_x, Hand_y;

  if (checkSafe(angles, armLengths, box, Hand_x, Hand_y)) {
    Serial.println("Error: Invalid angle, arm would hit something!");
    exit(0);  // Stop the program if not safe
  }

  elbow.write(inputAngle);
}

void moveWrist(int inputAngle) {
  Angles angles = {0, 0, inputAngle};  // Assume other angles are 0 for safety check
  float Hand_x, Hand_y;

  if (checkSafe(angles, armLengths, box, Hand_x, Hand_y)) {
    Serial.println("Error: Invalid angle, arm would hit something!");
    exit(0);  // Stop the program if not safe
  }

  wrist.write(inputAngle);
}

void moveGripper(int inputAngle) {
  if (inputAngle < 0 || inputAngle > 180) {
    exit(0);  // Crash the program for invalid input
  }
  gripper.write(inputAngle);
}

void loop() {
  // Example movement
  moveBase(90);  // Test base movement
  delay(500);
  moveShoulder(90);  // Test shoulder movement
  delay(500);
  moveElbow(45);  // Test elbow movement
  delay(500);
  moveWrist(75);  // Test wrist movement
  delay(500);
  moveGripper(30);  // Test gripper movement
  delay(500);
}