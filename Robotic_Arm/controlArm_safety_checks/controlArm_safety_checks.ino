#include <Servo.h>

// Create servo objects
Servo base;
Servo shoulder;
Servo elbow;
Servo wrist;
Servo gripper;

// Set pins
const int basePin = 5;
const int shoulderPin = 6;
const int elbowPin = 9;
const int wristPin = 10;
const int gripperPin = 11;

// Initialize PWM variables
const int minPWM = 500; // in microseconds
const int maxPWM[] = {2500, 2666}; // in microseconds

// Indexes for maxPWM
const int baseIdx = 1;
const int shoulderIdx = 1;
const int elbowIdx = 0;
const int wristIdx = 0;
const int gripperIdx = 0;

// Arm lengths (in cm) – adjust these values based on your arm's dimensions
// Order: Shoulder, Elbow, Wrist
float armLengths[] = {6 + (5.0 / 8.0), 4.75, 5 + (7.0 / 8.0)};

// Box dimensions (width, height)
// MATLAB code expects fields box.width and box.height;
// here we store them in an array where box[0] = width and box[1] = height.
float box[] = {-5.0, -3.0};

// Struct for angles
struct Angles {
  float ShoulderVertical;
  float Elbow;
  float Hand;
};

// Safety Check Code
// It returns true if the positions are safe, false otherwise.
// It outputs Elbow_x, Elbow_y, Hand_x, and Hand_y.
bool checkSafe(Angles angles, float arm_lengths[], float box[], 
               float &Elbow_x, float &Elbow_y, float &Hand_x, float &Hand_y) {
  float theta1 = angles.ShoulderVertical;
  float theta2 = angles.Elbow;
  float theta3 = angles.Hand;

  // Arm segment lengths
  float L_shoulder = arm_lengths[0];
  float L_elbow = arm_lengths[1];
  float L_wrist = arm_lengths[2];

  // Calculate the (x, y) position of the elbow joint (MATLAB uses negative cosine)
  Elbow_x = -L_shoulder * cos(radians(theta1));
  Elbow_y = L_shoulder * sin(radians(theta1));

  // Calculate intermediate angles
  float phi2 = 90 - theta1;
  float phi3 = theta2 - phi2;

  // Calculate the (x, y) position of the wrist joint
  float Wrist_x = Elbow_x - L_elbow * sin(radians(phi3));
  float Wrist_y = Elbow_y - L_elbow * cos(radians(phi3));

  float phi4 = 90 - phi3;
  float phi5 = theta3 - phi4;

  float phi_w = 180 - phi5;

  // Calculate the (x, y) position of the hand (end effector)
  Hand_x = Wrist_x - L_wrist * sin(radians(phi_w));
  Hand_y = Wrist_y - L_wrist * cos(radians(phi_w));

  // Get box dimensions (MATLAB code uses box.width and box.height)
  float box_width = box[0];
  float box_height = box[1];

  // Conditions as defined in the MATLAB code
  bool condition_1 = (Hand_x > box_width && Hand_x < -box_width);
  bool condition_2 = (Hand_y > box_height && Hand_y < 0);
  bool condition_3 = (Hand_y < box_height);

  // Return safe = NOT [ (condition_1 && condition_2) OR condition_3 ]
  return !((condition_1 && condition_2) || condition_3);
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

//HELPER FUNCTIONS START HERE
// Basic Move Functions w/ Safety Checks
void moveShoulder(int inputAngle) {
  Angles angles = {inputAngle, 0, 0};  // Assume other angles are 0 for safety check
  float Hand_x, Hand_y, Elbow_x, Elbow_y;

  if (checkSafe(angles, armLengths, box, Elbow_x, Elbow_y, Hand_x, Hand_y)) {
    Serial.println("Error: Shoulder angle would cause the arm to hit something!");
    Serial.flush();
    while (true);  // Halt further execution
  }
  
  int newAngle = map(inputAngle, 0, 270, 0, 180);
  shoulder.write(newAngle);
}

void moveBase(int inputAngle) {
  Angles angles = {0, 0, 0};  // Assume other angles are 0 for safety check
  float Hand_x, Hand_y, Elbow_x, Elbow_y;

  if (checkSafe(angles, armLengths, box, Elbow_x, Elbow_y, Hand_x, Hand_y)) {
    Serial.println("Error: Base angle would cause the arm to hit something!");
    Serial.flush();
    while (true);  // Halt further execution
  }

  int newAngle = map(inputAngle, 0, 270, 0, 180);
  base.write(newAngle);
}

void moveElbow(int inputAngle) {
  Angles angles = {0, inputAngle, 0};  // Assume other angles are 0 for safety check
  float Hand_x, Hand_y, Elbow_x, Elbow_y;

  if (checkSafe(angles, armLengths, box, Elbow_x, Elbow_y, Hand_x, Hand_y)) {
    Serial.println("Error: Elbow angle would cause the arm to hit something!");
    Serial.flush();
    while (true);  // Halt further execution
  }

  elbow.write(inputAngle);
}

void moveWrist(int inputAngle) {
  Angles angles = {0, 0, inputAngle};  // Assume other angles are 0 for safety check
  float Hand_x, Hand_y, Elbow_x, Elbow_y;

  if (checkSafe(angles, armLengths, box, Elbow_x, Elbow_y, Hand_x, Hand_y)) {
    Serial.println("Error: Wrist angle would cause the arm to hit something!");
    Serial.flush();
    while (true);  // Halt further execution
  }

  wrist.write(inputAngle);
}

void moveGripper(int inputAngle) {
  if (inputAngle < 0 || inputAngle > 180) {
    Serial.println("Error: Invalid gripper angle!");
    Serial.flush();
    while (true);  // Halt further execution
  }
  gripper.write(inputAngle);
}

//Move Slowly Functions
void moveShoulderSlowly(int inputAngle){
  Angles angles = {inputAngle, 0, 0};  // Assume other angles are 0 for safety check
  float Hand_x, Hand_y, Elbow_x, Elbow_y;

  if (checkSafe(angles, armLengths, box, Elbow_x, Elbow_y, Hand_x, Hand_y)) {
    Serial.println("Error: Shoulder angle would cause the arm to hit something!");
    Serial.flush();
    while (true);  // Halt further execution
  }

  int totalTime = 1000; // Total movement time in milliseconds
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

void moveBaseSlowly(int inputAngle){
  Angles angles = {0, 0, 0};  // Assume other angles are 0 for safety check
  float Hand_x, Hand_y, Elbow_x, Elbow_y;

  if (checkSafe(angles, armLengths, box, Elbow_x, Elbow_y, Hand_x, Hand_y)) {
    Serial.println("Error: Base angle would cause the arm to hit something!");
    Serial.flush();
    while (true);  // Halt further execution
  }
  
  int totalTime = 1000; // Total movement time in milliseconds
  int currentAngle = map(base.read(), 0, 180, 0, 270);
  int delta = inputAngle - currentAngle;
  int sign; 
  if (delta > 0) {
    sign = 1;
  } else if (delta < 0) {
    sign = -1;
  }
  delta = abs(delta);
  float tDelay = totalTime / delta;
  int newAngle = currentAngle + sign;
  
  for (int i = 0; i < delta; i++) {
    moveBase(newAngle);
    delay(tDelay);
    newAngle += sign;
  }
}

void moveElbowSlowly(int inputAngle){
  Angles angles = {0, inputAngle, 0};  // Assume other angles are 0 for safety check
  float Hand_x, Hand_y, Elbow_x, Elbow_y;

  if (checkSafe(angles, armLengths, box, Elbow_x, Elbow_y, Hand_x, Hand_y)) {
    Serial.println("Error: Elbow angle would cause the arm to hit something!");
    Serial.flush();
    while (true);  // Halt further execution
  }
  
  int totalTime = 1000; // Total movement time in milliseconds
  int currentAngle = elbow.read();
  int delta = inputAngle - currentAngle;
  int sign; 
  if (delta > 0) {
    sign = 1;
  } else if (delta < 0) {
    sign = -1;
  }
  delta = abs(delta);
  float tDelay = totalTime / delta;
  int newAngle = currentAngle + sign;
  
  for (int i = 0; i < delta; i++) {
    moveElbow(newAngle);
    delay(tDelay);
    newAngle += sign;
  }
}

void moveWristSlowly(int inputAngle){
  Angles angles = {0, 0, inputAngle};  // Assume other angles are 0 for safety check
  float Hand_x, Hand_y, Elbow_x, Elbow_y;

  if (checkSafe(angles, armLengths, box, Elbow_x, Elbow_y, Hand_x, Hand_y)) {
    Serial.println("Error: Wrist angle would cause the arm to hit something!");
    Serial.flush();
    while (true);  // Halt further execution
  }
  
  int totalTime = 1000; // Total movement time in milliseconds
  int currentAngle = wrist.read();
  int delta = inputAngle - currentAngle;
  int sign; 
  if (delta > 0) {
    sign = 1;
  } else if (delta < 0) {
    sign = -1;
  }
  delta = abs(delta);
  float tDelay = totalTime / delta;
  int newAngle = currentAngle + sign;
  
  for (int i = 0; i < delta; i++) {
    moveWrist(newAngle);
    delay(tDelay);
    newAngle += sign;
  }
}

void moveGripperSlowly(int inputAngle){
  int totalTime = 1000; // Total movement time in milliseconds
  int currentAngle = gripper.read();
  int delta = inputAngle - currentAngle;
  int sign; 
  if (delta > 0) {
    sign = 1;
  } else if (delta < 0) {
    sign = -1;
  }
  delta = abs(delta);
  float tDelay = totalTime / delta;
  int newAngle = currentAngle + sign;
  
  for (int i = 0; i < delta; i++) {
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


  moveShoulderSlowly(135);
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

  moveShoulderSlowly(135);
  moveElbowSlowly(75);
  moveWristSlowly(120);

  delay(500);

  moveGripperSlowly(0);

  delay(2000);


}

void loop() {
  wave();
  delay(1000);
  pickUpObject();
  delay(1000);
}
