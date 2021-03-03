#include "RobotLibrary.h"

#define KP 0.1
#define KD 5
#define motorSpeed 0.5

RobotDriverBase driver;
Adafruit_TCS34725 tcs;
QTRSensors qtr;

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
	qtr.setTypeRC(); // or setTypeAnalog()
  	qtr.setSensorPins((const uint8_t[]){A0, A1, A2}, 3);

	for (uint8_t i = 0; i < 250; i++)
	{
		qtr.calibrate();
		delay(20);
	}

}

double PID()
{
	static uint16_t lastError = 0;
	uint16_t sensors[7];

	int16_t position = qtr.readLineBlack(sensors);

	int16_t error = position - 4000;

	int16_t rotation = KP * error + KD * (error - lastError);
	lastError = error;

	driver.differentialSteer(motorSpeed, rotation);
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

