#include "RobotLibrary.h"

void RobotDriverBase::init() 
{
    md.init();
}

void RobotDriverBase::differentialSteer(double speed, double rotation) 
{
	if (speed > 1 || speed < -1 || rotation > 1 || rotation < -1) return; //throw range_error
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

void RobotDriverBase::stopIfFault()
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


bool RobotColourSensor::withinRange(int16_t rMax, int16_t rMin, int16_t gMax, uint16_t gMin, uint16_t bMax, uint16_t bMin)
{
	uint16_t r, g, b, c;

	tcs.getRawData(&r, &g, &b, &c);
	return 
		(r >= rMin && r <= rMax) && // 8 < R < 12
		(g >= gMin && g <= gMax) && // 10 < G < 15
		(b >= bMin && b <= bMax); //  6 < B < 9
}

bool RobotColourSensor::isGreen() 
{
	return withinRange(12, 8, 15, 10, 9, 6);
}

bool RobotColourSensor::isOrange()
{
	return true;
}