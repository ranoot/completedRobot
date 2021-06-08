#include "RobotLibrary.h"

void PreEvacFSM::ORIENTATE()
{
  driver.differentialSteer(ROTATION_SPEED, -1);
  if (tof.obstacle(ToF_SIDE, OBSTACLE_DISTANCE)) {
    currentState = &PreEvacFSM::FORWARD;
  }
}

void PreEvacFSM::FORWARD()
{
  driver.differentialSteer(motorSpeed, 0);
  if (!lightSensor.isAllWhite()) {
    currentState = &PreEvacFSM::LINE_TRACK;
  } else if (tof.getDistance(ToF_SIDE) > OBSTACLE_DISTANCE) {
    currentState = &PreEvacFSM::CLOCKWISE_TURN;
  }
}

void PreEvacFSM::CLOCKWISE_TURN()
{
  driver.differentialSteer(ROTATION_SPEED, 1);
  if (!lightSensor.isAllWhite()) {
    currentState = &PreEvacFSM::LINE_TRACK;
  } else if (tof.getDistance(ToF_SIDE) < OBSTACLE_DISTANCE) {
    currentState = &PreEvacFSM::CLOCKWISE_TURN;
  }
}