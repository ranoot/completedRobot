#pragma once
#ifndef ROBOTLIBRARY_H
#define ROBOTLIBRARY_H

#define KP 0.0008 //0.0008 with speed 0.25
#define KD 0.019 //0.05, 0.019

#define KP_TURN 0.0008
#define KD_TURN 0.019

#define motorSpeed 0.25 //0.23
#define M1 100
#define M2 100
#define IMU_BAUD_RATE 9600
// A15 for LDR
#define BLACK_THRESHOLD 600
#define TURN_DURATION 1500

#include <Arduino.h>
#include <DualVNH5019MotorShield.h>
#include <Adafruit_TCS34725.h>
#include <QTRSensors.h>
#include "Wire.h"
#include <SoftwareSerial.h>

inline const uint8_t lightSensorPins[7] = {A8, A9, A10, A11, A12, A13, A14};

void tcaselect(uint8_t i);
void turnTo(double angle);
void turn(double angle);

enum class Turn { NONE, RIGHT, LEFT, U_TURN };

enum class States {
  RESET, 
  LINE_TRACK, 
  READ_COLOUR_SENSORS, 
  INITIAL_TURN,
  WAIT,
  READ_BLACK_LINE
};

struct State {
  States currentState;
  unsigned long initialTime;
  int turnDirection;
  int turnNumber;
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
        QTRSensors& qtrRef();
        bool isAllBlack();
        void updateReading();
        uint16_t* currentReading();
    private:
        QTRSensors qtr;
        uint16_t currentReading_[7];
};

class RobotColourSensor {
  public:
    void init();
    bool isGreen(uint8_t sensorAddr);
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
    void operator()(uint16_t* sensors);
    void reset();
};

inline SoftwareSerial IMU_SERIAL(53, 52); // RX, TX. the one closer to the power is tx
extern RobotDriver driver;
extern RobotColourSensor colourSensor;
extern RobotLightSensor lightSensor;
extern Gyroscope gyro;
extern LineTrack PID;

#endif