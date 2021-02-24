#include "RobotLibrary.h"

RobotDriverBase driver;

void setup()
{
	driver.setup();
}

void loop()
{
	driver.differentialSteer(1, 0);
}

