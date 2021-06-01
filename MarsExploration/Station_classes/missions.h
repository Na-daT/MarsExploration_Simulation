#pragma once
#ifndef _Missions_
#define _Missions_

#include "../Defs.h"
class missions
{
	int ID;
	int FormDay;
	int Tarloc;
	int MissDur;
	int timeForRoverFromAndToTLOC;
	int MissSign;
	int waiting_time;
	int ExecutionEndDay;
	Mission_Type MissionType;
	Mission_Status Status;
	int priority;
public:
	missions(int id, int FD, int targ_loc, int duration,int sig, Mission_Type MT);
	//void setID(int id);
	int getID();
	Mission_Type getType();
	int getMissSign();
	int getTarloc();
	int getMissDur();
	int getFormD();
	void setStatus(Mission_Status St);
	void setEndDate(int d);
	int getMissEndDay();
	void setPriority(int p);
	int getWaitingtime();
	void setWaitingtime(int t);
	int getPriority();
	void setTimeFromToTLOC(int n);
	int getTimeFromToTLOC();



};
#endif
