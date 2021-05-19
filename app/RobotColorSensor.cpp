#include "RobotLibrary.h"
#define TCAADDR 0x70

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

  #ifdef TEST_COLOUR_SENSORS
    Serial.print("R:"); Serial.print(red); 
    Serial.print(", G:"); Serial.print(green);
    Serial.print(", B:"); Serial.print(blue); Serial.print("; ");

    Serial.print("H:"); Serial.print(out.hue); 
    Serial.print(", S:"); Serial.print(out.saturation);
    Serial.print(", B:"); Serial.print(out.brightness); Serial.print("; ");
  #endif
  // Serial.print("H:"); Serial.print(out.hue); 
  // Serial.print(", S:"); Serial.print(out.saturation);
  // Serial.print(", B:"); Serial.print(out.brightness); Serial.print("; ");
  return out;
}

bool RobotColourSensor::isGreen(float r, float g, float b)
{
	// if (sensorAddr > 1) {
	// 	Serial.println("Colour sensor selected out of range");
	// 	while(1);
	// };
	// float r, g, b;
	// tcaselect(sensorAddr);
	// tcs.getRGB(&r, &g, &b);
  
	auto HSBvalue = RGBtoHSB(r, g, b);

  if (HSBvalue.hue < 70 || HSBvalue.hue > 150) return false; // return false if out of bounds
  if (HSBvalue.saturation < 0.10 || HSBvalue.saturation > 0.6) return false;
  if (HSBvalue.brightness < 50.65 || HSBvalue.brightness > 118.1) return false;

	return true;
}

Turn RobotColourSensor::getTurn() {
  float r1, g1, b1, r2, g2, b2;
  
  tcaselect(0);
  tcs.getRGB(&r1, &g1, &b1); // Obtain both readings first to reduce delay between both readings

  tcaselect(1);
  tcs.getRGB(&r2, &g2, &b2);

  #ifdef TEST_COLOUR_SENSORS
    Serial.print("Left: ");
  #endif
  bool leftColourSensor = colourSensor.isGreen(r2, g2, b2);
  #ifdef TEST_COLOUR_SENSORS
    Serial.println(leftColourSensor);
  #endif

  #ifdef TEST_COLOUR_SENSORS
    Serial.print("Right: ");
  #endif
  bool rightColourSensor = colourSensor.isGreen(r1, g1, b1);
  #ifdef TEST_COLOUR_SENSORS
    Serial.println(rightColourSensor);
  #endif

  if (!leftColourSensor && !rightColourSensor) return Turn::NONE;
  if (!leftColourSensor) {
    return Turn::RIGHT;
  } else if (!rightColourSensor) {
    return Turn::LEFT;
  } else {
    return Turn::U_TURN;
  }
}
