#pragma once
#ifndef ROBOTLIBRARY_H
#define ROBOTLIBRARY_H

#define KP 0.205 //0.0008 with speed 0.25
#define KD 10//12
 //0.05, 0.019
#define LDR_PIN A15
#define motorSpeed 0.18 //0.22
#define CALIBRATION_MOTOR_SPEED 0.2
#define ROTATION_SPEED 0.23
#define IMU_BAUD_RATE 9600

#define BLACK_THRESHOLD 0.5
#define TURN_DURATION 900
#define TURN_CONSTANT 0.85
#define FORWARD_DURATION 500

// ~ testing switches ~
// #define PRINT_STATE
#define PRINT_TURN
// #define TEST_COLOUR_SENSORS
// #define TEST_LIGHT_SENSOR
// #define TEST_LINE_TRACK
// #define TEST_MOTORS

// only choose one please
// #define TEST_RIGHT_ANGLE_TURN
// #define TEST_U_TURN

#include <Arduino.h>
#include <DualVNH5019MotorShield.h>
#include <Adafruit_TCS34725.h>
#include <QTRSensors.h>
#include "Wire.h"
#include <SoftwareSerial.h>

inline const char* stateStr[] = {
  "RESET", 
  "LINE_TRACK", 
  "READ_COLOUR_SENSORS", 
  "INITIAL_TURN",
  "WAIT",
  "READ_BLACK_LINE"
};
inline const char* turnsStr[] = {"NONE", "RIGHT", "LEFT", "U_TURN"};

enum class Turn { NONE, RIGHT, LEFT, U_TURN };

enum class States {
  RESET, 
  LINE_TRACK, 
  READ_COLOUR_SENSORS, 
  INITIAL_TURN,
  WAIT,
  READ_BLACK_LINE,
  INITIAL_FORWARD // set waitDuratioin, forward -> wait
};

inline struct State {
  States currentState;
  // int currentState = 0;
  unsigned long initialTime;
  int turnDirection;
  int cycles = 0;
  int waitDuration;
  int counter;
} state;

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
      bool isAllBlack();
      bool isAllWhite();
      void updateReading();
      double* currentReading();
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
    bool isGreen(float r, float g, float b);
    HSB RGBtoHSB(double red, double green, double blue);
    Turn getTurn();
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

};

class LineTrack {
  private:
    double maxError = 0;
    double lastError = 0;
  public:
    void operator()(double* sensors);
    void reset();
};

void tcaselect(uint8_t i);
void turnTo(double angle);
void turn(double angle);
void printTurn(Turn turn);

inline SoftwareSerial IMU_SERIAL(53, 52); // RX, TX. the one closer to the power is tx
extern RobotDriver driver;
extern RobotColourSensor colourSensor;
extern RobotLightSensor lightSensor;
extern Gyroscope gyro;
extern LineTrack PID;

#endif