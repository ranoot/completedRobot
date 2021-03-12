#include "RobotLibrary.h"
#define TCAADDR 0x70


void tcaselect(uint8_t i) {
	if (i > 7) return;
	
	Wire.beginTransmission(TCAADDR);
	Wire.write(1 << i);
	Wire.endTransmission();  
}

void RobotDriver::init() 
{
    md.init();
}

void RobotDriver::differentialSteer(double speed, double rotation) 
{
	if (rotation>=0)
	{
		md.setM1Speed(400*speed);
		md.setM2Speed(400*(speed-2*speed*fabs(rotation)));
	}
	if (rotation<=0)
	{
		md.setM1Speed(400*(speed-2*speed*fabs(rotation)));
		md.setM2Speed(400*speed);
	}
    stopIfFault();
}

void RobotDriver::stopIfFault()
{
	if (md.getM1Fault())
	{
		Serial.println("M1 fault");
		while(1);
	}
	if (md.getM2Fault())
	{
		Serial.println("M2 fault");
		while(1);
	}
}

void RobotColourSensor::init()
{
	tcaselect(0);
	Serial.begin(9600);
	if (tcs1.begin()) {
    	Serial.println("Found sensor");
  	} else {
    	Serial.println("TCS34725 (1) not found");
    	while (1);
  	}

	tcaselect(1);
	Serial.begin(9600);
	if (tcs2.begin()) {
    	Serial.println("Found sensor");
  	} else {
    	Serial.println("TCS34725 (2) not found");
    	while (1);
  	}
}

HSB RobotColourSensor::RGBtoHSB(int red, int green, int blue)
{
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