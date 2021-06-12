#include "RobotLibrary.h"

void PreEvacFSM::ORIENTATE()
{
  // Serial.println("O");
  int reading = tof.getDistance(ToF_SIDE);
  Serial.println(reading);
  if (reading != -1) {
    if (
      (reading < INITIAL_OBJECT_DISTANCE) && 
      lightSensor.isAllWhite()
    ) {
      cycles = 2;
      driver.halt();
      currentState = &PreEvacFSM::FORWARD;
    }
  }
}

void PreEvacFSM::FORWARD()
{
  Serial.println("F");
  int reading = tof.getDistance(ToF_SIDE);
  driver.differentialSteer(OBSTACLE_ROTATION_SPEED, 0);
  // if (reading != -1) {
  if (reading > 400) {
    driver.halt();
    while (!gyro.dataReady());
    finalAngle = (int)(gyro.read()+90)%360;
    currentState = &PreEvacFSM::CLOCKWISE_TURN;
  }
  // }
}

void PreEvacFSM::CLOCKWISE_TURN()
{
  Serial.println("C");
  if (gyro.dataReady()) {
    int reading = gyro.read();
    int difference = (int)(finalAngle-reading)%360;
    //// Serial.println(difference);
    driver.differentialSteer(IMU_ROTATION_SPEED, 0.5);
    if (abs(difference) <= 5) {
      driver.halt();
      currentState = &PreEvacFSM::FORWARD;
    }
  }
}