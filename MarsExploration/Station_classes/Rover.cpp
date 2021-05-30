#include "Rover.h"

Rover::Rover(Mission_Type m, int speed, int cUpT, int numOfmissionsBefCheckUp)
{
	RoverType = m;
	setSpeed(speed);
	RoverCheckupTime = cUpT;

}

void Rover::setType(Mission_Type m)
{
	RoverType = m;
}

void Rover::setSpeed(int n)
{
	if (n > 0)
	{
		RovSpeed = n;
	}
}
