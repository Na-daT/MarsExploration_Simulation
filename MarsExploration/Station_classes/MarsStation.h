#pragma once
#ifndef _MarsStation_H
#define _MarsStation_H

#include "../DataStructures/Queue.h"
#include"../DataStructures/PriorityQueue.h"
#include"../Events/Event.h"
#include"missions.h"
#include"Rover.h"
#include"../Events/FormualtionEvent.h"
//#include "../UI/UI.h"


class MarsStation
{
private:

	int totNumEvents;
	int totalNumberofMissions;
	int totalNumberofRovers;
	int numofEmergMissions;
	int numofPolarMissions;
	int numofEmergRovers;
	int numofPolarRovers;
	int CurrentDay;

	Queue<Event *> *EventsQueue;
	PriorityQueue<missions*>* WaitingEmergMissQueue;
	Queue<missions*>* WaitingPolarMissQueue;
	Queue<missions*>* CompletedMissQueue;
	Queue<Rover*>* AvailableEmergRovQueue;
	Queue<Rover*>* AvailablePolarQueue;
	PriorityQueue<Rover*>* InExecRoverQueue;
	Queue<Rover*>* InCheckUpPolarQueue;
	Queue<Rover*>* InCheckUpEmergQueue;

public:
	MarsStation();

	void addtoQueue(missions* missP); //Mars station pointer to be able to execute the function addtoQueue to be added

	void loadRovers(int EmergencyRoversCount, int PolarRoversCount, int EmergencyRoverSpeed, int PolarRoverSpeed, int NumberofMissionsBefCheckUp,int EmergencyCheckUpDuration, int PolarCheckupDuration);

	void LoadEvents(int totnumber, int id, int day, Mission_Type MT, int target, int duration, int significance);//to enqueue event that will be read from the UI 

	int GetPriority(missions* missionP);

	//void IncrementTime();//function to increment day + increase waiting time of available missions
	void Excute_events();

	void AssignMissions();
	bool GetAvailableRover(missions* missionP);
	void UpdateCurrDay();
	void CheckCompletedMissions();
	void UpdateRoverStatus(Rover* rp);
	void CheckUpduartionEnd();
	void updateWaitingTime();

	/*void InteractiveMode();
	void SilentMode();
	void StepbyStepMode();*/

};

#endif
