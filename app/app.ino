#include "RobotLibrary.h"

RobotDriverBase driver;

void setup()
{
	driver.setup();
}

void loop()
{
	driver.differentialSteer(1, 0);
	delay(1000);
	driver.differentialSteer(1, 1);
}

