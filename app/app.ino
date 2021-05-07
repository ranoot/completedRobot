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
  IMU_SERIAL.begin(9600);
  driver.init();
  colourSensor.init();
	lightSensor.init();
  state.currentState = States::RESET;
}

// void rotationCheck()
// {
//   Turn turnType = colourSensor.getTurn();
//   switch (turnType) {
//     case Turn::RIGHT:
//       Serial.println("Turning right");
//       turnLine(1, 1);
//       break;
//     case Turn::LEFT:
//       Serial.println("Turning left");
//       turnLine(-1, 1);
//       break;
//     case Turn::U_TURN:
//       Serial.println("Turning 180");
//       turnLine(1, 2);
//       break;
//     case Turn::NONE:
//        break;
//   }
// }

// void rotationCheck()
// {
// 	Turn turnType = colourSensor.getTurn();
//   switch (turnType) {
//     case Turn::RIGHT:
//        Serial.println("Turning lmao");
//        turn(75);
//        break;
//      case Turn::LEFT:
//        Serial.println("Turning lmao1");
//        turn(-75);
//        break;
//      case Turn::U_TURN:
//        Serial.println("Turning lmao2");
//        turn(180);
//        break;
//      case Turn::NONE:
//        break;
//   }
// }

// void rotationCheck()
// {
//   Turn turnType = colourSensor.getTurn();
//   switch (turnType) {
//     case Turn::RIGHT:
//       Serial.println("Turning right");
//       turnLine(1, 1);
//       break;
//     case Turn::LEFT:
//       Serial.println("Turning left");
//       turnLine(-1, 1);
//       break;
//     case Turn::U_TURN:
//       Serial.println("Turning 180");
//       turnLine(1, 2);
//       break;
//     case Turn::NONE:
//        break;
//   }
// }

void checkRescueKit()
{
  
  for (int LDR = analogRead(A15); LDR > 300; LDR = analogRead(A15)) 
    driver.differentialSteer(motorSpeed, 0);
  driver.differentialSteer(0, 0);
  delay(5000);
}

void printState() { Serial.println(stateStr[static_cast<int>(state.currentState)]); }

void loop() {
  // //gyro.gyroFSM();
  lightSensor.updateReading();
  // #ifdef PRINT_STATE
  //   printState();
  // #endif

  #if !(defined(TEST_LINE_TRACK) || defined(TEST_LIGHT_SENSOR) || defined(TEST_COLOUR_SENSORS))
  switch (state.currentState)
  {
  case States::RESET:
    state.currentState = States::LINE_TRACK;
    break;

  case States::LINE_TRACK:
    {
      #ifdef PRINT_STATE 
        Serial.println("LINE TRACKING"); 
      #endif
      PID(lightSensor.currentReading());
      if (lightSensor.isAllBlack()) {
        state.currentState = States::READ_COLOUR_SENSORS;
      }
    }
    break;

  case States::READ_COLOUR_SENSORS:
    {
      #ifdef PRINT_STATE 
        Serial.println("READING COLOUR SENSORS"); 
      #endif
      driver.differentialSteer(0, 0);
      Turn turnType = colourSensor.getTurn();
      #ifdef PRINT_TURN
        printTurn(turnType);
      #endif
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
          state.turnDirection = 0;
          state.turnNumber = 0;
          break;
      }

      if (state.turnDirection && state.turnNumber) {
        state.currentState = States::INITIAL_TURN;
      } else /*if (!lightSensor.isAllBlack())*/ {
        state.currentState = States::LINE_TRACK;
      }
    }
    break;

  case States::INITIAL_TURN:
    #ifdef PRINT_STATE 
      Serial.println("DOING INITIAL TURN"); 
    #endif
    state.initialTime = millis();
    driver.differentialSteer(ROTATION_SPEED, state.turnDirection*state.turnNumber*0.9);
    state.currentState = States::WAIT;
    break;

  case States::WAIT:
    #ifdef PRINT_STATE 
      Serial.println("WAITING"); 
    #endif
    if (millis() - state.initialTime >= TURN_DURATION) state.currentState = States::READ_BLACK_LINE;
    break;

  case States::READ_BLACK_LINE:
    #ifdef PRINT_STATE 
      Serial.println("READING BLACK LINE"); 
    #endif
    if (lightSensor.currentReading()[3] > BLACK_THRESHOLD) {
      state.turnNumber--;
      if (state.turnNumber == 0) {
        state.turnDirection = 0;
        state.currentState = States::RESET;
      } else {
        state.currentState = States::INITIAL_TURN;
      }
    }
    break;

  default:
    Serial.println("no match");
    break;
  }
  #endif
  //  ~~Testing stuff~~
  #ifdef TEST_COLOUR_SENSORS
    Serial.print("Right: "); Serial.print(colourSensor.isGreen(0));
    Serial.print("| Left: "); Serial.print(colourSensor.isGreen(1));
    Serial.println();
  #endif

  #ifdef TEST_LIGHT_SENSOR
    uint16_t* currentReading = lightSensor.currentReading();
    for (int i = 0; i < 7; i++) {
      Serial.print(currentReading[i]);
      Serial.print(" ");
    }
    Serial.println();
    Serial.println(lightSensor.isAllBlack());
  #endif

  #ifdef TEST_LINE_TRACK
    PID(lightSensor.currentReading());
  #endif
}