#pragma once
#ifndef ROBOTLIBRARY_H
#define ROBOTLIBRARY_H

inline double motorSpeed = 0.24; //0.22
inline unsigned long initialTime = 0;

#define TCAADDR 0x70
#define LDR_PIN A15
#define ToF_FRONT 4
#define ToF_SIDE 6
#define colourSensor_Left 1
#define colourSensor_Right 0
#define colourSensor_Middle 2

inline double KP = 0.36; //0.0008 with speed 0.25
inline double KD = 40;
 //0.05, 0.019
#define CALIBRATION_MOTOR_SPEED 0.2
#define ROTATION_SPEED 0.23//0.23
#define IMU_ROTATION_SPEED 0.17 // * Rotation speed when turning with IMU
#define IMU_BAUD_RATE 9600 //! Makes no damn sense (baud rate of IMU at 19200)

#define INITIAL_OBJECT_DISTANCE 140 //* Distance to begin obstacle avoidance
#define OBSTACLE_DISTANCE 100 //* Distance to be mantained between object and robot
#define OBSTACLE_ROTATION_SPEED 0.18


#define BLACK_THRESHOLD 0.5
#define TURN_DURATION 800//900
#define TURN_CONSTANT 0.9
#define FORWARD_DURATION 650
#define grabDelay 250 // checked
#define DISTANCE_THRESHOLD 50
#define Turn90 950

// ~ evac state constants ~
#define FIXED_FORWARD_DURATION 5000 // change this!!!
#define FORWARD_DETECT_COLOUR_DURATION 200 // checking for blue or green
#define MAX_BOX_DISTANCE 100 // idk this also
#define REVERSE_DURATION 500

// ~ testing switches ~
//  #define PRINT_STATE
// #define PRINT_TURN
// #define TEST_COLOUR_SENSORS
// #define TEST_LIGHT_SENSOR
// #define TEST_LINE_TRACK
// #define TEST_MOTORS
// #define TEST_FRONT_TOF
// #define TEST_SIDE_TOF
// #define TEST

// only choose one please
// #define TEST_RIGHT_ANGLE_TURN
// #define TEST_U_TURN

#include <Arduino.h>
#include <DualVNH5019MotorShield.h>
#include <QTRSensors.h>
#include "Wire.h"
#include <SoftwareSerial.h>
#include <Servo.h>
#include <VL53L1X.h>

#include "RobotColourSensor.h"
#include "PreEvacuation.h"
#include "Gyroscope.h"
// inline int halfblack = 0;//1 left, 2 right

enum class Modes { PRE_EVACUATION, EVACUATION }; 
inline Modes mode = Modes::PRE_EVACUATION;

class RobotDriver {
    public:
        void differentialSteer(double speed, double rotation);
        void halt();
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
      int halfBlack();
    private:
      void read(double* readingArray);
      void calibrate();
      double currentReading_[7];
      double minimumReadings[7];
      double maximumReadings[7];
      const uint8_t lightSensorPins[7] = {A8, A9, A10, A11, A12, A13, A14};
};

class Servos {
  public:
    void init();
    void grab();
    void sort(BallType ball);// assume only used in ZONE
    void openDoor();
    // bool checkKit();
    void clawUp();
    void clawDown();
    void grabB();
  
};

class LDR {
  public: 
    void init();
    bool checkKit();
  private:
    void LDRcalibrate();
    int LDR_THRESHOLD;
    int LDR_MAX;
    int LDR_MIN;
};

class ToF {
  public:
    // if object within range of ToF
    bool obstacle(int sensorToF, int threshold);
    void initial();
    void avoidOb();
    int getDistance(int Sensor);
    void resetObStage();
  private:
    VL53L1X sensor;
    double ToFscaling = 0.01;
    double target = 80;
    int lastDistance;
    int stage = 0;
    int stage2 = 0;
};

void tcaselect(uint8_t i);
void turnTo(double angle);
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
  unsigned long currentTime;
  int startState = 0;
} evacState;

// Evac stage functions
void advanceZLoop();
void runEvacFSM();
EvacStates switchDirection(EvacStates s);
int getCurrentCorner();
void runEvacFSM();
void adjustPosition();
bool see();

void turn(double angle);

inline SoftwareSerial IMU_SERIAL(53, 52); // RX, TX. the one closer to the power is tx
extern RobotDriver driver;
extern RobotColourSensor colourSensor;
extern RobotLightSensor lightSensor;
extern Gyroscope gyro;
extern LineTrack PID;
extern Servos servo;
extern ToF tof;
extern LDR ldr;

#endif
