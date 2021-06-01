#pragma once
#ifndef _Rover_
#define _Rover_

#include "../Defs.h"
#include "missions.h"

class Rover
{
	Mission_Type RoverType;
	int RovSpeed;
	int RoverCheckupTime;
	int numOfmissionsBefCheckUp;
	int TotalNumOfMissions;
	missions* Mission_in_execution;
	int checkUpStartDate;
public:
	Rover(Mission_Type m, int speed, int cUpT, int numOfmissionsBefCheckUp);
	void setType(Mission_Type m);
	void setSpeed(int n);
	void SetMission(missions* Mis);
	void incrementTotMission();
	missions* getmissionp();
	bool CompareMissNoOfRov();
	Mission_Type getRoverType();
	int getCheckUpDuaratoin();
	int GetCheckUpStartDate();
	void setCheckUpStartDate(int ST);
};

#endif