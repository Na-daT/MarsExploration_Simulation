#pragma once
#include "../Defs.h"
class missions
{
	int ID;
	int FormDay;
	int Tarloc;
	int MissDur;
	int MissSign;
	int waiting_time;
	Mission_Type MissionType;
	Mission_Status Status;
public:
	missions(int id, int FD, int targ_loc, int duration,int sig);
	void setID(int id);
	Mission_Type getType();
	int getMissSign();
	int getTarloc();
	int getMissDur();
	int getFormD();

};

