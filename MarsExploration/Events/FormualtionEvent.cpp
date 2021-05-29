#include "FormualtionEvent.h"
#include "../Station_classes/missions.h"

FormualtionEvent::FormualtionEvent(int id, int FD, Mission_Type type, int targ, int duration, int signi):
	Event(id,FD),Type(type),location(targ),Duration(duration),sig(signi)
{
}

void FormualtionEvent::Execute(MarsStation* MarsStationP)
{
	missions* NeMission = new missions(ID, Event_day,location,Duration,sig/*add all data taken by the event*/);

	if (Type == Polar)
	{
		MarsStationP->addtoQueue(NeMission); //MArs station pointer to be able to execute the function addtoQueue to be added
	}
	else if (Type == Emergency)
	{
		MarsStationP->addtoQueue(NeMission);

	}

}
