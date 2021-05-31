#include "MarsStation.h"

MarsStation::MarsStation()
{
	totalNumberofMissions = 0;
	totalNumberofRovers = 0;
	numofEmergMissions = 0;
	numofPolarMissions = 0;
	numofEmergRovers = 0;
	numofPolarRovers = 0;
	CurrentDay = 1;

	EventsQueue = new Queue<Event*>;
	WaitingEmergMissQueue = new PriorityQueue<missions*>;
	WaitingPolarMissQueue = new Queue<missions*>;
	CompletedMissQueue = new Queue<missions*>;
	AvailableEmergRovQueue = new Queue<Rover*>;
	AvailablePolarQueue = new Queue<Rover*>;
	InExecRoverQueue = new PriorityQueue<Rover*>;
	InCheckUpPolarQueue = new Queue<Rover*>;
	InCheckUpEmergQueue = new Queue<Rover*>;
}

void MarsStation::addtoQueue(missions* missP)
{
	if (missP->getType() == Polar)
	{
		WaitingPolarMissQueue->enqueue(missP);
	}
	else
	{
		int pr = GetPriority(missP);
		WaitingEmergMissQueue->enqueue(missP, pr);
	}
}
int MarsStation::GetPriority(missions* missionP)
{
	int prio = (missionP->getMissSign()) + 10000 * (missionP->getMissDur()) / (missionP->getFormD() * missionP->getTarloc());
	return prio;
}
void MarsStation::Excute_events()
{
	Event* ev;
	while (EventsQueue->peek(ev))
	{
		if (ev->getEvent_day() > CurrentDay)
		{
			return;
		}
			ev->Execute(this);
			EventsQueue->dequeue(ev);
			delete ev;
	}
}

void MarsStation::AssignMissions()
{
	missions* currentMiss;
	if (WaitingEmergMissQueue->peekFront(currentMiss))
	{
		if (GetAvailableRover(currentMiss))
		{
			WaitingEmergMissQueue->dequeue(currentMiss);
			currentMiss->setStatus(in_Execution);
			currentMiss->setEndDate(CurrentDay);
		}
	}

	if (WaitingPolarMissQueue->peek(currentMiss))
	{
		if (GetAvailableRover(currentMiss))
		{
			WaitingPolarMissQueue->dequeue(currentMiss);
			currentMiss->setStatus(in_Execution);
			currentMiss->setEndDate(CurrentDay);
		}
	}
}

bool MarsStation::GetAvailableRover(missions* missionP)
{
	Mission_Type Mtype = missionP->getType();
	Rover* ARover;


	switch ((int)Mtype)
	{
	case(1):
		if (AvailableEmergRovQueue->dequeue(ARover))
		{
			InExecRoverQueue->enqueue(ARover,(CurrentDay + missionP->getMissDur()));
			ARover->SetMission(missionP);
			return true;
		}
		else if (AvailablePolarQueue->dequeue(ARover))
		{
			InExecRoverQueue->enqueue(ARover, (CurrentDay + missionP->getMissDur()));
			ARover->SetMission(missionP);
			ARover->incrementTotMission();
			return true;
		}
	case(2):
		if (AvailablePolarQueue->dequeue(ARover))
		{
			InExecRoverQueue->enqueue(ARover, (CurrentDay + missionP->getMissDur()));
			ARover->SetMission(missionP);
			ARover->incrementTotMission();
		}
	}
	return false;
}

void MarsStation::loadRovers(int EmergencyRoversCount, int PolarRoversCount, int EmergencyRoverSpeed, int PolarRoverSpeed, int NumberofMissionsBefCheckUp, int EmergencyCheckUpDuration, int PolarCheckupDuration)
{

	for (int i = 0; i < EmergencyRoversCount; i++)
	{
		Rover* R = new Rover(Emergency, EmergencyRoverSpeed, EmergencyCheckUpDuration, NumberofMissionsBefCheckUp);

		AvailableEmergRovQueue->enqueue(R);
		numofEmergRovers++;
	}


	for (int i = 0; i < PolarRoversCount; i++)
	{
		Rover* R = new Rover(Polar, PolarRoverSpeed, PolarCheckupDuration, NumberofMissionsBefCheckUp);

		AvailablePolarQueue->enqueue(R);
		numofPolarRovers++;
	}
	totalNumberofRovers = numofEmergRovers + numofPolarRovers;
}

void MarsStation::LoadEvents(int totnumber, int id, int day, Mission_Type MT, int target, int duration, int significance)
{
	Event* EventP = new FormualtionEvent(id, day, MT,target,duration, significance);
	EventsQueue->enqueue(EventP);

	if (MT == Polar)
	{
		numofPolarMissions++;
		totalNumberofMissions++;
	}
	else
	{
		numofEmergMissions++;
		totalNumberofMissions++;
	}

	totNumEvents = totnumber;
}


