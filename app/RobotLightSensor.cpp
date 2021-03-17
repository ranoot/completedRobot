#include "RobotLibrary.h"

void RobotLightSensor::init() 
{
    qtr.setTypeAnalog(); // or setTypeAnalog()
  	qtr.setSensorPins((const uint8_t[]){A7, A8, A9, A10, A11, A12, A13}, 7);

	for (uint8_t i = 0; i < 150; i++)
	{
		qtr.calibrate();
		delay(10);
		Serial.println(i);
	}
}

QTRSensors& RobotLightSensor::qtrRef() { return qtr; } 