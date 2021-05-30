#pragma once
#include "../Defs.h"
class Rover
{
	Mission_Type RoverType;
	int RovSpeed;
	int RoverCheckupTime;
	int numOfmissionsBefCheckUp;
	int TotalNumOfMissions;
public:
	Rover(Mission_Type m, int speed, int cUpT, int numOfmissionsBefCheckUp);
	void setType(Mission_Type m);
	void setSpeed(int n);

};

