#pragma once
#include "../Defs.h"
class missions
{
	int ID;
	int FormDay;
	int Tarloc;
	int MissDur;
	int MissSign;
	Mission_Type MissionType;
	Mission_Status Status;
public:
	missions(int id, int FD, int targ_loc, int duration,int sig);

};

