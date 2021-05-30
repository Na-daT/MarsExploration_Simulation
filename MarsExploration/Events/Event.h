#pragma once
#include "../Defs.h"
#include"../Station_classes/MarsStation.h"
class Event
{
protected:
	int ID;
	int Event_day;

public:
	Event(int id, int Etime);
	virtual void Execute(MarsStation* MarsStationP)=0;

};

