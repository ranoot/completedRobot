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
//	lightSensor.init();
  delay(2000);
}

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

#define LDRpin A15

void checkRescueKit()
{
  for (int LDR = analogRead(A15); LDR > 300; LDR = analogRead(A15)) 
    driver.differentialSteer(motorSpeed, 0);
  driver.differentialSteer(0, 0);
  delay(5000);
}

void loop() {
//  checkRescueKit();
//  if (IMU_SERIAL.available()) Serial.println(IMU_SERIAL.read());
//unsigned long timeStart = micros();
//rotationCheck();
//Serial.println(micros() - timeStart);
//turn(90);
}
