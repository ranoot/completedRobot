#include "RobotLibrary.h"
#define IMU_SERIAL Serial2

double IMU::read() 
{
  uint8_t start, sign, data;
  double angle;
  if (IMU_SERIAL.available()>2) {
    while (IMU_SERIAL.available()) {
      start = IMU_SERIAL.read();
      if (start >=  254) {
        sign = 255 ^ start;
        break;
      }
    }
    if (IMU_SERIAL.available()) {
      data = IMU_SERIAL.read();
      if (data <= 180) {
          if (sign) angle = data; // sign = 1 => angle is positive
          else angle = -data;
      }
    }
  }
  return angle;
}
