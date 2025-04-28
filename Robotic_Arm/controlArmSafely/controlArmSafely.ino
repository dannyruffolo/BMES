#include "RoboticArm.h"

void setup() {
  setupRobotArm();
}

void loop() {
  moveBaseSlowly(0);
  moveShoulderSlowly(0);
  moveElbowSlowly(0);
  moveWristSlowly(0);
  moveGripper(0);
  delay(1000);

  moveBaseSlowly(90);
  moveShoulderSlowly(45);
  moveElbowSlowly(120);
  moveWristSlowly(60);
  moveGripper(180);
  delay(1000);

  moveShoulderSlowly(45);
  moveElbowSlowly(180);
  moveWristSlowly(0);
  delay(1000);
}