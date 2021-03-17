#include <DualVNH5019MotorShield.h>
#include <Adafruit_TCS34725.h>
#include <QTRSensors.h>
#include "Wire.h"

void tcaselect(uint8_t i);

struct HSB {
	double hue;
	double saturation;
	double brightness;
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
        void init();
        bool isGreen(uint8_t sensorAddr);
        HSB RGBtoHSB(int red, int green, int blue);
    private:
        Adafruit_TCS34725 tcs;
};

class RobotLightSensor {
    public:
        void init();
        QTRSensors& qtrRef();
    private:
        QTRSensors qtr;
};