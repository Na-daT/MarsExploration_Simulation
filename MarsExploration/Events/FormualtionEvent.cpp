#include "FormualtionEvent.h"
#include "../Station_classes/missions.h"

FormualtionEvent::FormualtionEvent(int id, int FD, Mission_Type type, int targ, int duration, int signi):
	Event(id,FD),Type(type),location(targ),Duration(duration),sig(signi)
{
}

void FormualtionEvent::Execute(MarsStation* MarsStationP)
{
	//creates new mission
	missions* NeMission = new missions(ID, Event_day,location,Duration,sig,Type);

	//adds to appropriate list
	MarsStationP->addtoQueue(NeMission); 

}
