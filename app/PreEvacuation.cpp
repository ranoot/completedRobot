#include "RobotLibrary.h"

void PreEvacFSM::wait(int ms, void (PreEvacFSM::*f)(void))
{
  initialTime = millis();
  duration = ms;
  nextState = f;
  currentState = &PreEvacFSM::WAIT;
}

void PreEvacFSM::WAIT() 
{
  #ifdef PRINT_STATE
    Serial.println("8");
  #endif
  if ((millis() - initialTime) > duration) currentState = nextState;
}

void PreEvacFSM::run() 
{
  (this->*(this->currentState))();
}

PreEvacFSM::PreEvacFSM()
{
  currentState = &PreEvacFSM::RESET;
}

void PreEvacFSM::LINE_TRACK()
{
  #ifdef PRINT_STATE 
    Serial.println("1"); 
    // Serial.println(tof.getDistance(ToF_FRONT));
  #endif
  PID(lightSensor.currentReading());
  int tofReading = tof.getDistance(ToF_FRONT);
  // Serial.println(tofReading);
  if (lightSensor.isAllBlack()) {
    currentState = &PreEvacFSM::READ_COLOUR_SENSORS;
  } else if (lightSensor.isAllWhite()) {
    currentState = &PreEvacFSM::CHECK_RED;
  // } else if (ldr.checkKit()) {
  //   Serial.println("p");
  //   currentState = &PreEvacFSM::PICKUP;
  } else if (tofReading != -1){
    if (tofReading < INITIAL_OBJECT_DISTANCE) {
      Serial.println("t");
      currentState = &PreEvacFSM::ORIENTATE;
      driver.halt();
      driver.differentialSteer(OBSTACLE_ROTATION_SPEED, -1);
  }
  }
}

void PreEvacFSM::RESET()
{
  currentState = &PreEvacFSM::LINE_TRACK;
}

void PreEvacFSM::READ_COLOUR_SENSORS()
{
  driver.halt();
  #ifdef PRINT_STATE 
    Serial.println("3"); 
  #endif
  Turn turnType = colourSensor.getTurn();
  #ifdef PRINT_TURN
    printTurn(turnType);
  #endif
  switch (turnType) {
    case Turn::RIGHT:
      turnDirection = 1;
      cycles = 1;
      break;
    case Turn::LEFT:
      turnDirection = -1;
      cycles = 1;
      break;
    case Turn::U_TURN:
      turnDirection = 1;
      cycles = 2;
      break;
    case Turn::NONE:
      turnDirection = 0;
      cycles = 0;
      break;
  }

  if (turnDirection && cycles) {
    currentState = &PreEvacFSM::INITIAL_TURN;
  } else /*if (!lightSensor.isAllBlack())*/ {
    // driver.differentialSteer(0.2, 0); TODO: check if robot can cross black line
    // delay(100);
    driver.differentialSteer(0.2, 0);
    wait(100, &PreEvacFSM::LINE_TRACK);
  }
}

void PreEvacFSM::READ_BLACK_LINE()
{
  #ifdef PRINT_STATE
    Serial.println("4");
  #endif
  if (lightSensor.currentReading()[3] > BLACK_THRESHOLD) {
    cycles--;
    if (cycles == 0) {
      turnDirection = 0;
      currentState = &PreEvacFSM::LINE_TRACK;
    } else {
      currentState = &PreEvacFSM::INITIAL_TURN;
    }
  }
}

void PreEvacFSM::INITIAL_TURN()
{
  #ifdef PRINT_STATE 
    Serial.println("5"); 
  #endif
  driver.halt();
  driver.differentialSteer(ROTATION_SPEED, turnDirection*TURN_CONSTANT);
  wait(TURN_DURATION, &PreEvacFSM::READ_BLACK_LINE);
  
}

// void PreEvacFSM::INITIAL_FORWARD()
// {
//   #ifdef PRINT_STATE 
//     Serial.println("INITIAL FORWARD"); 
//   #endif
//   // while(!lightSensor.centered()){
//   //   preEvacState.countUpAl ++;
//   //   driver.differentialSteer(ROTATION_SPEED + 0.001*preEvacState.countUpAl, (preEvacState.countUpAl%2 ? 1 : -1));
//   //   delay(100);
//   // }
//   driver.differentialSteer(0.23, 0);
//   delay(FORWARD_DURATION);
//   evacState.initialTime = millis();
//   while(lightSensor.isAllWhite()) {
//     lightSensor.updateReading();
//   // evacState.currentTime = millis();
//   // if(evacState.currentTime - evacState.initialTime > 5000){
//   //   driver.differentialSteer(0,0);
//   //   delay(5000);
//   //   preEvacState.currentmode = Modes::ZONE;
//   //   Serial.println("change preEvacState");
//   //   break;
//   }
//   currentState = &PreEvacFSM::LINE_TRACK;
// }

// void PreEvacFSM::AVOIDING_OB()
// {
//   #ifdef PRINT_STATE 
//     Serial.println("7"); 
//   #endif
//   if (!lightSensor.isAllWhite() && countDownOb < 0){
//     driver.differentialSteer(0.2, -1);
//     delay(TURN_DURATION*0.5);
//     while(lightSensor.isAllWhite())lightSensor.updateReading();
//     currentState = &PreEvacFSM::LINE_TRACK;
//     return;
//   }
//   tof.avoidOb();
//   countDownOb --;
//   // Serial.println(countDownOb);
// }

void PreEvacFSM::PICKUP()
{
    #ifdef PRINT_STATE 
      Serial.println("PICKING UP"); 
    #endif
    driver.differentialSteer(-0.2, 0);
    delay(grabDelay);
    driver.halt();
    servo.grab();
    currentState = &PreEvacFSM::LINE_TRACK;
}

void PreEvacFSM::CHECK_RED()
{
  driver.halt();
  for (int i = 0; i < 4; i++) {
    if (
      colourSensor.isColour(Red, colourSensor_Right) ||
      colourSensor.isColour(Red, colourSensor_Left)
    ) {
      while(1);
    } else {
      break;
    }
  }
  driver.differentialSteer(motorSpeed, 0);
  wait(200, &PreEvacFSM::LINE_TRACK);
}

// void PreEvacFSM::printState()
// {
//   if (currentState == &PreEvacFSM::LINE_TRACK) {
//     Serial.println("Line Tracking");
//   } else if (currentState == &PreEvacFSM::INITIAL_TURN) {
//     Serial.println("Initial Turn");
//   } else if (currentState == &PreEvacFSM::READ_BLACK_LINE) {
//     Serial.println("Read Black line");
//   } else if (currentState == &PreEvacFSM::RESET) {
//     Serial.println("Reset");
//   } else if (currentState == &PreEvacFSM::PICKUP) {
//     Serial.println("Pickup");
//   } else if (currentState == &PreEvacFSM::AVOIDING_OB) {
//     Serial.println("Avoiding Object");
//   } else if (currentState == &PreEvacFSM::READ_COLOUR_SENSORS) {
//     Serial.println("Reading Colour Sensors");
//   }
// }