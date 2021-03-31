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
	red /= 255, green /= 255, blue /= 255;
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
		out.hue = 60 * ((green - blue)/chroma);
	} else if (max == green) {
		out.hue = 60 * (2 + (blue-red)/chroma);
	} else if (max == blue) {
		out.hue = 60 * (4 + (red - green)/chroma);
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

	if (HSBvalue.hue < 90 || HSBvalue.hue > 150) return false; // return false if out of bounds
  if (HSBvalue.saturation < 0.23 || HSBvalue.saturation > 0.42) return false;
	if (HSBvalue.brightness < 0.23 || HSBvalue.brightness > 0.40) return false;

	tcaselect(0);

	return true;
}
