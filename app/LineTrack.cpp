#include "RobotLibrary.h"

void LineTrack::reset() {
  maxError = 0;
  lastError = 0;
};

void LineTrack::operator()(double* sensors) {
  double error = 0;
  double maxValue{sensors[0]}; 

  int lowestIndex = 0;
  for (int j = 1; j <= 6; j++) {
    // Serial.println(maxValue);
    if (sensors[j] > maxValue) {
      maxValue = sensors[j];
      lowestIndex = j;
    }
  }
  // When only the leftmost or rightmost sensor is on the blackline 
  // -> error values may be lower than expected
  
  for (int i = 0, sensorID = -3; i < 7; i++, sensorID++) {
    // Serial.print(sensorID);
    // Serial.print(": ");
    // Serial.print(sensors[i]);
    // Serial.print(" ");
    error += sensorID * sensors[i];
  }

  Serial.println();
  
  if (error > maxError) maxError = error; // Calculate maximum rotation on the go
  
  
  if (lowestIndex == 0) {
    error = -maxError;
    // Serial.println("minleft");
  } else if (lowestIndex == 6) {
    error = maxError;
    // Serial.println("minright");
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
