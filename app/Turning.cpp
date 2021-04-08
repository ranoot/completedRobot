#include "RobotLibrary.h"

void turn(double angle)
{
  double initialAngle = gyro.read();
  Serial.print("Initial Angle: "); Serial.println(initialAngle);
  double difference = 0;
  while (difference < fabs(angle)) {
    driver.differentialSteer(motorSpeed, angle/180);
    double reading = gyro.read();
    difference = fabs(reading - initialAngle);
    Serial.print("Current: "); Serial.println(reading);
  }
  driver.differentialSteer(0, 0);
}
