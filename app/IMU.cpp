#include "RobotLibrary.h"
#define IMU_SERIAL Serial2
#define IMU_BAUD_RATE 115200

void IMU::init()
{
  IMU_SERIAL.begin(IMU_BAUD_RATE);
}

// Sign bit and start bit are empty for first few loops
// No value is read after arduino is reset for a few minisecond

double IMU::read() 
{
  uint8_t start, sign, data;
  double angle;
  if (IMU_SERIAL.available()>2) {
    while (IMU_SERIAL.available()) {
      start = IMU_SERIAL.read();
      Serial.println(start);
      if (start >=  254) {
        sign = 255 ^ start;
        break;
      }
    }
    if (IMU_SERIAL.available()) {
      data = IMU_SERIAL.read();
      Serial.println(data);
      if (data <= 180) {
          if (sign) angle = data; // sign = 1 => angle is positive
          else angle = -data;
      }
    }
  }
  return angle;
}
