#include "RobotLibrary.h"
// #include <algorithm>

RobotDriver driver;
RobotColourSensor colourSensor;
RobotLightSensor lightSensor;
LineTrack PID;
Gyroscope gyro;

void setup()
{
	Serial.begin(9600);
  gyro.init();
	driver.init();
	colourSensor.init();
	lightSensor.init();
}

void rotationCheck()
{
  Turn turnType = colourSensor.getTurn();
  switch (turnType) {
    case Turn::RIGHT:
      Serial.println("Turning right");
      turnLine(1, 1);
      break;
    case Turn::LEFT:
      Serial.println("Turning left");
      turnLine(-1, 1);
      break;
    case Turn::U_TURN:
      Serial.println("Turning 180");
      turnLine(1, 2);
      break;
    case Turn::NONE:
       break;
  }
}

void checkRescueKit()
{
  for (int LDR = analogRead(LDRpin); LDR > 300; LDR = analogRead(A15)) 
    driver.differentialSteer(motorSpeed, 0);
  driver.differentialSteer(0, 0);
  delay(5000);
}

void loop() {
  gyro.gyroFSM();
  lightSensor.updateReading();
  switch (state.currentState)
  {
  case States::RESET:
    state = States::LINE_TRACK;
    PID(lightSensor.currentReading());
    break;

  case States::LINE_TRACK:
    PID();
    if (lightSensor.isAllBlack()) {
      state = States::READ_COLOUR_SENSORS;
    }
    break;

  case States::READ_COLOUR_SENSORS:
    Turn turnType = colourSensor.getTurn();
    switch (turnType) {
      case Turn::RIGHT:
        state.turnDirection = 1;
        state.turnNumber = 1;
        break;
      case Turn::LEFT:
        state.turnDirection = -1;
        state.turnNumber = 1;
        break;
      case Turn::U_TURN:
        state.turnDirection = 1;
        state.turnNumber = 2;
        break;
      case Turn::NONE:
        break;
    }

    if (state.turnDirection && state.turnNumber) {
      state = States::INITIAL_TURN;
    } else {
      state = States::LINE_TRACK;
    }
    break;

  case States::INITIAL_TURN:
    state.initialTime = millis();
    driver.differentialSteer(motorSpeed, state.turnDirection*state.turnNumber*0.8);
    state.currentState = States::WAIT;
    break;

  case States::WAIT:
    if (millis() - state.initialTime >= TURN_DURATION) state = States::READ_BLACK_LINE;
    break;

  case States::READ_BLACK_LINE:
    if (lightSensor.currentReading()[3] < BLACK_THRESHOLD) {
      state.turnNumber--;
      if (state.turnNumber == 0) {
        state.turnDirection = 0;
        state.currentState = States::RESET;
      } else {
        state.currentState = States::INITIAL_TURN;
      }
    }
    break;
  }
}
