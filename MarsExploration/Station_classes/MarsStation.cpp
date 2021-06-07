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
	AvailableEmergRovQueue = new PriorityQueue<Rover*>;
	AvailablePolarQueue = new PriorityQueue<Rover*>;
	InExecRoverQueue = new PriorityQueue<Rover*>;
	InCheckUpPolarQueue = new Queue<Rover*>;
	InCheckUpEmergQueue = new Queue<Rover*>;
	MaintenancePolarQueue = new Queue<Rover*>;
	MaintenanceEmergQueue = new Queue<Rover*>;
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
	//this priority equation balances the significance of a mission and its how far it is and when it was formulated
	//if two missions have the same target location but one is formulated after the other then the one formulated earlier has higherr priority
	//same for Duration, if the only difference is mission duration then the longer the mission the higher priority it has
	int prio = (missionP->getMissSign()) + 10000 * (missionP->getMissDur()) / (missionP->getFormD() * missionP->getTarloc());
	return prio;
}


void MarsStation::Excute_events()
{
	Event* ev;
	while (EventsQueue->peek(ev))
	{
		//excute events on daily basis
		if (ev->getEvent_day() > CurrentDay)
		{
			return;
		}
		ev->Execute(this);
		EventsQueue->dequeue(ev);

		//de allocate the event after excution
		delete ev;
	}
}

