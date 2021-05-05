#include "RobotLibrary.h"

void RobotDriver::init() 
{
    md.init();
}

void RobotDriver::differentialSteer(double speed, double rotation) 
{
  // positive rotation is clockwise
  // negation of md.setM2Speed since motors were installed incorrectly
	if (rotation<=0)
	{
		md.setM1Speed(-(400*speed)); 
		md.setM2Speed(400*(speed-2*speed*fabs(rotation)));
	}
	if (rotation>=0)
	{
		md.setM1Speed(-(400*(speed-2*speed*fabs(rotation))));
		md.setM2Speed(400*speed);
	}
  stopIfFault();
}

void RobotDriver::stopIfFault()
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

DualVNH5019MotorShield& RobotDriver::get_md()
{
  return md;
}
