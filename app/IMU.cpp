#include "RobotLibrary.h"

void Gyroscope::init()
{
  IMU_SERIAL.begin(IMU_BAUD_RATE);
}

// Sign bit and start bit are empty for first few loops
// No value is read after arduino is reset for a few minisecond

double Gyroscope::read() 
{
  uint8_t start, sign, data;
  double angle;
   while(IMU_SERIAL.available() <= 2);
   if (IMU_SERIAL.available() >= 2) {
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
  }
  if (sign) angle = (double)data; // sign = 1 => angle is positive
  else angle = 360 - (double)data;
  
  return angle;
}
