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
  #if defined(TEST_RIGHT_ANGLE_TURN)
    state.currentState = States::INITIAL_TURN;
    state.turnDirection = 1;
    state.cycles = 1;
  #endif

  #if defined(TEST_U_TURN)
    state.currentState = States::INITIAL_TURN;
    state.turnDirection = 1;
    state.cycles = 2;
  #endif

  #if !(defined(TEST_180_TURN) || defined(TEST_RIGHT_ANGLE_TURN))
    state.currentState = States::RESET;
  #endif
}

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
  #ifdef PRINT_STATE
    printState();
    // Serial.println(state.cycles);
  #endif

  #if !(defined(TEST_LINE_TRACK) || defined(TEST_LIGHT_SENSOR) || defined(TEST_COLOUR_SENSORS) || defined(TEST_MOTORS))
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
        // state.counter = 2;
      } else if (lightSensor.isAllWhite()) {
        state.currentState = States::INITIAL_FORWARD;
      }
    }
    break;

  case States::READ_COLOUR_SENSORS:
    {
      #ifdef PRINT_STATE 
        Serial.println("READING COLOUR SENSORS"); 
      #endif
      // driver.differentialSteer(0, 0);
      Turn turnType = colourSensor.getTurn();
      #ifdef PRINT_TURN
        printTurn(turnType);
      #endif
      switch (turnType) {
        case Turn::RIGHT:
          state.turnDirection = 1;
          state.cycles = 1;
          break;
        case Turn::LEFT:
          state.turnDirection = -1;
          state.cycles = 1;
          break;
        case Turn::U_TURN:
          state.turnDirection = 1;
          state.cycles = 2;
          break;
        case Turn::NONE:
          state.turnDirection = 0;
          state.cycles = 0;
          break;
      }

      if (state.turnDirection && state.cycles) {
        // if(state.cycles == 2 || state.counter == 0)state.currentState = States::INITIAL_TURN;
        // else state.counter--;
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
    state.waitDuration = TURN_DURATION;
    driver.differentialSteer(ROTATION_SPEED, state.turnDirection*TURN_CONSTANT);
    state.currentState = States::WAIT;
    break;

  case States::WAIT:
    #ifdef PRINT_STATE 
      Serial.println("WAITING"); 
    #endif
    if (millis() - state.initialTime >= state.waitDuration) state.currentState = States::READ_BLACK_LINE;
    break;

  case States::READ_BLACK_LINE:
    #ifdef PRINT_STATE 
      Serial.println("READING BLACK LINE"); 
    #endif
    {
      for (int i = 0; i < 7; i++) {
        if (lightSensor.currentReading()[i] > BLACK_THRESHOLD) {
          state.cycles--;
          if (state.cycles == 0) {
            state.turnDirection = 0;
            state.currentState = States::RESET;
          } else {
            state.currentState = States::INITIAL_TURN;
          }
          break;
        }
      }
    }
    break;

  case States::INITIAL_FORWARD:
  {
    state.cycles = 1;
    state.initialTime = millis();
    state.waitDuration = FORWARD_DURATION;
    driver.differentialSteer(motorSpeed, 0);
    state.currentState = States::WAIT;
  }
  break;

  default:
    Serial.println("no match");
    break;
  }
  #endif

  //  ~~Testing stuff~~
  #ifdef TEST_COLOUR_SENSORS
    // float r1, g1, b1, r2, g2, b2;
    
    // tcaselect(0);
    // tcs.getRGB(&r1, &g1, &b1);

    // tcaselect(1);
    // tcs.getRGB(&r2, &g2, &b2);

    // bool leftColourSensor = colourSensor.isGreen(r2, g2, b2);
    // bool rightColourSensor = colourSensor.isGreen(r1, g1, b1);
    // Serial.print("Right: "); Serial.print(rightColourSensor);
    // Serial.print("| Left: "); Serial.print(leftColourSensor);
    colourSensor.getTurn();
    // Serial.println();
  #endif

  #ifdef TEST_LIGHT_SENSOR
    double* currentReading = lightSensor.currentReading();
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

  #ifdef TEST_MOTORS
    driver.differentialSteer(1, 0);
  #endif
}