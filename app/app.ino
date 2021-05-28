#include "RobotLibrary.h"

RobotDriver driver;
RobotColourSensor colourSensor;
RobotLightSensor lightSensor;
Gyroscope gyro;
LineTrack PID;
Servos servo;
ToF tof;

void setup()
{
  Serial.begin(9600);
  IMU_SERIAL.begin(9600);
  driver.init();
  colourSensor.init();
	lightSensor.init();
  servo.init();
  tof.initial();
  state.currentmode = Modes::LINE;
  
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
  
  #if defined(TEST)
  //random code
  #endif

  Serial.println("end setting");
}

void loop() {
  // //gyro.gyroFSM();
  lightSensor.updateReading();
  #if !(defined(TEST_LINE_TRACK) || defined(TEST_LIGHT_SENSOR) || defined(TEST_COLOUR_SENSORS) || defined(TEST_MOTORS)|| defined(TEST))
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
      if (servo.checkKit()){
        state.currentState = States::PICKING_UP;
      }else if (lightSensor.isAllBlack()) {
        state.currentState = States::READ_COLOUR_SENSORS;
      }else if (tof.obstacle(ToF_FRONT)){
        Serial.println(tof.getDistance(ToF_FRONT));
        while (!tof.obstacle(ToF_SIDE)){
          driver.differentialSteer(0.20, -1);
        }
        state.currentState = States::AVOIDING_OB;
      }else if (lightSensor.isAllWhite()) {
        state.currentState = States::INITIAL_FORWARD;
      }
    }
    break;

  case States::READ_COLOUR_SENSORS:
    {
      #ifdef PRINT_STATE 
        Serial.println("READING COLOUR SENSORS"); 
      #endif
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
        state.currentState = States::INITIAL_TURN;
      } else /*if (!lightSensor.isAllBlack())*/ {
        driver.differentialSteer(0.2, 0);
        delay(100);
        state.currentState = States::LINE_TRACK;
      }
    }
    break;

  case States::INITIAL_TURN:
  {
    #ifdef PRINT_STATE 
      Serial.println("INITIAL TURN"); 
    #endif
    driver.differentialSteer(ROTATION_SPEED, state.turnDirection*state.cycles*TURN_CONSTANT);
    delay(TURN_DURATION);
    while(!lightSensor.onTrack())  lightSensor.updateReading();
    state.cycles --;
    if (state.cycles == 0) {
      state.turnDirection = 0;
      state.currentState = States::LINE_TRACK;
    }
  }
  break;

  case States::INITIAL_FORWARD:
  {
      #ifdef PRINT_STATE 
        Serial.println("INITIL FORWARD"); 
      #endif
    driver.differentialSteer(motorSpeed, 0);
    delay(FORWARD_DURATION);
    while(!lightSensor.onTrack()) lightSensor.updateReading();
    state.currentState = States::LINE_TRACK;
  }
  break;

  case States::PICKING_UP:
  {
      #ifdef PRINT_STATE 
        Serial.println("PICKING UP"); 
      #endif
    driver.differentialSteer(-0.2, 0);
    delay(grabDelay);
    servo.grab();
    state.currentState = States::LINE_TRACK;
  }
  break;

  case States::AVOIDING_OB:
  {
      #ifdef PRINT_STATE 
        Serial.println("AVOIDING OBSTACLES"); 
      #endif
    if (lightSensor.onTrack()){
      driver.differentialSteer(0.2, -1);
      delay(TURN_DURATION*0.5);
      while(!lightSensor.onTrack())lightSensor.updateReading();
      state.currentState = States::LINE_TRACK;
      break;
    }
    // driver.differentialSteer(0.2, tof.distancePID());
    driver.differentialSteer(0.2, 0.36);
  }
  break;

  default:
    Serial.println("no match");
    break;
  }
  #endif

  //  ~~Testing stuff~~
  #ifdef TEST_COLOUR_SENSORS
    Serial.println(colourSensor.isColour(Green, colourSensor_Left));
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
    driver.differentialSteer(1, 0.5);
    delay(3000);
    driver.differentialSteer(1, -0.5);
    delay(3000);
    driver.differentialSteer(1, -0);
    delay(1000);
  #endif

  #ifdef TEST
  Serial.println(tof.getDistance(ToF_FRONT));
  #endif

}
