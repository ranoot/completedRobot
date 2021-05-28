#include "RobotLibrary.h"

Servo servo1, servo2, servo3, servo4;//left right big door

void Servos::init(){
  servo1.attach(3);
  servo2.attach(5);
  servo3.attach(11);
  servo4.attach(13);
  servo1.write(100);//--to move inside(clock);50 to grab
  servo2.write(0);
  servo3.write(170);
  servo4.write(0);
  // for (int i = 0; i < 700; i++){
  //   LDRcalibrate();
}

void Servos::grab(){
  servo3.write(0);
  delay(2000);
  servo2.write(50);
  servo1.write(50);
  delay(2000);
  servo3.write(170);
  delay(1500);
  if (state.currentmode == Modes::LINE)
  {
    servo1.write(100);
    servo2.write(0);
    delay(2000);
  }
  
}

void Servos::sort(BallType ball){
  if (ball == BallType::OrangeB){
    servo1.write(50);
    delay(2000);
    servo1.write(100);
    delay(100);
  }
  if (ball == BallType::WhiteB){
    servo1.write(100);
    delay(2000);
    servo2.write(100);
    delay(100);
  }else{
    servo2.write(0);
    servo1.write(100);
    delay(2000);
  }
  servo3.write(0);
  delay(100);
}

void Servos::openDoor(){
  // servo4.write(90);
  servo2.write(100);
  delay(1000);
  servo1.write(100);
  delay(1000);
}

void Servos::LDRcalibrate(){
  int brightness = analogRead(LDR_PIN);
  if(LDR_MIN > brightness) LDR_MIN = brightness;
  if(LDR_MAX < brightness) LDR_MAX = brightness;
  LDR_THRESHOLD = (LDR_MIN + LDR_MAX)*0.5;
  Serial.println(LDR_THRESHOLD);
}

bool Servos::checkKit(){
  int brightness = analogRead(LDR_PIN);
  // Serial.println(brightness);
  // Serial.println("was for servo");
  if (brightness < LDR_THRESHOLD){
    return true;
  }else{
    return false;
  }
}

void Servos::clawUp() {
  servo3.write(170);
  delay(2000);
  servo2.write(100);
  servo1.write(100);
  delay(2000);
}

void Servos::clawDown() {
  servo3.write(0);
  delay(2000);
  servo2.write(100);
  servo1.write(100);
  delay(2000);
}