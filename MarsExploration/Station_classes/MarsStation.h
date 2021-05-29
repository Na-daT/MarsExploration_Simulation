#pragma once
#include "../DataStructures/Queue.h"
#include"../DataStructures/PriorityQueue.h"
#include"../Events/Event.h"
#include"missions.h"
#include"Rover.h"

class MarsStation
{
private:


	int totalNumberofMissions;
	int totalNumberofRovers;
	int numofEmergMissions;
	int numofPolarMissions;
	int numofEmergRovers;
	int numofPolarRovers;


	Queue<Event*>* EventsQueue;
	PriorityQueue<missions*>* WaitingEmergMissQueue;
	Queue<missions*>* WaitingPolarMissQueue;
	Queue<missions*>* CompletedMissQueue;
	Queue<Rover*>* AvailableEmergRovQueue;
	Queue<Rover*>* AvailablePolarQueue;
	PriorityQueue<Rover*>* InExecRoverQueue;
	Queue<Rover*>* InCheckUpPolarQueue;
	Queue<Rover*>* InCheckUpEmergQueue;

public:
	void addtoQueue(missions*); //MArs station pointer to be able to execute the function addtoQueue to be added

	void loadRovers(int EmergencyRoversCount, int PolarRoversCount, int EmergencyRoverSpeed, int PolarRoverSpeed, int NumberofMissionsBefCheckUp,int EmergencyCheckUpDuration, int PolarCheckupDuration);

	void LoadEvents();//to enqueue event that will be read from the UI 
};

