#include "RobotLibrary.h"
// #include <algorithm>
//TODO: pause loop with button
//TODO: Pickup 

RobotDriver driver;
RobotColourSensor colourSensor;
RobotLightSensor lightSensor;
Gyroscope gyro;
LineTrack PID;
Servos servo;
ToF tof;
LDR ldr;

void setup()
{
  Serial.begin(9600);
  gyro.init();
  driver.init();
  colourSensor.init();
	lightSensor.init();
  servo.init();
  delay(2000);
  Serial.println("Calibrate LDR!");
  ldr.init();
  tof.initial();
  
  #if defined(TEST_RIGHT_ANGLE_TURN)
    preEvacState.currentState = PreEvacStates::INITIAL_TURN;
    preEvacState.turnDirection = 1;
    preEvacState.cycles = 1;
  #endif

  #if defined(TEST_U_TURN)
    preEvacState.currentState = PreEvacStates::INITIAL_TURN;
    preEvacState.turnDirection = 1;
    preEvacState.cycles = 2;
  #endif

  // #if !(defined(TEST_180_TURN) || defined(TEST_RIGHT_ANGLE_TURN))
  //   currentState = PreEvacStates::RESET;
  // #endif
  // IMU_SERIAL.write("#oc");
  // IMU_SERIAL.write("#on");
  #if defined(TEST)
  //random code
  #endif
  // pinMode(A13, INPUT);
  Serial.println("end setting");
  initialTime = millis();

  // while (!gyro.dataReady());
  // preEvacFSM.finalAngle = (int)(gyro.read()+OBSTACLE_AVOIDANCE_TURN_ANGLE)%360;
}

void loop() {
  // //gyro.gyroFSM();
  lightSensor.updateReading();
  // for (int i = 0; i < 7; i++) {
  //   Serial.print(lightSensor.currentReading()[i]);
  //   Serial.print(" ");
  // }
  // Serial.println();
  
  // if (gyro.dataReady()) Serial.println(gyro.read());
  // if (IMU_SERIAL.available()) Serial.println(IMU_SERIAL.read());
  #if !(defined(TEST_LINE_TRACK) || defined(TEST_LIGHT_SENSOR) || defined(TEST_COLOUR_SENSORS) || defined(TEST_MOTORS)|| defined(TEST))
  // //  if (gyro.dataReady()) {
  // //   int reading = gyro.read();
  // //   int difference = (int)(preEvacFSM.finalAngle-reading)%360;
  // //   Serial.println(difference);
  // //   driver.differentialSteer(IMU_ROTATION_SPEED, 0.5);
  // //   if (abs(difference) <= 5) {
  // //     driver.halt();
  // //     while(1);
  // //   }
  // // }

  // Serial.println(analogRead(A13));
  // colourSensor.print(1, colourSensor_Right);
  preEvacFSM.run();
  // Serial.println(millis() - initialTime);
  // }else if (preEvacState.currentmode == Modes::ZONE){
  //   switch (evacState.zLoopCount)
  //   {
  //   case 1:
  //   // driver.differentialSteer(0.2, 0);
  //   // delay(500);
  //   // driver.differentialSteer(0.2, 1);
  //   // delay(Turn90);
  //   // evacState.zLoopCount = 1;
  //     break;
    
  //   case 0://pickup and go straight;
  //   if(servo.checkKit()){
  //     driver.differentialSteer(-0.5,0);
  //     delay(80);
  //     servo.grabB();
  //     servo.sort(colourSensor.checkBall());
  //   }else if (colourSensor.isColour(Green, colourSensor_Left)){
  //     preEvacState.currentmode = Modes::LINE;
  //   }else if (tof.obstacle(ToF_FRONT, 200)){
  //     evacState.zLoopCount = 2;
  //   }else{
  //     driver.differentialSteer(0.15, 0);
  //   }
  //   break;

  //   case 2://depo
  //   while(!tof.obstacle(ToF_FRONT, 130)) driver.differentialSteer(0.13, 0);
  //   driver.differentialSteer(0.3, 1);
  //   delay(1900);
  //   driver.differentialSteer(-0.15, 0);
  //   delay(500);
  //   driver.differentialSteer(0, 0);
  //   servo.openDoor();
  //   Serial.println("finished Open door");
  //   evacState.zLoopCount = 0;
  // }

  // while(tof.obstacle(ToF_FRONT, 100) && tof.obstacle(ToF_SIDE, 100)){
  //   driver.differentialSteer(0,0);
  // }
  // }// stop button. Used in normal line track, avoiding ob and aimless forward in zone.
  #endif

  //  ~~Testing stuff~~
  #ifdef TEST_COLOUR_SENSORS
    Serial.print("white:");
    Serial.println(colourSensor.isColour(White, colourSensor_Right));
    Serial.print("orange:");
    Serial.println(colourSensor.isColour(Orange, colourSensor_Left));
    Serial.println(tof.getDistance(ToF_FRONT));
  #endif

  #ifdef TEST_LIGHT_SENSOR
    double* currentReading = lightSensor.currentReading();
    for (int i = 0; i < 7; i++) {
      Serial.print(currentReading[i]);
      Serial.print(" ");
    }
    Serial.println();
    Serial.println(lightSensor.isAllBlack());
  #endif

  #ifdef TEST_LINE_TRACK
    PID(lightSensor.currentReading());
  #endif

  #ifdef TEST_MOTORS
    driver.differentialSteer(0.13, 0);
  #endif

  #ifdef TEST_FRONT_TOF
    Serial.println(tof.getDistance(ToF_FRONT));
  #endif

  #ifdef TEST_SIDE_TOF
    Serial.println(tof.getDistance(ToF_SIDE));
  #endif

  #ifdef TEST
  // if (driver.newSpeed == 0.5){
  //   driver.differentialSteer(0.5, 0);
  //   delay(1500);
  //   driver.differentialSteer(-1, 0);
  //   delay(10);
  //   driver.differentialSteer(0,0);
  //   delay(500);
  //   driver.differentialSteer(0.2, 1);
  //   delay(1900);
  //   while(!lightSensor.isAllWhite()){
  //     PID(lightSensor.currentReading());
  //     lightSensor.updateReading();
  //   }
  //   driver.differentialSteer(0,0);
  //   delay(5000);
  // int i = 0;
  // if(i == 1){
  //   Serial.println("last");
  //   servo.openDoor();
  // }
  // else if(tof.obstacle(ToF_FRONT, 100)){
  //   driver.differentialSteer(0.2, 1);
  //   delay(1900);
  //   i = 1;
  //   Serial.println("second");
  // }else{
  //   driver.differentialSteer(0.13, 0);
  // }
    while(!tof.obstacle(ToF_FRONT, 120)) driver.differentialSteer(0.13, 0);
    driver.differentialSteer(0.2, 1);
    delay(1650);
    driver.differentialSteer(-0.13, 0);
    delay(900);
    driver.differentialSteer(0, 0);
    servo.openDoor();
  #endif
}
