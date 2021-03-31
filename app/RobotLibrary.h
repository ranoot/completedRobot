#ifndef ROBOTLIBRARY_H
#define ROBOTLIBRARY_H

#include <Arduino.h>
#include <DualVNH5019MotorShield.h>
#include <Adafruit_TCS34725.h>
#include <QTRSensors.h>
#include "Wire.h"

void tcaselect(uint8_t i);
void rightAngleTurn(char direction);

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

class IMU {
  public:
    double read();
};
#endif
