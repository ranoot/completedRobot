#include "RobotLibrary.h"

#define KP 0.1
#define KD 5
#define motorSpeed 0.005
#define TCAADDR 0x70
#define M1 100
#define M2 100

Adafruit_TCS34725 tcs1;
Adafruit_TCS34725 tcs2;

QTRSensors qtr;
DualVNH5019MotorShield md;
 
void tcaselect(uint8_t i) {
	if (i > 7) return;
	
	Wire.beginTransmission(TCAADDR);
	Wire.write(1 << i);
	Wire.endTransmission();  
}

void setup()
{
	Serial.begin(9600);
	md.init();
	// Serial.begin(9600);
	// if (tcs.begin()) {
    // 	Serial.println("Found sensor");
  	// } else {
    // 	Serial.println("No TCS34725 found ... check your connections");
    // 	while (1);
  	// }
	qtr.setTypeAnalog(); // or setTypeAnalog()
  	qtr.setSensorPins((const uint8_t[]){A8, A9, A10, A11, (uint8_t)'\036', (uint8_t)'\037', (uint8_t)'\038'}, 7);

	for (uint8_t i = 0; i < 250; i++)
	{
		qtr.calibrate();
		delay(20);
	}

	// tcaselect(0);
	// tcs1.begin();
	// tcaselect(1);
	// tcs2.begin();
}

double PID()
{
	static uint16_t lastError = 0;
	uint16_t sensors[7];

	int16_t position = qtr.readLineBlack(sensors);

	int16_t error = position - 3000;

	int16_t rotation = KP * error + KD * (error - lastError);
	lastError = error;

	int16_t m1Speed = M1 + motorSpeed;
  	int16_t m2Speed = M2 - motorSpeed;
	md.setSpeeds(m1Speed, -m2Speed);
}

void differentialSteer(double speed, double rotation)
{
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

bool withinRange(Adafruit_TCS34725 tcs, int16_t rMax, int16_t rMin, int16_t gMax, uint16_t gMin, uint16_t bMax, uint16_t bMin)
{
	uint16_t r, g, b, c;

	tcs.getRawData(&r, &g, &b, &c);
	return 
		(r >= rMin && r <= rMax) && // 8 < R < 12
		(g >= gMin && g <= gMax) && // 10 < G < 15
		(b >= bMin && b <= bMax); // 6 < B < 9
}

bool isGreen(Adafruit_TCS34725 tcs) { return withinRange(tcs, 12, 8, 15, 10, 9, 6); }

void checkGreen()
{
	#define angularMotorSpeed 0.5
	tcaselect(0);
	bool leftColourSensor = isGreen(tcs1);

	tcaselect(1);
	bool rightColourSensor = isGreen(tcs2);

	if (!leftColourSensor && !rightColourSensor)return;
	if (!leftColourSensor) {
		differentialSteer(motorSpeed, 0.5);
		delay(1500);
		differentialSteer(motorSpeed, 0);
	} else if (!rightColourSensor) {
		differentialSteer(motorSpeed, -0.5);
		delay(1500);
		differentialSteer(motorSpeed, 0);
	} else {
		differentialSteer(motorSpeed, 1);
		delay(3000);
		differentialSteer(motorSpeed, 0);
	}
}

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
	PID();
}

