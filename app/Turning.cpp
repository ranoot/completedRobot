//#include "RobotLibrary.h"
////
//void turnTo(double angle)
//{
//  //if (angle < 0) rotation = -rotation;
//  //if (angle == 180) rotation = 1;4
//  //int finalAngle = (int)(angle + gyro.read())%360;
//  //Serial.println(finalAngle);
//  double reading;
//  double difference;
//  do {
//    reading = gyro.read();
//    difference = ((int)angle+360-reading2)%360;
//    if (difference>180.0){
//      driver.differentialSteer(motorSpeed-0.02, -1);
//    }
//    else{
//      driver.differentialSteer(motorSpeed-0.02, 1);
//    }
//    Serial.println(gyro.read());
//  } while (difference != 0);
//
//  driver.differentialSteer(0, 0);
//}
//
//void turn(double angle)
//{
//  int finalAngle = ((int)(angle + gyro.read()))%360;
//  turnTo((double)finalAngle);
//}
