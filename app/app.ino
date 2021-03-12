// #include <ArduinoSTL.h>
// #include <system_configuration.h>
// #include <unwind-cxx.h>

#include "RobotLibrary.h"
// #include <algorithm>

#define KP 0.0003
#define KD 0.00001
#define motorSpeed 0.005
#define M1 100
#define M2 100

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_4X);

QTRSensors qtr;
DualVNH5019MotorShield md;
 
void setup()
{
	Serial.begin(9600);
	md.init();
	
	qtr.setTypeAnalog(); // or setTypeAnalog()
  	qtr.setSensorPins((const uint8_t[]){A7, A8, A9, A10, A11, A12, A13}, 7);

	for (uint8_t i = 0; i < 150; i++)
	{
		qtr.calibrate();
		delay(10);
		Serial.println(i);
	}
}

double PID()
{
	#define maxError 2785
	static uint16_t lastError = 0;
	uint16_t sensors[7];

	qtr.read(sensors);

	double error{0};
	uint16_t minValue{sensors[0]};
	for (int i = 0, sensorID = -3; i < 7; i++, sensorID++)
	{
		double sensorReading = (double)sensors[i];
		// Serial.print(sensorID);
		// Serial.print(": ");
		// Serial.print(sensorReading);
		// Serial.print(" ");
		error += sensorID * sensorReading;
	}
	for (int j = 1; j <= 6; j++) {
		// Serial.println(minValue);
		minValue = (sensors[j] > minValue) ? sensors[j] : minValue;
	}
	if (sensors[0] == minValue) {
		error = -maxError;
		// Serial.println("minleft");
	} else if (sensors[6] == minValue) {
		error = maxError;
		// Serial.println("minright");
	}

	// uint16_t* maximumValue = std::max_element(sensors, sensors + 6);
	// if (maximumValue == (sensors + 6)) {
	// 	error = 27000;
	// } else if (maximumValue == sensors) {
	// 	error = -27000;
	// }
	// double D = error - lastError;
	lastError = error;
	double rotation{(error * KP) + ((error - lastError) * KD)};
	
	differentialSteer(0.19, rotation);

	// Serial.print(error);
	// Serial.print(" -> ");
	// Serial.println(rotation);
	// int16_t m1Speed = M1 + rotation;
  	// int16_t m2Speed = M2 - rotation;
	// md.setSpeeds(m1Speed, -m2Speed);
}

void differentialSteer(double speed, double rotation)
{
	// positive rotation is clockwise
	if (rotation>=0)
	{
		md.setM1Speed(400*speed);
		md.setM2Speed(-(400*(speed-2*speed*fabs(rotation)))); // negative to ensure that
	}
	if (rotation<=0)
	{
		md.setM1Speed(400*(speed-2*speed*fabs(rotation)));
		md.setM2Speed(-(400*speed));
	}
}

// void test() 
// { 
// 	uint16_t r, g, b, c;
// 	tcs.getRawData(&r, &g, &b, &c);
	
// 	HSB HSBvalue = RGBtoHSB(r, g, b);
// 	Serial.print("H: "); Serial.println(HSBvalue.hue);
// 	Serial.print("S: "); Serial.println(HSBvalue.saturation);
// 	Serial.print("B: "); Serial.println(HSBvalue.brightness);
// }

// void checkGreen()
// {
// 	#define angularMotorSpeed 0.5
// 	tcaselect(0);
// 	bool leftColourSensor = isGreen(tcs1);

// 	tcaselect(1);
// 	bool rightColourSensor = isGreen(tcs2);

// 	if (!leftColourSensor && !rightColourSensor)return;
// 	if (!leftColourSensor) {
// 		differentialSteer(motorSpeed, 0.5);
// 		delay(1500);
// 		differentialSteer(motorSpeed, 0);
// 	} else if (!rightColourSensor) {
// 		differentialSteer(motorSpeed, -0.5);
// 		delay(1500);
// 		differentialSteer(motorSpeed, 0);
// 	} else {
// 		differentialSteer(motorSpeed, 1);
// 		delay(3000);
// 		differentialSteer(motorSpeed, 0);
// 	}
// }

void loop() {
	// uint16_t r, g, b, c, colorTemp, lux;

	// tcs.getRawData(&r, &g, &b, &c);
	// // colorTemp = tcs.calculateColorTemperature(r, g, b);
	// colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
	// lux = tcs.calculateLux(r, g, b);

	// Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
	// Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
	// Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
	// Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
	// Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
	// Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
	// Serial.println(" ");
	// differentialSteer(0.2, 0.5);
	// delay(1000);
	// differentialSteer(0.2, -0.5);
	// delay(1000);
	// differentialSteer(0.2, 0);
	// delay(1000);

	PID();
}

