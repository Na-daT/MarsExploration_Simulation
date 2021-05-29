#pragma once
#include "../Defs.h"
class Event
{
protected:
	int ID;
	int Event_day;

public:
	Event(int id, int Etime);
	virtual void Execute()=0;

};

