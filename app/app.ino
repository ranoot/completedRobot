#include "RobotLibrary.h"
// #include <algorithm>

RobotDriver driver;
RobotColourSensor colourSensor;
RobotLightSensor lightSensor;
Gyroscope gyro;
 
void setup()
{
	Serial.begin(9600);
  IMU_SERIAL.begin(115200);
	driver.init();
	colourSensor.init();
	//lightSensor.init();
  delay(2000);

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
	double rotation{(error * KP) + ((error - lastError) * KD)};
	
	if (rotation > 1) rotation = 1; // limit rotation to between 1 and -1
  if (rotation < -1) rotation = -1; 
	
	driver.differentialSteer(motorSpeed, rotation);
 
  lastError = error;

//	Serial.print(error);
//	Serial.print("->");
//	Serial.println(rotation);
}

//void turn(double angle)
//{
//  double initialAngle = gyro.read();
//  Serial.print("Initial Angle: "); Serial.println(initialAngle);
//  double difference = 0;
//  while (difference < fabs(angle)) {
//    driver.differentialSteer(motorSpeed, angle/180);
//    double reading = gyro.read();
//    difference = fabs(reading - initialAngle);
//    Serial.print("Current: "); Serial.println(reading);
//  }
//  driver.differentialSteer(0, 0);
//}

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

#define LDRpin A15

void checkRescueKit()
{
  for (int LDR = analogRead(A15); LDR > 300; LDR = analogRead(A15)) driver.differentialSteer(motorSpeed, 0);
  driver.differentialSteer(0, 0);
  delay(5000);
}

void loop() {
//  Serial.println("Start");
//  checkRescueKit();
//  delay(5000);
  Serial.println(analogRead(A15));
}
