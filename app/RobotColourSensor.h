#include <Adafruit_TCS34725.h>

struct HSB {
	double hue;
	double saturation;
	double brightness;
};

struct ColoursProperty {
  HSB minHSB;
  HSB maxHSB;
};

enum class Turn { NONE, RIGHT, LEFT, U_TURN };
inline const char* turnsStr[] = {"NONE", "RIGHT", "LEFT", "U_TURN"}; // For printing out state enum

enum class BallType { NoB, OrangeB, WhiteB };

inline ColoursProperty Green = { {70, 0.10, 50.65}, {150, 0.42, 118.1} };
inline ColoursProperty White = { {50, 0.05, 70}, {90, 0.15, 100} };
inline ColoursProperty Orange = { {10, 0.50, 120}, {30, 0.75, 140} };
inline ColoursProperty Blue = { {200, 0.30, 90}, {230, 0.6, 120} };
inline ColoursProperty Red = { {-20, 0.50, 130}, {0, 0.70, 170} };
inline ColoursProperty Black = { {40, 0 , 70}, {60, 0.1, 90} };
inline ColoursProperty Green2 = { {120, 0.3, 100}, {140, 0.6, 130} };

class RobotColourSensor {
  public:
    void init();
    bool isColour(const ColoursProperty& c, uint8_t chosenSensor);
    void print(const int option, const uint8_t chosenSensor);
    BallType checkBall();
    HSB RGBtoHSB(double red, double green, double blue);
    Turn getTurn();
    Adafruit_TCS34725& getTcs();
  private:
    Adafruit_TCS34725 tcs;
};