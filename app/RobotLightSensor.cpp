#include "RobotLibrary.h"

void RobotLightSensor::init() 
{
	read(maximumReadings);
  read(minimumReadings);
  for (int i = 0; i < 5; i++) {
    driver.differentialSteer((i%2 ? 1 : -1) * CALIBRATION_MOTOR_SPEED, 0);
    for(int i = 0; i < 700; i++) {
      calibrate();
      // Serial.println(i);
    }
  }

  driver.differentialSteer(0, 0);
  delay(5000);
  for (int i = 0; i < 7; i++) {
    Serial.print(maximumReadings[i]);
    Serial.print(" ");
  }
  Serial.println();
  for (int i = 0; i < 7; i++) {
    Serial.print(minimumReadings[i]);
    Serial.print(" ");
  }
  Serial.println();
}

void RobotLightSensor::read(double* readingArray)
{
  for (int i = 0; i < 7; i++) {
    readingArray[i] = analogRead(lightSensorPins[i]);
  }
}

void RobotLightSensor::calibrate()
{
  double reading[7];
  read(reading);
  for (int i = 0; i < 7; i++) {
    if (reading[i] > maximumReadings[i]) maximumReadings[i] = reading[i];
    if (reading[i] < minimumReadings[i]) minimumReadings[i] = reading[i];
  }
}

void RobotLightSensor::updateReading()
{
  double readings[7];
  read(readings);
  for (int i = 0; i < 7; i++) {
    double reading = (readings[i] - minimumReadings[i])/(maximumReadings[i] - minimumReadings[i]);
    if (reading > 1) reading = 1;
    if (reading < 0) reading = 0;
    currentReading_[i] = reading;
  }
}

double* RobotLightSensor::currentReading()
{
  return currentReading_;
}

bool RobotLightSensor::isAllBlack() 
{
  for (int i = 0; i < 7; i++)
  {
    if (currentReading_[i] < BLACK_THRESHOLD) {
      return false;
    }
  }
  return true;
}

bool RobotLightSensor::isAllWhite() 
{
  for (int i = 0; i < 7; i++)
  {
    if (currentReading_[i] > BLACK_THRESHOLD) {
      return false;
    }
  }
  return true;
}

bool RobotLightSensor::onTrack()
{
  for (int i = 0; i < 7; i++)
  {
    if (currentReading_[i] > BLACK_THRESHOLD) {
      return true;
    }
  }
  return false;
}

int RobotLightSensor::halfBlack()
{
  int count = 0;
  for (int i = 0; i < 4; i++)
  {
    if (currentReading_[i] > BLACK_THRESHOLD) {
      count += 1;
    }
  }
  if (count == 4) return 1;
  count = 0;
  for (int i = 3; i < 7; i++)
  {
    if (currentReading_[i] > BLACK_THRESHOLD) {
      count += 1;
    }
  }
  if (count == 4) return 2;
  return 0;
}
