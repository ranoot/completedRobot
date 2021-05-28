#include "RobotLibrary.h"

//front one is 4, side one is 6

void ToF::initial(){
  for (uint8_t i = 4; i <7; i += 2){
    tcaselect(i);
    sensor.setTimeout(500);
    if (!sensor.init())
    {
      Serial.println("Failed to detect and initialize sensor!");
      while (1);
    }
    sensor.setDistanceMode(VL53L1X::Short);
    sensor.setMeasurementTimingBudget(50000);
    sensor.startContinuous(20);
  }
}

bool ToF::obstacle(int sensorToF){
  tcaselect(sensorToF);
  int distance = sensor.read();
  // Serial.println(distance);
  if (sensor.timeoutOccurred()) Serial.print("TIMEOUT");
  if (distance < DISTANCE_THRESHOLD){
//    Serial.println("found ob");
//    delay(1000);
    return true;
  }else{
    return false;
  }
}

double ToF::distancePID(){
  tcaselect(ToF_SIDE);
  int distance = sensor.read();
  Serial.println(distance);
  Serial.print("avoiding ");
  double error = target - (double)distance;
  double rotation = error*ToFscaling;
  if(rotation < 0){
    Serial.println(0);
    return 0;
  }
  if(rotation > 0.5){
    Serial.println(0.5);
    return 0.5;
  }
  Serial.println(rotation);
  return rotation;
}

void ToF::avoidOb(){
  driver.differentialSteer(0.2, 0.3);
}

int ToF::getDistance(int Sensor){
  tcaselect(Sensor);
  int v = sensor.read();
  return v;
}