#ifndef ROBOTLIBRARY_H
#define ROBOTLIBRARY_H

#define KP 0.0008 //0.0008 with speed 0.25
#define KD 0.019 //0.05, 0.019
#define motorSpeed 0.25 //0.23
#define M1 100
#define M2 100
#define IMU_SERIAL Serial2
#define IMU_BAUD_RATE 9600
// A15 for LDR

#include <Arduino.h>
#include <DualVNH5019MotorShield.h>
#include <Adafruit_TCS34725.h>
#include <QTRSensors.h>
#include "Wire.h"

void tcaselect(uint8_t i);
void rightAngleTurn(char direction);
void turn(double angle);

struct HSB {
	double hue;
	double saturation;
	double brightness;
};

class RobotDriver {
    public:
        void differentialSteer(double speed, double rotation);
        void init();
        DualVNH5019MotorShield& get_md();
    private:
        DualVNH5019MotorShield md;
        void stopIfFault();
};

class RobotLightSensor {
    public:
        void init();
        QTRSensors& qtrRef();
    private:
        QTRSensors qtr;
};

class RobotColourSensor {
  public:
    void init();
    bool isGreen(uint8_t sensorAddr);
    HSB RGBtoHSB(double red, double green, double blue);
  private:
    Adafruit_TCS34725 tcs;
};

class Gyroscope {
  private:
  public:
    void init();
    double read();
};


extern RobotDriver driver;
extern RobotColourSensor colourSensor;
extern RobotLightSensor lightSensor;
extern Gyroscope gyro;

#endif
