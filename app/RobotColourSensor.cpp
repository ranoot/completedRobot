#include "RobotLibrary.h"

void tcaselect(uint8_t i) 
{
	if (i > 7) return;
	
	Wire.beginTransmission(TCAADDR);
	Wire.write(1 << i);
	Wire.endTransmission();  
}

void printTurn(Turn turn)
{
  Serial.println( turnsStr[static_cast<int>(turn)] );
}

void RobotColourSensor::init()
{
  // TCS (1) is on the starboard side
  // TCS (2) is on the port side
  tcs.setIntegrationTime(TCS34725_INTEGRATIONTIME_50MS);
  tcs.setGain(TCS34725_GAIN_16X);
	tcaselect(0);
	if (tcs.begin()) {
  	Serial.println("Found sensor");
	} else {
  	Serial.println("TCS34725 (1) not found");
  	// while (1);
	}

	tcaselect(1);
	if (tcs.begin()) {
  	Serial.println("Found sensor");
	} else {
  	Serial.println("TCS34725 (2) not found");
  	// while (1);
	}
 
  tcaselect(2);
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("TCS34725 (2) not found");
    // while (1);
  }
}

HSB RobotColourSensor::RGBtoHSB(double red, double green, double blue)
{
  //red /= 255, green /= 255, blue /= 255;
  double min, max, chroma;
  HSB out;

  min = red < green ? red : green;
  min = min < blue ? min : blue;

  max = red > green ? red : green;
  max = max > blue ? max : blue;

  chroma = max - min;

  out.brightness = max;
  out.saturation = out.brightness == 0 ? 0 : chroma/max;

  if (chroma == 0) {
    out.hue = 0;
  } else if (max == red) {
    out.hue = 60.00 * ((green - blue)/chroma);
  } else if (max == green) {
    out.hue = 60.00 * (2.00 + (blue-red)/chroma);
  } else if (max == blue) {
    out.hue = 60.00 * (4.00 + (red - green)/chroma);
  }

  // #ifdef TEST_COLOUR_SENSORS
  //   Serial.print("R:"); Serial.print(red); 
  //   Serial.print(", G:"); Serial.print(green);
  //   Serial.print(", B:"); Serial.print(blue); Serial.print("; ");

  //   Serial.print("H:"); Serial.print(out.hue); 
  //   Serial.print(", S:"); Serial.print(out.saturation);
  //   Serial.print(", B:"); Serial.print(out.brightness); Serial.print("; ");
  // #endif

  return out;
}

// bool RobotColourSensor::isColour(double color[2][3], uint8_t chosenSensor){
//   float r, g, b;
//   tcaselect(chosenSensor);
//   tcs.getRGB(&r, &g, &b);
//   auto HSBvalue = RGBtoHSB(r, g, b);

//   if (HSBvalue.hue < color[0][0] || HSBvalue.hue > color[1][0]) return false; // return false if out of bounds
//   if (HSBvalue.saturation < color[0][1] || HSBvalue.saturation > color[1][1]) return false;
//   if (HSBvalue.brightness < color[0][2] || HSBvalue.brightness > color[1][2]) return false;

//   return true;
// }

bool RobotColourSensor::isColour(const ColoursProperty& c, uint8_t chosenSensor){
  float r, g, b;
  tcaselect(chosenSensor);
  tcs.getRGB(&r, &g, &b);
  auto HSBvalue = RGBtoHSB(r, g, b);

  if (HSBvalue.hue < c.minHSB.hue || HSBvalue.hue > c.maxHSB.hue) return false; // return false if out of bounds
  if (HSBvalue.saturation < c.minHSB.saturation || HSBvalue.saturation > c.maxHSB.saturation) return false;
  if (HSBvalue.brightness < c.minHSB.brightness || HSBvalue.brightness > c.maxHSB.brightness) return false;

  return true;
}

Turn RobotColourSensor::getTurn() {
  bool leftColourSensor = false, rightColourSensor = false;
  
  for (int i = 0; (i < 10 && !(leftColourSensor && rightColourSensor)); i++) {
    if(colourSensor.isColour(Green, colourSensor_Left)) leftColourSensor = true;
    if(colourSensor.isColour(Green, colourSensor_Right)) rightColourSensor = true;
  }

  if (!leftColourSensor && !rightColourSensor) return Turn::NONE;
  if (!leftColourSensor) {
    return Turn::RIGHT;
  } else if (!rightColourSensor) {
    return Turn::LEFT;
  } else {
    Serial.println("u");
    return Turn::U_TURN;
  }
}

BallType RobotColourSensor::checkBall(){
  if (colourSensor.isColour(White, colourSensor_Middle))return BallType::WhiteB;
  if (colourSensor.isColour(Orange, colourSensor_Middle))return BallType::OrangeB;
  return BallType::NoB;
}

void RobotColourSensor::print(const int option, const uint8_t chosenSensor)
{
  float r, g, b;
  tcaselect(chosenSensor);
  tcs.getRGB(&r, &g, &b);
  switch (option) {
    case 0:
      {
        Serial.print("R:"); Serial.print(r); 
        Serial.print(", G:"); Serial.print(g);
        Serial.print(", B:"); Serial.print(b); Serial.print("; ");
      }
      break;

    case 1:
      {
        HSB c = RGBtoHSB(r, g, b);
        Serial.print("H:"); Serial.print(c.hue); 
        Serial.print(", S:"); Serial.print(c.saturation);
        Serial.print(", B:"); Serial.print(c.brightness); Serial.print("; ");
      }
      break;
  }
}
