#include "MarsStation.h"
#include "../UI/UI.h"
#include <windows.h>
#include <stdlib.h>

MarsStation::MarsStation(/*UI* UIp*/)
{
	totalNumberofMissions = 0;
	totalNumberofRovers = 0;
	numofEmergMissions = 0;
	numofPolarMissions = 0;
	numofEmergRovers = 0;
	numofPolarRovers = 0;
	CurrentDay = 1;

	PUI = new UI(this);
	
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

MarsStation::~MarsStation()
{
	if (PUI)
		delete PUI;

	delete EventsQueue;
	delete WaitingEmergMissQueue;
	delete WaitingPolarMissQueue;
	delete CompletedMissQueue;
	delete AvailableEmergRovQueue;
	delete AvailablePolarQueue;
	delete InExecRoverQueue;
	delete InCheckUpPolarQueue;
	delete InCheckUpEmergQueue;
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
			
			missionP->setTimeFromToTLOC(ceil(2 * (float((float)missionP->getTarloc() / float(ARover->getRoverSpeed())) / float(25))));
			int pr =1000 / (CurrentDay + missionP->getFullTimeEx());
			InExecRoverQueue->enqueue(ARover,pr);//msh 3amlen 7esab el tarloc 
			ARover->SetMission(missionP);
			ARover->incrementTotMission();
			ARover->setCurrentMissPr(pr);
			return true;
		}
		else if (AvailablePolarQueue->dequeue(ARover))
		{
			
			missionP->setTimeFromToTLOC(ceil(2*(float((float)missionP->getTarloc() / float(ARover->getRoverSpeed())) / float(25))));
			int pr = 1000/ (CurrentDay + missionP->getFullTimeEx());
			InExecRoverQueue->enqueue(ARover,pr);
			ARover->SetMission(missionP);
			ARover->incrementTotMission();
			ARover->setCurrentMissPr(pr);
			return true;
		}
	case(2):
		if (AvailablePolarQueue->dequeue(ARover))
		{
			missionP->setTimeFromToTLOC(ceil(2 * (float((float)missionP->getTarloc() / float(ARover->getRoverSpeed())) / float(25))));
			int pr = (1000 / (CurrentDay + missionP->getFullTimeEx()));
			InExecRoverQueue->enqueue(ARover, pr);
			ARover->SetMission(missionP);
			ARover->incrementTotMission();
			ARover->setCurrentMissPr(pr);
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
	//check lw fe ay mountainous waiting ayam ktera w should be promoted

}

void MarsStation::CheckCompletedMissions()
{
	Rover* ARover;
	while (InExecRoverQueue->peekFront(ARover))
	{
		if (ARover->getmissionp()->getMissEndDay() == CurrentDay)
		{
			ARover->getmissionp()->setStatus(Completed);
			CompletedMissQueue->enqueue(ARover->getmissionp());
			ARover->SetMission(nullptr);
			UpdateRoverStatus(ARover);
		}
		else
			break;


	}
}

void MarsStation::UpdateRoverStatus(Rover* rp)
{
	InExecRoverQueue->dequeue(rp);
	if (rp->CompareMissNoOfRov())
	{
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
	Queue<missions*>* tempEmerg = new Queue<missions*>;
	for (int i = 0; i < numofPolarMissions;i++)
	{
		if (WaitingPolarMissQueue->dequeue(tempMi))
		{
			tempMi->setWaitingtime(tempMi->getWaitingtime() + 1);
			WaitingPolarMissQueue->enqueue(tempMi);
		}
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

/*void MarsStation::StartSim()
{
	CurrentDay = 0;
	UpdateCurrDay();
	Excute_events();
	AssignMissions();
}*/

void MarsStation::SaveOutputFile(ofstream& outputF)
{
	missions* M;
	outputF << "CD   " << "ID   " << "FD   " << "WD   " << "ED   " << endl;
	for (int i = 0; i < totalNumberofMissions; i++)
	{
		CompletedMissQueue->dequeue(M);
		outputF << calculateCD(M) << "    " << M->getID() << "    " << M->getFormD() << "    " << M->getWaitingtime() << "    " << M->getFullTimeEx() << endl;
		CompletedMissQueue->enqueue(M);
	}
	
	outputF << "......................................................" << endl;
	outputF << "......................................................" << endl;
	outputF << "Missions: " << totalNumberofMissions << " " << "[P: " << numofPolarMissions << " ,E: " << numofEmergMissions << "]" << endl;
	outputF << "Rovers: " << totalNumberofRovers << " " << "[P: " << numofPolarRovers << " ,E: " << numofEmergRovers << "]" << endl;
	outputF << "Avg Wait = " << CalculateAvgWaiting() << ", " << "Avg Exec = " << AvgExecTime() << endl;
}

float MarsStation::CalculateAvgWaiting()
{
	missions* M;
	int sum = 0;
	for (int i = 0; i < totalNumberofMissions; i++)
	{
		CompletedMissQueue->dequeue(M);
		sum += M->getWaitingtime();
		CompletedMissQueue->enqueue(M);
	}

	float Avg = (float) sum / (float)totalNumberofMissions;
	return Avg;
}

int MarsStation::calculateCD(missions* M)
{
	int CD = M->getFormD() + M->getWaitingtime() + M->getFullTimeEx();
	return CD;
}

float MarsStation::AvgExecTime()
{
	missions* M;
	int sum = 0;
	for (int i = 0; i < totalNumberofMissions; i++)
	{
		CompletedMissQueue->dequeue(M);
		sum += M->getFullTimeEx();
		CompletedMissQueue->enqueue(M);
	}

	float Avg = (float)sum / (float)totalNumberofMissions;
	return Avg;
}

void MarsStation::StartSim(int t)
{
	switch (t)
	{
	case(1):
		InteractiveMode();
		break;
	case(2):
		SilentMode();
		break;
	case(3):
		StepbyStepMode();
		break;
	}
}

void MarsStation::InteractiveMode()
{
	if (!PUI->LoadStation())
	{
		return;
	}

	CurrentDay = 0;

	while (!(EventsQueue->isEmpty() && WaitingEmergMissQueue->isEmpty() && WaitingPolarMissQueue->isEmpty()
		&& InExecRoverQueue->isEmpty() && InCheckUpPolarQueue->isEmpty() && InCheckUpEmergQueue->isEmpty()))
	{
		UpdateCurrDay();
		Excute_events();
		AssignMissions();

		PUI->prin_CurrentDay(CurrentDay);
		printMissionsLine();
		printInExecMiss_Rovers();
		//ba2y el print function
		Print_Rover_Line();
		Print_InCheckUp_Rovers();
		Print_CompletedMissions();

		PUI->WaitForUserClick();

	}
	PUI->SaveFile();
}

void MarsStation::SilentMode()
{
	if (!PUI->LoadStation())
	{
		return;
	}

	PUI->SilentModePrintout();
	CurrentDay = 0;

	while (!(EventsQueue->isEmpty() && WaitingEmergMissQueue->isEmpty() && WaitingPolarMissQueue->isEmpty()
		&& InExecRoverQueue->isEmpty() && InCheckUpPolarQueue->isEmpty() && InCheckUpEmergQueue->isEmpty()))
	{
		UpdateCurrDay();
		Excute_events();
		AssignMissions();
	}
	PUI->SaveFile();
	//PUI print "Simulation ends, Output file created"
}

void MarsStation::StepbyStepMode()
{
	if (!PUI->LoadStation())
	{
		return;
	}

	CurrentDay = 0;

	while (!(EventsQueue->isEmpty() && WaitingEmergMissQueue->isEmpty() && WaitingPolarMissQueue->isEmpty()
		&& InExecRoverQueue->isEmpty() && InCheckUpPolarQueue->isEmpty() && InCheckUpEmergQueue->isEmpty()))
	{
		UpdateCurrDay();
		Excute_events();
		AssignMissions();
		PUI->prin_CurrentDay(CurrentDay);
		printMissionsLine();
		printInExecMiss_Rovers();
		//ba2y el print function
		Print_Rover_Line();
		Print_InCheckUp_Rovers();
		Print_CompletedMissions();


		Sleep(1000);
	}
	PUI->SaveFile();
}

void MarsStation::printMissionsLine()
{
	missions* MI;
	int* WaitingPolarIDs = new int[numofPolarMissions];
	int* WaitingEmergencyIDs = new int[numofEmergMissions];
	int waitingEm = 0;
	int waitingPolar = 0;

	Queue <missions*>* tempEm = new Queue <missions*>;

	while (WaitingPolarMissQueue->dequeue(MI))
	{
		WaitingPolarIDs[waitingPolar] = MI->getID();
		waitingPolar++;
		tempEm->enqueue(MI);
	}

	while (tempEm->dequeue(MI))
	{
		WaitingPolarMissQueue->enqueue(MI);
	}
//fe 7aga shklha 8lt leh ba loop over the total number of polar missions when idk asln ad eh waiting
	
		while (WaitingEmergMissQueue->dequeue(MI))
		{
			WaitingEmergencyIDs[waitingEm] = MI->getID();
			tempEm->enqueue(MI);
			waitingEm++;
		}
		while (tempEm->dequeue(MI))
		{
			WaitingEmergMissQueue->enqueue(MI, MI->getPriority());
		}

	PUI->print_waitingMissions(waitingEm+waitingPolar, WaitingEmergencyIDs, waitingEm, waitingPolar,WaitingPolarIDs);
}

void MarsStation::printInExecMiss_Rovers()
{
	Queue<Rover*>* tempRovQueue = new Queue<Rover*>;
	Rover* tempRv;
	
	int EmCounter = 0;
	int PCounter = 0;
	int* EmRoverIds = new int [numofEmergRovers];
	int* PolarRovIds = new int[numofPolarRovers];
	int* EmMissionsIds = new int [numofPolarRovers];
	int* PolarMissionsIds = new int [numofPolarRovers];
	
	while (InExecRoverQueue->dequeue(tempRv))
	{
		if (tempRv->getmissionp()->getType() == Emergency)
		{
			EmRoverIds[EmCounter] = tempRv->getID();
			EmMissionsIds[EmCounter] = tempRv->getmissionp()->getID();
			EmCounter++;
		}
		else
		{
			PolarRovIds[PCounter] = tempRv->getID();
			PolarMissionsIds[PCounter] = tempRv->getmissionp()->getID();
			PCounter++;
		}
		tempRovQueue->enqueue(tempRv);
	}
	while (tempRovQueue->dequeue(tempRv))
	{
		InExecRoverQueue->enqueue(tempRv, tempRv->getCurrentMissPr());
	}

	PUI->Print_inExecMis_Rovers(EmCounter + PCounter, EmCounter, PCounter, EmMissionsIds, EmRoverIds, PolarMissionsIds, PolarRovIds);
}

void MarsStation::Print_Rover_Line()
{
	Rover* Rv;
	int totEm = 0;
	int totPolar = 0;
	int* EmergencyIDs = new int[numofEmergRovers];
	int* PolarIDs = new int[numofPolarRovers];
	Queue<Rover*>* tempR = new Queue<Rover*>;

	while (AvailableEmergRovQueue->dequeue(Rv))
	{
		EmergencyIDs[totEm] = Rv->getID();
		totEm++;
		tempR->enqueue(Rv);
	}
	while (tempR->dequeue(Rv))
	{
		AvailableEmergRovQueue->enqueue(Rv);
	}

	while (AvailablePolarQueue->dequeue(Rv))
	{
		PolarIDs[totPolar] = Rv->getID();
		totPolar++;
		tempR->enqueue(Rv);
	}
	while (tempR->dequeue(Rv))
	{
		AvailablePolarQueue->enqueue(Rv);
	}

	PUI->Print_Rover_Line(totEm+totPolar,EmergencyIDs,PolarIDs,totEm,totPolar);
}

void MarsStation::Print_InCheckUp_Rovers()
{
	Rover* Rv;
	int totEm = 0;
	int totPolar = 0;
	int* EmergencyIDs = new int[numofEmergRovers];
	int* PolarIDs = new int[numofPolarRovers];
	Queue<Rover*>* tempR = new Queue<Rover*>;

	while (InCheckUpEmergQueue->dequeue(Rv))
	{
		
		EmergencyIDs[totEm] = Rv->getID();
		totEm++;
		tempR->enqueue(Rv);
	}
	while (tempR->dequeue(Rv))
	{
		InCheckUpEmergQueue->enqueue(Rv);
	}

	while (InCheckUpPolarQueue->dequeue(Rv))
	{
		
		PolarIDs[totPolar] = Rv->getID();
		totPolar++;
		tempR->enqueue(Rv);
	}
	while (tempR->dequeue(Rv))
	{
		InCheckUpPolarQueue->enqueue(Rv);
	}

	PUI->Print_CheckUp_Rovers(totEm + totPolar, totEm, totPolar, EmergencyIDs, PolarIDs);
}

void MarsStation::Print_CompletedMissions()
{
	missions* tempMis;
	int totEm = 0;
	int totPolar = 0;
	int* EmergencyIDs = new int[numofEmergRovers];
	int* PolarIDs = new int[numofPolarRovers];
	Queue<missions*>* tempQ = new Queue<missions*>;

	while (CompletedMissQueue->dequeue(tempMis))
	{
		if (tempMis->getType() == Emergency)
		{
			
			EmergencyIDs[totEm] = tempMis->getID();
			totEm++;
			tempQ->enqueue(tempMis);
		}
		else if (tempMis->getType() == Polar)
		{
			PolarIDs[totPolar] = tempMis->getID();
			totPolar++;
			tempQ->enqueue(tempMis);

		}
	}

	while (tempQ->dequeue(tempMis))
	{
		CompletedMissQueue->enqueue(tempMis);
	}
	PUI->CompletedMissions(totEm + totPolar, totEm, totPolar, EmergencyIDs, PolarIDs);
	


}






void MarsStation::loadRovers(int EmergencyRoversCount, int PolarRoversCount, int EmergencyRoverSpeed, int PolarRoverSpeed, int NumberofMissionsBefCheckUp, int EmergencyCheckUpDuration, int PolarCheckupDuration)
{
	//creates all rovers and enqueues them according to data from input file
	

	int Id = 0;
	for (int i = 0; i < EmergencyRoversCount; i++)
	{
		Rover* R = new Rover(Emergency, EmergencyRoverSpeed, EmergencyCheckUpDuration, NumberofMissionsBefCheckUp,Id);

		AvailableEmergRovQueue->enqueue(R);
		numofEmergRovers++;
		Id++;
	}


	for (int i = 0; i < PolarRoversCount; i++)
	{
		Rover* R = new Rover(Polar, PolarRoverSpeed, PolarCheckupDuration, NumberofMissionsBefCheckUp,Id);

		AvailablePolarQueue->enqueue(R);
		numofPolarRovers++;
		Id++;
	}
	totalNumberofRovers = numofEmergRovers + numofPolarRovers;
}

void MarsStation::LoadEvents(int totnumber, int id, int day, Mission_Type MT, int target, int duration, int significance)
{
	//creates event and enqueue it 
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


