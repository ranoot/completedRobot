#include "RobotLibrary.h"

Servo servo1, servo2, clawArm, door;//left right big door

void Servos::init(){
  servo1.attach(3);
  servo2.attach(5);
  clawArm.attach(11);
  door.attach(13);
  servo1.write(100);//--to move inside(clock);50 to grab
  servo2.write(0);
  clawArm.write(170);
  door.write(20);
}

void Servos::grab(){
  clawArm.write(0);
  delay(2000);
  servo2.write(50);
  servo1.write(50);
  delay(2000);
  clawArm.write(170);
  delay(1500);
  if (mode == Modes::PRE_EVACUATION)
  {
    servo2.write(0);
    delay(2000);
    servo1.write(100);
    delay(100);
  }
  
}

void Servos::sort(BallType ball){
  if (ball == BallType::OrangeB){
    servo2.write(0);
    delay(2000);
    servo1.write(100);
    delay(100);
  }
  else{
    servo1.write(100);
    delay(2000);
    servo2.write(0);
    delay(100);
  }
}

void Servos::openDoor(){
  door.write(0);
  delay(5000);
  door.write(40);
  delay(5000);
  door.write(20);
}

void Servos::clawUp() {
  clawArm.write(170);
  delay(2000);
  servo2.write(0);
  servo1.write(100);
  delay(2000);
}

void Servos::clawDown() {
  clawArm.write(0);
  delay(2000);
  servo2.write(0);
  servo1.write(100);
  delay(2000);
}

void Servos::grabB(){
  driver.differentialSteer(-0, 0);
  delay(50);
  clawArm.write(0);
  // driver.differentialSteer(-0.5, 0);
  // delay(50);
  driver.differentialSteer(0, 0);
  delay(950);
  servo2.write(50);
  servo1.write(50);
  delay(2000); 
  clawArm.write(170);
  delay(1500);
}
