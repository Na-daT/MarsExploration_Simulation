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
	int current_missionEx_pr;
	int ID;
public:
	Rover(Mission_Type m, int speed, int cUpT, int numOfmissionsBefCheckUp, int id);
	void setType(Mission_Type m);
	void setSpeed(int n);
	void SetMission(missions* Mis);
	void incrementTotMission(); //instead of setting etc
	missions* getmissionp();
	bool CompareMissNoOfRov(); //logic could have been in mars station
	Mission_Type getRoverType();
	int getCheckUpDuaratoin();
	int GetCheckUpStartDate();
	void setCheckUpStartDate(int ST);
	int getRoverSpeed();
	void setCurrentMissPr(int P);
	int getCurrentMissPr();
	int getID();

};

#endif