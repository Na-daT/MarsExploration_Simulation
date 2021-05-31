#pragma once
#ifndef _Event_H
#define _Event_H


#include "../Defs.h"
//#include"../Station_classes/MarsStation.h"

class MarsStation;

class Event
{
protected:
	int ID;
	int Event_day;

public:
	Event(int id, int Etime);
	virtual void Execute(MarsStation* MarsStationP)=0;
	int getEvent_day();
};

#endif