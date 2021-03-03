#include <DualVNH5019MotorShield.h>
#include <Adafruit_TCS34725.h>

class RobotDriverBase {
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
        bool isOrange();
        bool isWhite();
    private:
        Adafruit_TCS34725 tcs;
        bool withinRange(int16_t rMax, int16_t rMin, int16_t gMax, uint16_t gMin, uint16_t bMax, uint16_t bMin);
};