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
	int current_missionInEx_priority;
	int ID;
	int total_distance;
	int maintenance_Start_Date;
	bool flag;
public:
	Rover(Mission_Type m, int speed, int cUpT, int numOfmissionsBefCheckUp, int id);
	void setType(Mission_Type m);
	void setSpeed(int n);
	void SetMission(missions* Mis);
	void settingTotMission();
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
	int get_TotalDistance();
	void setTotDistance(int d);
	void SetMaintenanceStartDate(int t);
	int getMaintenanceStartDate();
	void setTotalMissionsDonebeforeCheckup(int t);
	int getTotalMissionsDonebeforeCheckup();
	void flagRover();
	bool isRoverFlagged();

};

#endif