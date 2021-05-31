#pragma once
#include "Event.h"
#include "../Station_classes/missions.h"
#include "../Station_classes/MarsStation.h"

class FormualtionEvent :
    public Event
{
private:
    Mission_Type Type;
    int location;
    int Duration;
    int sig;

public:
    FormualtionEvent(int id, int FD, Mission_Type type, int targ, int duration, int signi);
   	virtual void Execute(MarsStation* MarsStationP);
    
};

