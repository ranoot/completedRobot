#include "RobotLibrary.h"

void LineTrack::reset() {
  maxError = 0;
  lastError = 0;
};

void LineTrack::operator()() {
  uint16_t sensors[7];

  lightSensor.qtrRef().read(sensors);

  double error = 0;
  uint16_t minValue{sensors[0]};
  
  for (int i = 0, sensorID = -3; i < 7; i++, sensorID++) {
    double sensorReading = (double)sensors[i];
    // Serial.print(sensorID);
    // Serial.print(": ");
    // Serial.print(sensorReading);
    // Serial.print(" ");
    error += sensorID * sensorReading;
  }
  
  if (error > maxError) maxError = error; // Calculate maximum rotation on the go
  
  for (int j = 1; j <= 6; j++) {
    // Serial.println(minValue);
    minValue = (sensors[j] > minValue) ? sensors[j] : minValue;
  }
  if (sensors[0] == minValue) {
    error = -maxError;
    //Serial.println("minleft");
  } else if (sensors[6] == minValue) {
    error = maxError;
    //Serial.println("minright");
  }
  double rotation{(error * KP) + ((error - lastError) * KD)};
  
  if (rotation > 1) rotation = 1; // limit rotation to between 1 and -1
  if (rotation < -1) rotation = -1; 
  
  driver.differentialSteer(motorSpeed, rotation);
 
  lastError = error;

//  Serial.print(error);
//  Serial.print("->");
//  Serial.println(rotation);
};
