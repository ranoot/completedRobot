#include <DualVNH5019MotorShield.h>
#include <Adafruit_TCS34725.h>
#include <QTRSensors.h>
#include "Wire.h"

struct HSB {
	int hue;
	int saturation;
	int brightness;
};

class RobotDriver {
    public:
        void differentialSteer(double speed, double rotation);
        void init();
    private:
        DualVNH5019MotorShield md;
        void stopIfFault();
};

class RobotColourSensor {
    public:
        RobotColourSensor();
        void init();
        bool isGreen();
        HSB RGBtoHSB(int red, int green, int blue);
    private:
        Adafruit_TCS34725 tcs1;
        Adafruit_TCS34725 tcs2;
};