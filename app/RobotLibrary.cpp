#include "RobotLibrary.h"

void RobotDriverBase::setup() 
{
    md.init();
}

void RobotDriverBase::differentialSteer(double speed, double rotation) 
{
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