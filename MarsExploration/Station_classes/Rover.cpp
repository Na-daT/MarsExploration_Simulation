#include "Rover.h"

Rover::Rover(Mission_Type m, int speed, int cUpT, int numOfmissions)
{
	RoverType = m;
	setSpeed(speed);
	RoverCheckupTime = cUpT;
	numOfmissionsBefCheckUp = numOfmissions;
	TotalNumOfMissions = 0;
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

void Rover::SetMission(missions* Mis)
{
	Mission_in_execution = Mis;
}

void Rover::incrementTotMission()
{
	TotalNumOfMissions++;
}

missions* Rover::getmissionp()
{
	return Mission_in_execution;
}

bool Rover::CompareMissNoOfRov()
{
	if (TotalNumOfMissions == numOfmissionsBefCheckUp)
		return true;
	else
		return false;
}

Mission_Type Rover::getRoverType()
{
	return RoverType;
}

int Rover::getCheckUpDuaratoin()
{
	return RoverCheckupTime;
}

int Rover::GetCheckUpStartDate()
{
	return checkUpStartDate;
}

void Rover::setCheckUpStartDate(int ST)
{
	checkUpStartDate = ST;
}
