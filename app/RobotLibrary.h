#pragma once
#ifndef ROBOTLIBRARY_H
#define ROBOTLIBRARY_H

#define TCAADDR 0x70
#define LDR_PIN A15
#define ToF_FRONT 4
#define ToF_SIDE 6
#define colourSensor_Left 1
#define colourSensor_Right 0
#define colourSensor_Middle 2

#define KP 0.23 //0.0008 with speed 0.25
#define KD 4//12
 //0.05, 0.019
#define motorSpeed 0.22 //0.22
#define CALIBRATION_MOTOR_SPEED 0.2
#define ROTATION_SPEED 0.23
#define IMU_BAUD_RATE 9600

#define BLACK_THRESHOLD 0.5
#define TURN_DURATION 900
#define TURN_CONSTANT 0.87
#define FORWARD_DURATION 400
#define FORWARD_SUS 10
#define grabDelay 100 // to be checked
#define DISTANCE_THRESHOLD 120

// ~ evac state constants ~
#define FIXED_FORWARD_DURATION 5000 // change this!!!
#define FORWARD_DETECT_COLOUR_DURATION 200 // checking for blue or green
#define MAX_BOX_DISTANCE 100 // idk this also
#define REVERSE_DURATION 500

// ~ testing switches ~
#define PRINT_STATE
#define PRINT_TURN
// #define TEST_COLOUR_SENSORS
// #define TEST_LIGHT_SENSOR
// #define TEST_LINE_TRACK
// #define TEST_MOTORS
// #define TEST

// only choose one please
// #define TEST_RIGHT_ANGLE_TURN
// #define TEST_U_TURN

#include <Arduino.h>
#include <DualVNH5019MotorShield.h>
#include <Adafruit_TCS34725.h>
#include <QTRSensors.h>
#include "Wire.h"
#include <SoftwareSerial.h>
#include <Servo.h>
#include <VL53L1X.h>

inline const char* turnsStr[] = {"NONE", "RIGHT", "LEFT", "U_TURN"};

enum class Turn {NONE, RIGHT, LEFT, U_TURN };

enum class BallType {NoB, OrangeB, WhiteB};

enum class States {
  RESET, 
  LINE_TRACK, 
  READ_COLOUR_SENSORS, 
  INITIAL_TURN,
  INITIAL_FORWARD,
  PICKING_UP,
  AVOIDING_OB,
  SWEEPING,
  STOP,
};

enum class Modes {LINE, ZONE};

inline struct State {
  States currentState;
  Modes currentmode;
  // int currentState = 0;
  unsigned long initialTime;
  int turnDirection;
  int cycles = 0;
} state;

struct HSB {
	double hue;
	double saturation;
	double brightness;
};

struct ColoursProperty {
  HSB minHSB;
  HSB maxHSB;
};

inline ColoursProperty Green = {{70, 0.10, 50.65}, {150, 0.42, 118.1}};
inline ColoursProperty White = {{80, 0.02, 80}, {160, 0.09, 90}};
inline ColoursProperty Orange = {{10, 0.40, 100}, {20, 0.75, 130}};
inline ColoursProperty Blue;// = {{10, 0.40, 100}, {20, 0.75, 130}}; need readings

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
      bool isAllBlack();
      bool isAllWhite();
      void updateReading();
      double* currentReading();
      bool onTrack();
    private:
      void read(double* readingArray);
      void calibrate();
      double currentReading_[7];
      double minimumReadings[7];
      double maximumReadings[7];
      const uint8_t lightSensorPins[7] = {A8, A9, A10, A11, A12, A13, A14};
};

class RobotColourSensor {
  public:
    void init();
    bool isColour(ColoursProperty c, uint8_t chosenSensor);
    BallType checkBall();
    HSB RGBtoHSB(double red, double green, double blue);
    Turn getTurn();
    Adafruit_TCS34725& getTcs();
  private:
    Adafruit_TCS34725 tcs;
};

class Gyroscope {
  private:
    enum gyroStates { START, READING, NOT_READING };
    gyroStates currentState_;
    double currentReading_;
  public:
    void init();
    double read();
    void gyroFSM();
    double currentReading();
    gyroStates currentState();
    void turnTo(int angle);

};

class LineTrack {
  private:
    double maxError = 0;
    double lastError = 0;
  public:
    void operator()(double* sensors);
    void reset();
};

class Servos {
  public:
    void init();
    void grab();
    void sort(BallType ball);// assume only used in ZONE
    void openDoor();
    bool checkKit();
    void clawUp();
    void clawDown();
  private:
    void LDRcalibrate();
    int LDR_THRESHOLD;
    int LDR_MAX;
    int LDR_MIN;
};

class ToF {
  public:
    bool obstacle(int sensorToF);
    void initial();
    double distancePID();
    void avoidOb();
    int getDistance(int Sensor);
  private:
    VL53L1X sensor;
    double ToFscaling = 0.01;
    double target = 80;
};

void tcaselect(uint8_t i);
void turnTo(double angle);
void turn(double angle);
void printTurn(Turn turn);

// Evac stage constants
enum class EvacStates {
  RIGHT_TURN,
  LEFT_TURN,
  FORWARD,  // TOF / not white / ball
  FIXED_FORWARD, // TOF / time / ball
  // LOG_POSITION,
  PICKUP,
  DEPOSIT
};

inline const EvacStates zLoop[8] = {
  EvacStates::FORWARD,
  EvacStates::LEFT_TURN,
  EvacStates::FIXED_FORWARD,
  EvacStates::LEFT_TURN,
  EvacStates::FORWARD,
  EvacStates::RIGHT_TURN,
  EvacStates::FIXED_FORWARD,
  EvacStates::RIGHT_TURN
};

inline const EvacStates start[3] = {
  EvacStates::RIGHT_TURN,
  EvacStates::FORWARD,
  EvacStates::LEFT_TURN
};

inline struct EvacState {
  EvacStates currentState = start[0];
  EvacStates previousState;
  BallType ballType;
  int zLoopState = 0;
  int zLoopCount = 0;
  bool exit = false;
  // int corner;
  // double initialAngle;
  int forwardDuration = FIXED_FORWARD_DURATION;
  unsigned long initialTime;
  int startState = 0;
} evacState;

// Evac stage functions
void advanceZLoop();
void runEvacFSM();
EvacStates switchDirection(EvacStates s);
int getCurrentCorner();
void runEvacFSM();

inline SoftwareSerial IMU_SERIAL(53, 52); // RX, TX. the one closer to the power is tx
extern RobotDriver driver;
extern RobotColourSensor colourSensor;
extern RobotLightSensor lightSensor;
extern Gyroscope gyro;
extern LineTrack PID;
extern Servos servo;
extern ToF tof;

#endif
