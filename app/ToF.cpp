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
    sensor.setDistanceMode(VL53L1X::Medium);
    sensor.setMeasurementTimingBudget(33000);
    sensor.startContinuous(20);
  }
}

bool ToF::obstacle(int sensorToF, int threshold){
  tcaselect(sensorToF);
  int distance;

  if (sensor.timeoutOccurred()) Serial.print("TIMEOUT");
  
  for (int i = 0; i < 7; i ++){
    distance = sensor.read(false);
    // Serial.println(distance);
    if (distance > threshold) return false;
  }
  
  return true;
}

// void ToF::avoidOb()
// {
//   int distance;
//   lastDistance = 1000;
//   if(obstacle(ToF_SIDE, 300)){
//     driver.differentialSteer(0.2, 0);
//   } else {
//     while (stage == 0) {
//       distance = sensor.read();
//       if (distance < 300){
//         if (distance > lastDistance){
//           stage = 1;
//         }
//         lastDistance = distance;
//         driver.differentialSteer(0.2, 1);
//       }else{
//         driver.differentialSteer(0.2, 1);
//       }
//     }
//   }
// if(){
//   if(tof.obstacle(ToF_SIDE, 300)&&stage2 == 0) driver.differentialSteer(0.2, 1);
//   if(!tof.obstacle(ToF_SIDE, 100)){
//     driver.differentialSteer(0.2, 0);
//     stage2 == 1;
//   }else stage = 0;
// }
// }

// * Returns -1 if data is not ready
int ToF::getDistance(int Sensor){
  tcaselect(Sensor);

  if (sensor.timeoutOccurred()) Serial.print("TIMEOUT");
  
  int v = -1;
  if (sensor.dataReady()) v = sensor.read(false);
  return v;
}

// void ToF::resetObStage(){
//   stage = 0;
//   stage2 = 0;
//   preEvacFSM.countDownOb = 30;
// }
