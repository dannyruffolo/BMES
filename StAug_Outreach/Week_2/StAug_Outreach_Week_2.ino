#include "RoboticArm.h"

void setup() {
  setupRobotArm();

}

void loop() {
  moveShoulderSlowly(90);
  delay(1000);
  moveElbowSlowly(90);
  delay(1000);
  moveWristSlowly(0);
  delay(5000);
  moveWristSlowly(90);
  delay(5000);
}
