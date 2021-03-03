#include "RobotLibrary.h"

// RobotDriverBase driver;
Adafruit_TCS34725 tcs;

void setup()
{
	// driver.init();
	Serial.begin(9600);
	if (tcs.begin()) {
    	Serial.println("Found sensor");
  	} else {
    	Serial.println("No TCS34725 found ... check your connections");
    	while (1);
  	}
}

void loop(void) {
	uint16_t r, g, b, c, colorTemp, lux;

	tcs.getRawData(&r, &g, &b, &c);
	// colorTemp = tcs.calculateColorTemperature(r, g, b);
	colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
	lux = tcs.calculateLux(r, g, b);

	Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
	Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
	Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
	Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
	Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
	Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
	Serial.println(" ");
}

