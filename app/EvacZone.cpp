#include "RobotLibrary.h"

void changeState(EvacStates s) {
  evacState.previousState = evacState.currentState;
  evacState.currentState = s;
}

EvacStates switchDirection(EvacStates s) {
  if (s == EvacStates::LEFT_TURN) return EvacStates::RIGHT_TURN;
  if (s == EvacStates::RIGHT_TURN) return EvacStates::LEFT_TURN;
  return s;
}

void advanceZLoop() { 
  if (evacState.startState >= 2) {
    if (evacState.zLoopState >= 7) {
      evacState.zLoopState = 0;
      evacState.zLoopCount++;
    } else {
      evacState.zLoopState++; 
    }
    changeState((exit) ? switchDirection(zLoop[evacState.zLoopState]) : zLoop[evacState.zLoopState]);
  } else {
    evacState.startState++;
    changeState(start[evacState.startState]);
  }
}


int getCurrentCorner() { // 1, 2, 3 or 4 (should only be called in pick-up state/ end)
  if (evacState.zLoopCount <= 2) { 
    // current position is either 2 or 3 when little zLoops are completed
    if (evacState.startState == 2) return 3;
    else return 2; //evacState.startState = 2 when FORWARD during start
  } else {  // current position is either 1 or 4

    /*
    Assumption:
      Right -> Fixed Forward => position 4
      Left -> Forward => position 4
      Right -> Forward => position 1
      Left -> Fixed Forward => position 1
    */

    if (evacState.currentState == EvacStates::FIXED_FORWARD) {
      if (evacState.previousState == EvacStates::RIGHT_TURN) return 4;
      if (evacState.previousState == EvacStates::LEFT_TURN) return 1;
    }

    if (evacState.currentState == EvacStates::FORWARD) {
      if (evacState.previousState == EvacStates::RIGHT_TURN) return 1;
      if (evacState.previousState == EvacStates::LEFT_TURN) return 4;
    }
  }
}

// void turn(double angle){
//   if (angle < 0) {
//   driver.differentialSteer(0.2, -1);
//   delay(-angle*11.8);
//   }else{
//   driver.differentialSteer(0.2, 1);
//   delay(angle*11);
//   }
// }

/*----------
  1       2
  4       3
  ----------*/
//TODO: test for FIXED_FORWARD_DURATION, TOF detection, set initialAngle, turnby function
// update light sensor and IMU readings
void runEvacFSM()
{
  lightSensor.updateReading();
  // gyro.gyroFSM();

  switch (evacState.currentState) 
  {
  case EvacStates::RIGHT_TURN:
    {
      servo.clawUp();
      turn(90);
      advanceZLoop();
      servo.clawDown();
    }
    break;

  case EvacStates::LEFT_TURN:
    {
      servo.clawUp();
      turn(-90);
      advanceZLoop();
      servo.clawDown();
    }
    break;

  case EvacStates::FORWARD:
    {
      driver.differentialSteer(motorSpeed, 0);
      evacState.ballType = colourSensor.checkBall();
      if (tof.getDistance(ToF_FRONT) <= MAX_BOX_DISTANCE) {
        changeState(EvacStates::DEPOSIT);
      } else if (evacState.ballType != BallType::NoB) {
        changeState(EvacStates::PICKUP);
      } else if (!lightSensor.isAllWhite()) {
        if (exit) {
          driver.differentialSteer(motorSpeed, 0);
          delay(FORWARD_DETECT_COLOUR_DURATION);
          driver.differentialSteer(0, 0);
          if (
            (colourSensor.isColour(Blue, colourSensor_Left) && colourSensor.isColour(Blue, colourSensor_Left)) ||
            (colourSensor.isColour(Green, colourSensor_Left) && colourSensor.isColour(Green, colourSensor_Left))
          ) {
            while (true);
          }
        }
        advanceZLoop();
      }
    }
    break;

  case EvacStates::FIXED_FORWARD:
    {
      if (!evacState.initialTime) {
        evacState.initialTime = millis();
        evacState.forwardDuration = FIXED_FORWARD_DURATION;
        driver.differentialSteer(motorSpeed, 0);
      } else {
        evacState.ballType = colourSensor.checkBall();
        if (tof.getDistance(ToF_FRONT) <= MAX_BOX_DISTANCE) {
          advanceZLoop();
          evacState.initialTime = 0;
        } else if ((millis()-evacState.initialTime) >= evacState.forwardDuration) {
          advanceZLoop();
          evacState.initialTime = 0;
        } else if (evacState.ballType != BallType::NoB) {
          evacState.forwardDuration -= (int)(millis()-evacState.initialTime); 
          // store the amount of time left to complete the forward movement
          
          changeState(EvacStates::PICKUP);
        }
      }
    }
    break;

  case EvacStates::PICKUP:
    {
      servo.grab();
      servo.sort(evacState.ballType);

      changeState(evacState.previousState);
    }
    break;

  case EvacStates::DEPOSIT:
    {
      turn(180);
      driver.differentialSteer(-motorSpeed, 0);
      delay(REVERSE_DURATION);
      driver.differentialSteer(0, 0);
      servo.openDoor();
      evacState.exit = true;
    }
    break;

  // case EvacStates::LOG_POSITION:
  //   {
  //     evacState.corner = getCurrentCorner();
  //     advanceZLoop();
  //   }
  //   break;
  }
}

void adjustPosition(){
  while (! (colourSensor.isColour(Red, colourSensor_Left) || colourSensor.isColour(Blue, colourSensor_Left) || colourSensor.isColour(Black, colourSensor_Left) || colourSensor.isColour(Green2, colourSensor_Left)|| colourSensor.isColour(Blue, colourSensor_Left))){
    driver.differentialSteer(0.2, 0.5);
  }
  while (! (colourSensor.isColour(Red, colourSensor_Right) || colourSensor.isColour(Blue, colourSensor_Right) || colourSensor.isColour(Black, colourSensor_Right) || colourSensor.isColour(Green2, colourSensor_Right)|| colourSensor.isColour(Blue, colourSensor_Right))){
    driver.differentialSteer(0.2, -0.5);
  }
}

bool see(){
  if(colourSensor.isColour(Red, colourSensor_Left) || colourSensor.isColour(Blue, colourSensor_Left) || colourSensor.isColour(Black, colourSensor_Left) || colourSensor.isColour(Green2, colourSensor_Left)|| colourSensor.isColour(Blue, colourSensor_Left)){
    return true;
  }
  if(colourSensor.isColour(Red, colourSensor_Right) || colourSensor.isColour(Blue, colourSensor_Right) || colourSensor.isColour(Black, colourSensor_Right) || colourSensor.isColour(Green2, colourSensor_Right)|| colourSensor.isColour(Blue, colourSensor_Right)){
    return true;
  }
  return false;
}