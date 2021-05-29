#include "MarsStation.h"

MarsStation::MarsStation()
{
	totalNumberofMissions = 0;
	totalNumberofRovers = 0;
	numofEmergMissions = 0;
	numofPolarMissions = 0;
	numofEmergRovers = 0;
	numofPolarRovers = 0;

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
