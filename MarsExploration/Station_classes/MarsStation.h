#pragma once
#include "../DataStructures/Queue.h"
#include"../DataStructures/PriorityQueue.h"
#include"../Events/Event.h"
#include"missions.h"
#include"Rover.h"

class MarsStation
{
private:
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


};

