// #include <ArduinoSTL.h>
// #include <system_configuration.h>
// #include <unwind-cxx.h>

#include "RobotLibrary.h"
// #include <algorithm>

#define KP 0.0008 //0.0008 with speed 0.25
#define KD 0.019 //0.05, 0.019
#define motorSpeed 0.25 //0.23
#define M1 100
#define M2 100
#define IMU_SERIAL Serial2

RobotDriver driver;
RobotColourSensor colourSensor;
RobotLightSensor lightSensor;
IMU IMU;
 
void setup()
{
	Serial.begin(9600);
  IMU_SERIAL.begin(115200);
	driver.init();
	colourSensor.init();
	//lightSensor.init();

}

double PID()
{
	static double maxError = 0;
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
  if (error > maxError) maxError = error;
	for (int j = 1; j <= 6; j++) {
		// Serial.println(minValue);
		minValue = (sensors[j] > minValue) ? sensors[j] : minValue;
	}
	if (sensors[0] == minValue) {
		error = -maxError;
		//Serial.println("minleft");
	} else if (sensors[6] == minValue) {
		error = maxError;
		//Serial.println("minright");
	}
	// uint16_t* maximumValue = std::max_element(sensors, sensors + 6);
	// if (maximumValue == (sensors + 6)) {
	// 	error = 27000;
	// } else if (maximumValue == sensors) {
	// 	error = -27000;
	// }
	// double D = error - lastError;
	double rotation{(error * KP) + ((error - lastError) * KD)};
	
	if (rotation > 1) rotation = 1;
  if (rotation < -1) rotation = -1; 
	
	driver.differentialSteer(motorSpeed, rotation);
 
  lastError = error;

//	Serial.print(error);
//	Serial.print("->");
	Serial.println(rotation);
//	double m1Speed = rotation > 0 ? motorSpeed : motorSpeed + rotation;
//  double m2Speed = rotation > 0 ? motorSpeed - rotation : motorSpeed;
//	driver.get_md().setSpeeds(m1Speed, -m2Speed);
//  Serial.print(m1Speed); Serial.print(" | "); Serial.println(m2Speed); 
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

void turn(double angle)
{
  double initialAngle = IMU.read();
  Serial.print("Initial Angle: "); Serial.println(initialAngle);
  double difference = 0;
  while (difference < fabs(angle)) {
    driver.differentialSteer(motorSpeed, angle/180);
    double reading = IMU.read();
    difference = fabs(reading - initialAngle);
    Serial.print("Current: "); Serial.println(reading);
  }
  driver.differentialSteer(0, 0);
}

void checkGreen()
{
	bool leftColourSensor = colourSensor.isGreen(1);
	bool rightColourSensor = colourSensor.isGreen(0);

	if (!leftColourSensor && !rightColourSensor) return;
	if (!leftColourSensor) {
	  turn(90);
    Serial.println("Turn right");
	} else if (!rightColourSensor) {
		turn(-90);
    Serial.println("Turn left");
	} else {
    turn(180);
    Serial.println("180 turn");
	}
}

void loop() {
  Serial.println(IMU.read());
}
