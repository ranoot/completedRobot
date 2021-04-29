#include "RobotLibrary.h"

void RobotLightSensor::init() 
{
  qtr.setTypeAnalog(); // or setTypeAnalog()
	qtr.setSensorPins(lightArrayPins, 7);

	for (uint8_t i = 1; i <= 150; i++)
	{
		qtr.calibrate();
		delay(10);
		Serial.println(i);
	}
}

void RobotLightSensor::updateReading()
{
  qtr.read(currentReading_);
}

uint16_t* RobotLightSensor::currentReading()
{
  return currentReading_;
}

bool RobotLightSensor::isAllBlack() 
{
  bool val = true;
  for (int i = 0; i < 7; i++)
  {
    if (currentReading_[i] > 600) {
      val = false;
      break;
    }
  }
  return val;
}

QTRSensors& RobotLightSensor::qtrRef() { return qtr; } 
