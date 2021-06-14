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

void PreEvacFSM::FORWARD()
{
  // Serial.println("F");
  int reading = tof.getDistance(ToF_SIDE);
  
  driver.differentialSteer(OBSTACLE_FORWARD_SPEED, 0);
  // if (reading != -1) {
  if (reading > 400) {
    driver.halt();
    cycles--;
    while (!gyro.dataReady());
    finalAngle = (int)(gyro.read()+OBSTACLE_AVOIDANCE_TURN_ANGLE)%360;
    currentState = &PreEvacFSM::CLOCKWISE_TURN;
  }
  // Serial.println(reading);
}

void PreEvacFSM::CLOCKWISE_TURN()
{
  // Serial.println("C");
  if (gyro.dataReady()) {
    int reading = gyro.read();
    int difference = (int)(finalAngle+360-reading)%360;
    // Serial.println(difference);
    driver.differentialSteer(IMU_ROTATION_SPEED*((double)difference/90), 0.5);
    if (abs(difference) <= 10) {
      driver.halt();
      if (cycles > 0) {
        driver.differentialSteer(OBSTACLE_FORWARD_SPEED, 0);
        wait(1400, &PreEvacFSM::FORWARD);
      } else {
        turnDirection = -1;
        cycles = 1;
        driver.differentialSteer(OBSTACLE_FORWARD_SPEED, 0);
        wait(OBSTACLE_FORWARD_DURATION, &PreEvacFSM::INITIAL_TURN);
      }
    }
  }
}