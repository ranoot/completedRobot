#include "RobotLibrary.h"

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
//	lightSensor.init();
  delay(2000);
}

<<<<<<< HEAD
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
=======
void rotationCheck()
{
	Turn turnType = colourSensor.getTurn();
  switch (turnType) {
    case Turn::RIGHT:
       Serial.println("Turning lmao");
       turn(75);
       break;
     case Turn::LEFT:
       Serial.println("Turning lmao1");
       turn(-75);
       break;
     case Turn::U_TURN:
       Serial.println("Turning lmao2");
       turn(180);
       break;
     case Turn::NONE:
       break;
  }
}
>>>>>>> parent of a5d8094... state machine

#define LDRpin A15

void checkRescueKit()
{
<<<<<<< HEAD
  for (int LDR = analogRead(LDRpin); LDR > 300; LDR = analogRead(A15)) 
    driver.differentialSteer(1, 0);
=======
  for (int LDR = analogRead(A15); LDR > 300; LDR = analogRead(A15)) 
    driver.differentialSteer(motorSpeed, 0);
>>>>>>> parent of a5d8094... state machine
  driver.differentialSteer(0, 0);
  delay(5000);
}
DualVNH5019MotorShield md;
void loop() {
<<<<<<< HEAD
//  gyro.gyroFSM();
  lightSensor.updateReading();
//  for (int i = 0; i < 7; i++) {
//    Serial.print(lightSensor.currentReading()[i]); Serial.print(" ");
//  }
  driver.differentialSteer(1, 0);
 Serial.println();
 switch (state.currentState)
 {
 case States::RESET:
   state.currentState = States::LINE_TRACK;
   break;

 case States::LINE_TRACK:
   PID(lightSensor.currentReading());
   if (lightSensor.isAllBlack()) {
     state.currentState = States::READ_COLOUR_SENSORS;
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
     state.currentState = States::INITIAL_TURN;
   } else {
     state.currentState = States::LINE_TRACK;
   }
   break;

 case States::INITIAL_TURN:
   state.initialTime = millis();
   driver.differentialSteer(motorSpeed, state.turnDirection*state.turnNumber*0.8);
   state.currentState = States::WAIT;
   break;

 case States::WAIT:
   if (millis() - state.initialTime >= TURN_DURATION) state.currentState = States::READ_BLACK_LINE;
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
=======
//  checkRescueKit();
//  if (IMU_SERIAL.available()) Serial.println(IMU_SERIAL.read());
//unsigned long timeStart = micros();
//rotationCheck();
//Serial.println(micros() - timeStart);
//turn(90);
>>>>>>> parent of a5d8094... state machine
}