void MarsStation::AssignMissions()
{
	missions* currentMiss;
	while (WaitingEmergMissQueue->peekFront(currentMiss))
	{
		//if no available Rovers the mission is not assigned and waits for next day
		if (GetAvailableRover(currentMiss))
		{
			WaitingEmergMissQueue->dequeue(currentMiss);
			currentMiss->setStatus(in_Execution);

			//this function takes the current day as a parameter and adds it to total time of excution to gett End date of mission
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

	//each mission type has different priority for each rover type
	switch ((int)Mtype)
	{
	case(1):
		if (AvailableEmergRovQueue->dequeue(ARover))
		{
			AssignAvailableRover(missionP, ARover);
			return true;
		}
		else if (AvailablePolarQueue->dequeue(ARover))
		{

			AssignAvailableRover(missionP, ARover);
			return true;
		}
		else if (MaintenanceEmergQueue->dequeue(ARover))
		{
			AssignInMaintenanceRover(missionP, ARover);

			return true;
		}
		else if (MaintenancePolarQueue->dequeue(ARover))
		{
			AssignInMaintenanceRover(missionP, ARover);
			return true;
		}
	case(2):
		if (AvailablePolarQueue->dequeue(ARover))
		{
			AssignAvailableRover(missionP, ARover);
			return true;

		}
		if (MaintenancePolarQueue->dequeue(ARover))
		{
			AssignInMaintenanceRover(missionP, ARover);
			return true;
		}
	}
	return false;
}

//to avoid repeating logic in above funcion
void MarsStation::AssignAvailableRover(missions* Mission, Rover* RovertobeAssigned)
{
	Mission->setTimeFromToTLOC(ceil(2 * (float((float)Mission->getTarloc() / float(RovertobeAssigned->getRoverSpeed())) / float(25))));

	//enqueue rovers according to time they take to finish mission
	//rovers that finish first has higher priority thus are always on top of queue
	int pr = 1000 / (CurrentDay + Mission->getFullTimeEx());
	InExecRoverQueue->enqueue(RovertobeAssigned, pr);


	RovertobeAssigned->SetMission(Mission);
	RovertobeAssigned->setTotalMissionsDonebeforeCheckup(RovertobeAssigned->getTotalMissionsDonebeforeCheckup() + 1);
	RovertobeAssigned->setCurrentMissPr(pr);

	//need to increment total distance moved by rover to check if it need maintenance
	RovertobeAssigned->setTotDistance(RovertobeAssigned->get_TotalDistance() + Mission->getTarloc());
}

void MarsStation::AssignInMaintenanceRover(missions* Mission, Rover* RovertobeAssigned)
{
	//same logic as above function 
	//only diference is now rover has half its speed for rest of simulation

	RovertobeAssigned->setSpeed(ceil(RovertobeAssigned->getRoverSpeed() / 2));

	AssignAvailableRover(Mission, RovertobeAssigned);
}

void MarsStation::UpdateCurrDay()
{

	CurrentDay++;
	CheckCompletedMissions();
	//Check check up and maintenance duration of rovers
	CheckUpduartionEnd();
	//update waiting time of missions still waiting in queues
	updateWaitingTime();

}

void MarsStation::CheckCompletedMissions()
{
	Rover* ARover;
	while (InExecRoverQueue->peekFront(ARover))
	{
		//if the first rover in excution still didnt finish then according the priority no other rover has thus break out of the while loop

		if (ARover->getmissionp()->getMissEndDay() == CurrentDay)
		{
			//random probability
			if (rand() % 10 < 1 /*ARover->getRoverSpeed()*ARover->getID() / ARover->getmissionp()->getTarloc()*/)
			{
				//reallocating mission with same attributes except formualtion day
				missions* currentMiss = new missions(ARover->getmissionp()->getID(), CurrentDay, ARover->getmissionp()->getTarloc(), ARover->getmissionp()->getMissDur(), ARover->getmissionp()->getMissSign(), ARover->getmissionp()->getType());
				
				//sending mission back to waiting missions queue
				addtoQueue(currentMiss);

				//condition Rover to go into check up immediatly
				ARover->settingTotMission();
				InExecRoverQueue->dequeue(ARover);
				UpdateRoverStatus(ARover);

				//deallocate original failed mission
				delete ARover->getmissionp();

			}
			//default logic when mission doesnt fail
			else
			{

				ARover->getmissionp()->setStatus(Completed);
				CompletedMissQueue->enqueue(ARover->getmissionp());
				InExecRoverQueue->dequeue(ARover);
				UpdateRoverStatus(ARover);
			}
		}
		else
			break;
	}
}

void MarsStation::UpdateRoverStatus(Rover* rp)
{
	//get total distance done by rover so far (condition of maintenance)
	int Tarloc = rp->get_TotalDistance();

	//mission is already moved to in excution queue
	rp->SetMission(nullptr);

	//first compare the total missions done by rover so far to the number of missions before check up from input file
	//condition of check up
	//if a rover needs both check up and maintenance, check up is done first as maintenance can be detrminated without finishing unllike check up
	if (rp->CompareMissNoOfRov())
	{
		//reset the total missions done by rover so far 
		rp->setTotalMissionsDonebeforeCheckup(0);

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
	//rovers undergo maintenance if they move more than 500km total
	else if (Tarloc >= 500)
	{
		if (rp->getRoverType() == Emergency)
		{
			MaintenanceEmergQueue->enqueue(rp);
			rp->SetMaintenanceStartDate(CurrentDay);
		}
		else
		{
			MaintenancePolarQueue->enqueue(rp);
			rp->SetMaintenanceStartDate(CurrentDay);
		}
	}
	//if rover doesnt need neither check up nor maintenace it is send back to the appropriate availble queue
	else
	{
		if (rp->getRoverType() == Emergency)
			AvailableEmergRovQueue->enqueue(rp, rp->getRoverSpeed());
		else
			AvailablePolarQueue->enqueue(rp, rp->getRoverSpeed());

	}
}

//check up and maintenance duration
void MarsStation::CheckUpduartionEnd()
{
	Rover* Rv;
	while (InCheckUpPolarQueue->peek(Rv) && (Rv->getCheckUpDuaratoin() + Rv->GetCheckUpStartDate() == CurrentDay))
	{
		InCheckUpPolarQueue->dequeue(Rv);
		//we still need to check if the rover needed maintenance
		UpdateRoverStatus(Rv);
	}
	while (InCheckUpEmergQueue->peek(Rv) && (Rv->getCheckUpDuaratoin() + Rv->GetCheckUpStartDate() == CurrentDay))
	{
		InCheckUpEmergQueue->dequeue(Rv);
		UpdateRoverStatus(Rv);
	}
	//maintenance duration is 3 days regardelss of rover type
	while (MaintenancePolarQueue->peek(Rv) && (Rv->getMaintenanceStartDate() + 3 == CurrentDay))
	{
		//reset total distance moved by rover
		Rv->setTotDistance(0);
		MaintenancePolarQueue->dequeue(Rv);
		AvailablePolarQueue->enqueue(Rv, Rv->getRoverSpeed());
	}
	while (MaintenanceEmergQueue->peek(Rv) && (Rv->getMaintenanceStartDate() + 3 == CurrentDay))
	{
		Rv->setTotDistance(0);
		MaintenanceEmergQueue->dequeue(Rv);
		AvailableEmergRovQueue->enqueue(Rv, Rv->getRoverSpeed());
	}
}

void MarsStation::updateWaitingTime()
{
	missions* tempMi;
	Queue<missions*>* tempQ = new Queue<missions*>;

	//increment number of waiting days then enqueue missions again
	while (WaitingPolarMissQueue->dequeue(tempMi))
	{
		tempMi->setWaitingtime(tempMi->getWaitingtime() + 1);
		tempQ->enqueue(tempMi);
	}
	while (tempQ->dequeue(tempMi))
	{
		WaitingPolarMissQueue->enqueue(tempMi);
	}


	while (WaitingEmergMissQueue->dequeue(tempMi))
	{
		tempMi->setWaitingtime(tempMi->getWaitingtime() + 1);
		tempQ->enqueue(tempMi);
	}
	while (tempQ->dequeue(tempMi))
	{
		WaitingEmergMissQueue->enqueue(tempMi, tempMi->getPriority());
	}
}

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

	float Avg = (float)sum / (float)totalNumberofMissions;
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

//program only terminates if there are no more events, missions in execution, or rovers in check up or maintenance
void MarsStation::InteractiveMode()
{
	if (!PUI->LoadStation())
	{
		return;
	}

	CurrentDay = 0;

	while (!(EventsQueue->isEmpty() && WaitingEmergMissQueue->isEmpty() && WaitingPolarMissQueue->isEmpty()
		&& InExecRoverQueue->isEmpty() && InCheckUpPolarQueue->isEmpty() && InCheckUpEmergQueue->isEmpty() 
		&& MaintenancePolarQueue->isEmpty() && MaintenanceEmergQueue->isEmpty()))
	{
		UpdateCurrDay();
		Excute_events();
		AssignMissions();

		//print functions
		PUI->prin_CurrentDay(CurrentDay);
		printMissionsLine();
		printInExecMiss_Rovers();
		Print_Rover_Line();
		Print_InCheckUp_Rovers();
		Print_Maintenance_Rovers();
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
		&& InExecRoverQueue->isEmpty() && InCheckUpPolarQueue->isEmpty() && InCheckUpEmergQueue->isEmpty()
		&& MaintenancePolarQueue->isEmpty() && MaintenanceEmergQueue->isEmpty()))
	{
		UpdateCurrDay();
		Excute_events();
		AssignMissions();
	}
	PUI->SaveFile();
}

void MarsStation::StepbyStepMode()
{
	if (!PUI->LoadStation())
	{
		return;
	}

	CurrentDay = 0;

	while (!(EventsQueue->isEmpty() && WaitingEmergMissQueue->isEmpty() && WaitingPolarMissQueue->isEmpty()
		&& InExecRoverQueue->isEmpty() && InCheckUpPolarQueue->isEmpty() && InCheckUpEmergQueue->isEmpty()
		&& MaintenancePolarQueue->isEmpty() && MaintenanceEmergQueue->isEmpty()))
	{
		UpdateCurrDay();
		Excute_events();
		AssignMissions();

		//print functions
		PUI->prin_CurrentDay(CurrentDay);
		printMissionsLine();
		printInExecMiss_Rovers();
		Print_Rover_Line();
		Print_InCheckUp_Rovers();
		Print_Maintenance_Rovers();
		Print_CompletedMissions();


		Sleep(1000);
	}
	PUI->SaveFile();
}

void MarsStation::printMissionsLine()
{
	missions* MI;
	//get ids of all mission waiting in current day
	//and their total number (changes every day)
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

	//send data to UI to Print on console
	PUI->print_waitingMissions(waitingEm + waitingPolar, WaitingEmergencyIDs, waitingEm, waitingPolar, WaitingPolarIDs);
}

void MarsStation::printInExecMiss_Rovers()
{
	Queue<Rover*>* tempRovQueue = new Queue<Rover*>;
	Rover* tempRv;

	int EmCounter = 0;
	int PCounter = 0;
	int* EmRoverIds = new int[numofEmergRovers];
	int* PolarRovIds = new int[numofPolarRovers];
	int* EmMissionsIds = new int[numofPolarRovers];
	int* PolarMissionsIds = new int[numofPolarRovers];

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
		AvailableEmergRovQueue->enqueue(Rv, Rv->getRoverSpeed());
	}

	while (AvailablePolarQueue->dequeue(Rv))
	{
		PolarIDs[totPolar] = Rv->getID();
		totPolar++;
		tempR->enqueue(Rv);
	}
	while (tempR->dequeue(Rv))
	{
		AvailablePolarQueue->enqueue(Rv, Rv->getRoverSpeed());
	}

	PUI->Print_Rover_Line(totEm + totPolar, EmergencyIDs, PolarIDs, totEm, totPolar);
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

void MarsStation::Print_Maintenance_Rovers()
{
	Rover* Rv;
	int totEm = 0;
	int totPolar = 0;
	int* EmergencyIDs = new int[numofEmergRovers];
	int* PolarIDs = new int[numofPolarRovers];
	Queue<Rover*>* tempR = new Queue<Rover*>;

	while (MaintenanceEmergQueue->dequeue(Rv))
	{

		EmergencyIDs[totEm] = Rv->getID();
		totEm++;
		tempR->enqueue(Rv);
	}
	while (tempR->dequeue(Rv))
	{
		MaintenanceEmergQueue->enqueue(Rv);
	}

	while (MaintenancePolarQueue->dequeue(Rv))
	{

		PolarIDs[totPolar] = Rv->getID();
		totPolar++;
		tempR->enqueue(Rv);
	}
	while (tempR->dequeue(Rv))
	{
		MaintenancePolarQueue->enqueue(Rv);
	}

	PUI->Print_Maintenance_Rovers(totEm + totPolar, totEm, totPolar, EmergencyIDs, PolarIDs);
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

void MarsStation::loadRovers(int EmergencyRoversCount, int PolarRoversCount, int* EmergencyRoverSpeed, int* PolarRoverSpeed, int NumberofMissionsBefCheckUp, int EmergencyCheckUpDuration, int PolarCheckupDuration)
{
	//creates all rover objects and enqueues them according to data from input file

	//ID generator, incremented every time a rover is created to esnure it is unique
	int Id = 1;
	for (int i = 0; i < EmergencyRoversCount; i++)
	{
		Rover* R = new Rover(Emergency, EmergencyRoverSpeed[i], EmergencyCheckUpDuration, NumberofMissionsBefCheckUp, Id);

		AvailableEmergRovQueue->enqueue(R, R->getRoverSpeed());
		numofEmergRovers++;
		Id++;
	}


	for (int i = 0; i < PolarRoversCount; i++)
	{
		Rover* R = new Rover(Polar, PolarRoverSpeed[i], PolarCheckupDuration, NumberofMissionsBefCheckUp, Id);

		AvailablePolarQueue->enqueue(R, R->getRoverSpeed());
		numofPolarRovers++;
		Id++;
	}
	totalNumberofRovers = numofEmergRovers + numofPolarRovers;
}

void MarsStation::LoadEvents(int totnumber, int id, int day, Mission_Type MT, int target, int duration, int significance)
{
	//creates event and enqueue it 
	Event* EventP = new FormualtionEvent(id, day, MT, target, duration, significance);
	EventsQueue->enqueue(EventP);

	if (MT == Polar)
	{
		numofPolarMissions++;

	}
	else
	{
		numofEmergMissions++;
	}

	totalNumberofMissions++;
	totNumEvents = totnumber;
}


