#pragma once
#ifndef _Formulation_event_
#define _Formulation_event_

#include <iostream>
using namespace std;

#include "../Station_classes/missions.h"
#include "../Station_classes/MarsStation.h"
#include "Event.h"

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

#endif