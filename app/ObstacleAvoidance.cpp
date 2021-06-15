#include "RobotLibrary.h"

void PreEvacFSM::ORIENTATE()
{
  Serial.println("O");
  int reading = tof.getDistance(ToF_SIDE);
  // Serial.println(reading);
  if (reading != -1) {
    if (
      (reading < INITIAL_OBJECT_DISTANCE)// && 
      // lightSensor.isAllWhite()
    ) {
      cycles = 2;
      driver.halt();
      currentState = &PreEvacFSM::FORWARD;
    }
  }
}

// void PreEvacFSM::FORWARD()
// {
//   driver.differentialSteer(OBSTACLE_FORWARD_SPEED, 0);
//   if (!gyro.dataReady()) return;
//   int reading = tof.getDistance(ToF_SIDE);
//   if (reading == 1) return;
  
//   if (tof.getDistance(ToF_SIDE) > OBSTACLE_DISTANCE) {
//     currentState = &PreEvacFSM::CLOCKWISE_TURN;
//   } else if (abs(gyro.read()-finalAngle) <= 5) {
//     driver.halt();
//     turnDirection = -1;
//     cycles = 1;
//     // driver.differentialSteer(OBSTACLE_FORWARD_SPEED, 0);
//     wait(OBSTACLE_FORWARD_DURATION, &PreEvacFSM::INITIAL_TURN);
//   }
// }

void PreEvacFSM::CLOCKWISE_TURN()
{
  driver.differentialSteer(OBSTACLE_ROTATION_SPEED, 0.3268);
  // if (!gyro.dataReady()) return;
  // int reading = tof.getDistance(ToF_SIDE);
  // if (reading == 1) return;
  
  // if (tof.getDistance(ToF_SIDE) < OBSTACLE_DISTANCE) {
  //   currentState = &PreEvacFSM::FORWARD;
  // } else 
  if (lightSensor.currentReading()[3] > BLACK_THRESHOLD) {
    driver.halt();
    turnDirection = -1;
    cycles = 1;
    // wait(OBSTACLE_FORWARD_DURATION, &PreEvacFSM::INITIAL_TURN);
    currentState = &PreEvacFSM::INITIAL_TURN;
  }
} 

void PreEvacFSM::FORWARD()
{
  // Serial.println("F");
  // int reading = tof.getDistance(ToF_SIDE);
  
  driver.differentialSteer(OBSTACLE_FORWARD_SPEED, 0);
  // if (reading != -1) {
  // if (reading > 400) {
  //   driver.halt();
  //   cycles--;
  //   while (!gyro.dataReady());
  //   previousAngle = gyro.read();
  //   angleTurned = 0;
  //   currentState = &PreEvacFSM::CLOCKWISE_TURN;
  // }
  // Serial.println(reading);
  if (lightSensor.isAllWhite()) currentState = &PreEvacFSM::CLOCKWISE_TURN;
}

// void PreEvacFSM::CLOCKWISE_TURN()
// {
//   // Serial.println("C");
//   if (gyro.dataReady()) {
//     int reading = gyro.read();
//     int difference = (int)(reading+360-previousAngle)%360;
//     angleTurned += (difference < 10) ? difference : 0;
//     previousAngle = reading;

//     // Serial.print(difference); Serial.print(" "); Serial.println(angleTurned);

//     driver.differentialSteer(IMU_ROTATION_SPEED /**((double)difference/90)*/, 0.5);
//     if (angleTurned >= OBSTACLE_AVOIDANCE_TURN_ANGLE) {
//       driver.halt();
//       if (cycles > 0) {
//         driver.differentialSteer(OBSTACLE_FORWARD_SPEED, 0);
//         wait(1000, &PreEvacFSM::FORWARD);
//       } else {
//         turnDirection = -1;
//         cycles = 1;
//         driver.differentialSteer(OBSTACLE_FORWARD_SPEED, 0);
//         wait(OBSTACLE_FORWARD_DURATION, &PreEvacFSM::INITIAL_TURN);
//       }
//     }
//   } 
// }