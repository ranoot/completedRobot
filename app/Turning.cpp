#include "RobotLibrary.h"

void turnTo(double angle)
{
 //if (angle < 0) rotation = -rotation;
 //if (angle == 180) rotation = 1;4
 //int finalAngle = (int)(angle + gyro.read())%360;
 //Serial.println(finalAngle);
 while(!gyro.dataReady());
 double reading;
 double difference;
 do {
   if (gyro.dataReady()) {
    reading = gyro.read();
    // Serial.println(reading);
    difference = (int)(angle-reading)%360;
    Serial.println(difference);
    if (difference>180){
      driver.differentialSteer(IMU_ROTATION_SPEED, -0.5);
    }
    else{
      driver.differentialSteer(IMU_ROTATION_SPEED, 0.5);
    }
   }
  //    Serial.println(gyro.read());
 } while (abs(difference) >= 5);

 driver.halt();
}

void turn(double angle)
{
  while(!gyro.dataReady());
  int finalAngle = ((int)(angle + gyro.read()))%360;
  Serial.println(finalAngle);
  turnTo((double)finalAngle);
}
