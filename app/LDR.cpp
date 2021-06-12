#include "RobotLibrary.h"

void LDR::init() {
  for (int i = 0; i < 700; i++) {
    LDRcalibrate();
  }
  LDR_THRESHOLD = (LDR_MIN + LDR_MAX)*0.5;
  Serial.println(LDR_THRESHOLD);
}

void LDR::LDRcalibrate(){
  int brightness = analogRead(LDR_PIN);
  if(LDR_MIN > brightness) LDR_MIN = brightness;
  if(LDR_MAX < brightness) LDR_MAX = brightness;
  // Serial.println(LDR_THRESHOLD);
}

bool LDR::checkKit(){
  int brightness = analogRead(LDR_PIN);
  // Serial.println(brightness);
  // Serial.println("was for servo");
  return (brightness < LDR_THRESHOLD);
}
