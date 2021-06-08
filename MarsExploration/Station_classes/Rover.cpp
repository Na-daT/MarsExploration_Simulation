#include "Rover.h"

Rover::Rover(Mission_Type m, int speed, int cUpT, int numOfmissions, int id)
{
	RoverType = m;
	setSpeed(speed);
	RoverCheckupTime = cUpT;
	numOfmissionsBefCheckUp = numOfmissions;
	TotalNumOfMissions = 0;
	ID = id;
	total_distance = 0;
	//flag = false;
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


void Rover::settingTotMission()
{
	TotalNumOfMissions = numOfmissionsBefCheckUp;
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

int Rover::getRoverSpeed()
{
	return RovSpeed;
}

void Rover::setCurrentMissPr(int P)
{
	current_missionInEx_priority = P;

}

int Rover::getCurrentMissPr()
{
	return current_missionInEx_priority;
}

int Rover::getID()
{
	return ID;
}
/*
void Rover::setSpeedHalf()
{
	setSpeed(RovSpeed/2);
}
*/
int Rover::get_TotalDistance()
{
	return total_distance;
}

void Rover::setTotDistance(int d)
{
	total_distance = d;
}



void Rover::SetMaintenanceStartDate(int t)
{
	maintenance_Start_Date = t;
}

int Rover::getMaintenanceStartDate()
{
	return maintenance_Start_Date;
}

void Rover::setTotalMissionsDonebeforeCheckup(int t)
{
	TotalNumOfMissions = t;

}

int Rover::getTotalMissionsDonebeforeCheckup()
{
	return TotalNumOfMissions;
}

/*void Rover::flagRover()
{
	flag = true;
}

bool Rover::isRoverFlagged()
{
	return flag;
}
*/