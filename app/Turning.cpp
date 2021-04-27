#include "RobotLibrary.h"

void turnTo(double angle)
{
  //if (angle < 0) rotation = -rotation;
  //if (angle == 180) rotation = 1;4
  //int finalAngle = (int)(angle + gyro.read())%360;
  //Serial.println(finalAngle);
  double reading;
  double difference;
  int finalAngle = ((int)(angle + gyro.read()))%360;
  do {
    reading = gyro.read();
    difference = (int)(finalAngle+360-reading)%360;
    if (difference>180){
      driver.differentialSteer(motorSpeed-0.02, -1);
    }
    else{
      driver.differentialSteer(motorSpeed-0.02, 1);
    }
    Serial.println(gyro.read());
  } while (difference != 0);

  driver.differentialSteer(0, 0);
}

void turnLine(int direction, int n)
{
  uint16_t checkB[7];
  int dn = 0;
  bool b = 0;
  
  while(dn < n){
    //Serial.println("start turning....");
    driver.differentialSteer(0.20, n*direction*0.8);
    delay(1500);
    while(!b){
      //Serial.println("continue turning");
      driver.differentialSteer(0.25, n*direction*0.8);
      lightSensor.qtrRef().read(checkB);
      for (int i = 2; i < 4; i++) {
        if (checkB[i] > BLACK_THRESHOLD){
          b = 1;
          continue;
        }
      }
    }
    dn++;
    Serial.print("dn:");Serial.println(dn);
  }
  
driver.differentialSteer(0, 0);
}
