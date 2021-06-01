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
		missP->setPriority(pr);
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
	while (WaitingEmergMissQueue->peekFront(currentMiss))
	{
		if (GetAvailableRover(currentMiss))
		{
			WaitingEmergMissQueue->dequeue(currentMiss);
			currentMiss->setStatus(in_Execution);
			currentMiss->setEndDate(CurrentDay);
		}
		else
			break;
	}

	while (WaitingPolarMissQueue->peek(currentMiss))
	{
		if (GetAvailableRover(currentMiss))
		{
			WaitingPolarMissQueue->dequeue(currentMiss);
			currentMiss->setStatus(in_Execution);
			currentMiss->setEndDate(CurrentDay);
		}
		else
			break;
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
			InExecRoverQueue->enqueue(ARover,1000*(1/(CurrentDay + missionP->getMissDur())));
			ARover->SetMission(missionP);
			ARover->incrementTotMission();
			return true;
		}
		else if (AvailablePolarQueue->dequeue(ARover))
		{
			InExecRoverQueue->enqueue(ARover, 1000 * (1 / (CurrentDay + missionP->getMissDur())));
			ARover->SetMission(missionP);
			ARover->incrementTotMission();
			return true;
		}
	case(2):
		if (AvailablePolarQueue->dequeue(ARover))
		{
			InExecRoverQueue->enqueue(ARover, 1000 * (1 / (CurrentDay + missionP->getMissDur())));
			ARover->SetMission(missionP);
			ARover->incrementTotMission();
			return true;
		
		}
	}
	return false;
}

void MarsStation::UpdateCurrDay()
{

	CurrentDay++;
	CheckCompletedMissions();
	//Check check up duration of rovers
	CheckUpduartionEnd();
	//update waiting time of missions in queues
	updateWaitingTime();

}

void MarsStation::CheckCompletedMissions()
{
	Rover* ARover;
	while (InExecRoverQueue->peekFront(ARover))
	{
		if (ARover->getmissionp()->getMissEndDay()==CurrentDay)
		{
			ARover->getmissionp()->setStatus(Completed);
			CompletedMissQueue->enqueue(ARover->getmissionp());
			ARover->SetMission(nullptr);
			UpdateRoverStatus(ARover);
		}


	}




}

void MarsStation::UpdateRoverStatus(Rover* rp)
{
	if (rp->CompareMissNoOfRov())
	{
		InExecRoverQueue->dequeue(rp);
		if (rp->getRoverType() == Emergency)
		{
			InCheckUpEmergQueue->enqueue(rp);
			rp->setCheckUpStartDate(CurrentDay);
		}
		else
		{
			InCheckUpPolarQueue->enqueue(rp);
			rp->setCheckUpStartDate(CurrentDay);
		}
	}
	else
	{
		if (rp->getRoverType() == Emergency)
			AvailableEmergRovQueue->enqueue(rp);
		else
			AvailablePolarQueue->enqueue(rp);

	}
}

void MarsStation::CheckUpduartionEnd()
{
	Rover* Rv;
	while (InCheckUpPolarQueue->peek(Rv) && (Rv->getCheckUpDuaratoin() + Rv->GetCheckUpStartDate() == CurrentDay))
	{
			InCheckUpPolarQueue->dequeue(Rv);
			AvailablePolarQueue->enqueue(Rv);
	}
	while (InCheckUpEmergQueue->peek(Rv) && (Rv->getCheckUpDuaratoin() + Rv->GetCheckUpStartDate() == CurrentDay))
	{
		InCheckUpEmergQueue->dequeue(Rv);
		AvailableEmergRovQueue->enqueue(Rv);
	}
}

void MarsStation::updateWaitingTime()
{
	missions* tempMi;
	//eue <missions*>* tempPolar=new Queue<missions*>;
	Queue<missions*>* tempEmerg = new Queue<missions*>;
	for (int i = 0; i < numofPolarMissions;i++)
	{
		WaitingPolarMissQueue->dequeue(tempMi);
		tempMi->setWaitingtime(tempMi->getWaitingtime() + 1);
		WaitingPolarMissQueue->enqueue(tempMi);
	}
	while (WaitingEmergMissQueue->dequeue(tempMi))
	{
		tempMi->setWaitingtime(tempMi->getWaitingtime() + 1);
		tempEmerg->enqueue(tempMi);
	}
	while (tempEmerg->dequeue(tempMi))
	{
		WaitingEmergMissQueue->enqueue(tempMi, tempMi->getPriority());
	}
}

void MarsStation::StartSim()
{
	CurrentDay = 0;
	UpdateCurrDay();
	Excute_events();
	AssignMissions();
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


