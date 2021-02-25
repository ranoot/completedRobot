#include <DualVNH5019MotorShield.h>
#include <Adafruit_TCS34725.h>

class RobotDriverBase {
    public:
        void differentialSteer(double speed, double rotation);
        void setup();
    private:
        DualVNH5019MotorShield md;
        void stopIfFault();
};