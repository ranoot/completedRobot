// #include <ArduinoSTL.h>
// #include <system_configuration.h>
// #include <unwind-cxx.h>

#include "RobotLibrary.h"
// #include <algorithm>

#define KP 0.140
#define KD 0.0 
#define motorSpeed 100
#define M1 100
#define M2 100
#define IMU_SERIAL Serial2

RobotDriver driver;
RobotColourSensor colourSensor;
RobotLightSensor lightSensor;
 
void setup()
{
	Serial.begin(9600);
  IMU_SERIAL.begin(115200);
	driver.init();
	colourSensor.init();
	lightSensor.init();

}

double PID()
{
	const double maxError = 2785;
	static uint16_t lastError = 0;
	uint16_t sensors[7];

	lightSensor.qtrRef().read(sensors);

	double error = 0;
	uint16_t minValue{sensors[0]};
	for (int i = 0, sensorID = -3; i < 7; i++, sensorID++)
	{
		double sensorReading = (double)sensors[i];
//		Serial.print(sensorID);
//		Serial.print(": ");
//		Serial.print(sensorReading);
//		Serial.print(" ");
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
	
//	driver.differentialSteer(0.3, rotation);

//	Serial.print(error);
//	Serial.print(" -> ");
//	Serial.println(rotation);
	double m1Speed = rotation > 0 ? motorSpeed : motorSpeed + rotation;
  double m2Speed = rotation > 0 ? motorSpeed - rotation : motorSpeed;
	driver.get_md().setSpeeds(m1Speed, -m2Speed);
}

//void test() 
//{ 
// 	float r, g, b;
// 	tcs.getRGB(&r, &g, &b);
//	
// 	auto HSBvalue = RGBtoHSB(r, g, b);
//  Serial.print("R: "); Serial.println(r);
//  Serial.print("G: "); Serial.println(g);
//  Serial.print("B: "); Serial.println(b);
//
// 	Serial.print("H: "); Serial.println(HSBvalue.hue);
// 	Serial.print("S: "); Serial.println(HSBvalue.saturation);
// 	Serial.print("B: "); Serial.println(HSBvalue.brightness);
//}

void checkGreen()
{
	#define angularMotorSpeed 0.2
	bool leftColourSensor = colourSensor.isGreen(1);
	bool rightColourSensor = colourSensor.isGreen(0);

	if (!leftColourSensor && !rightColourSensor){
  Serial.println("TURNING");
		return;
	}
	if (!leftColourSensor) {
		driver.differentialSteer(angularMotorSpeed, 0.5);
		delay(1500);
    Serial.println("1");
		// driver.differentialSteer(angularMotorSpeed, 0);
	} else if (!rightColourSensor) {
		driver.differentialSteer(angularMotorSpeed, -0.5);
		delay(1500);
      Serial.println("2");
		// driver.differentialSteer(angularMotorSpeed, 0);
	} else {
		driver.differentialSteer(angularMotorSpeed, 1);
		delay(3000);
      Serial.println("U");
		// driver.differentialSteer(angularMotorSpeed, 0);
	}
}

void loop() {
  PID();
}
