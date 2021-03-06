#include "RobotLibrary.h"
#define TCAADDR 0x70

void tcaselect(uint8_t i) {
	if (i > 7) return;
	
	Wire.beginTransmission(TCAADDR);
	Wire.write(1 << i);
	Wire.endTransmission();  
}

void RobotColourSensor::init()
{
  // TCS (1) is on the starboard side
  // TCS (2) is on the port side
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

	return out;
}

bool RobotColourSensor::isGreen(uint8_t sensorAddr)
{
	if (sensorAddr > 1) {
		Serial.println("Colour sensor selected out of range");
		while(1);
	};
	float r, g, b;
	tcaselect(sensorAddr);
	tcs.getRGB(&r, &g, &b);
  
	auto HSBvalue = RGBtoHSB(r, g, b);

//  Serial.print("H:"); Serial.print(HSBvalue.hue); 
//  Serial.print(", S:"); Serial.print(HSBvalue.saturation);
//  Serial.print(", B:"); Serial.println(HSBvalue.brightness);

	if (HSBvalue.hue < 100 || HSBvalue.hue > 130) return false; // return false if out of bounds
  if (HSBvalue.saturation < 0.09 || HSBvalue.saturation > 0.42) return false;
	if (HSBvalue.brightness < 58.65 || HSBvalue.brightness > 107.1) return false;

	return true;
}

Turn RobotColourSensor::getTurn() {
  bool leftColourSensor = colourSensor.isGreen(1);
  bool rightColourSensor = colourSensor.isGreen(0);

  if (!leftColourSensor && !rightColourSensor) return Turn::NONE;
  if (!leftColourSensor) {
    return Turn::RIGHT;
  } else if (!rightColourSensor) {
    return Turn::LEFT;
  } else {
    return Turn::U_TURN;
  }
}

//void test() 
//{ 
//   float r, g, b;
//  tcs.getRGB(&r, &g, &b);
//  
//  auto HSBvalue = RGBtoHSB(r, g, b);
//  Serial.print("R: "); Serial.println(r);
//  Serial.print("G: "); Serial.println(g);
//  Serial.print("B: "); Serial.println(b);
//
//  Serial.print("H: "); Serial.println(HSBvalue.hue);
//  Serial.print("S: "); Serial.println(HSBvalue.saturation);
//  Serial.print("B: "); Serial.println(HSBvalue.brightness);
//}
