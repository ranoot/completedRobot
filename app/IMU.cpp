#include "RobotLibrary.h"

inline void Gyroscope::init()
{
  currentState_ = NOT_READING; // Ensures 2 bytes are in buffer before read
  IMU_SERIAL.begin(IMU_BAUD_RATE);
}

// Sign bit and start bit are empty for first few loops
// No value is read after arduino is reset for a few minisecond
// ** Check that 2 bits are present before using function
double Gyroscope::read() 
{
  uint8_t start, sign, data;
  double angle;
  while (IMU_SERIAL.available()) {
    start = IMU_SERIAL.read();
    if (start >=  254) {
      sign = 255 ^ start;
      break;
    }
  }
  while (IMU_SERIAL.available()) {
    data = IMU_SERIAL.read();
    if (data <= 180) break;
  }
  if (sign) angle = (double)data; // sign = 1 => angle is positive
  else angle = 360 - (double)data;
  
  return angle;
}

void Gyroscope::gyroFSM()
{
  switch (currentState_)
  {
  case READING:
    currentReading_ = read();
    if (IMU_SERIAL.available() < 2) currentState_ = NOT_READING;
    break;
  
  case NOT_READING:
    if (IMU_SERIAL.available() >= 2) currentState_ = READING;
    break;

  case START: 
  // start case included to allow for another function to check if a value has been read at least once
    if (IMU_SERIAL.available() >= 2) currentState_ = READING;
    break;
  }
}

double Gyroscope::currentReading()
{
  return currentReading_;
}

Gyroscope::gyroStates Gyroscope::currentState() 
{
  return currentState_;
}