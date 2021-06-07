#pragma once
#ifndef _MarsStation_H
#define _MarsStation_H

#include <fstream>
#include "../DataStructures/Queue.h"
#include"../DataStructures/PriorityQueue.h"
#include"../Events/Event.h"
#include"missions.h"
#include"Rover.h"
#include"../Events/FormualtionEvent.h"


class UI; //forward declaration

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

	UI* PUI;

	Queue<Event*>* EventsQueue;
	PriorityQueue<missions*>* WaitingEmergMissQueue;
	Queue<missions*>* WaitingPolarMissQueue;
	Queue<missions*>* CompletedMissQueue;
	PriorityQueue<Rover*>* AvailableEmergRovQueue;
	PriorityQueue<Rover*>* AvailablePolarQueue;
	PriorityQueue<Rover*>* InExecRoverQueue;
	Queue<Rover*>* InCheckUpPolarQueue;
	Queue<Rover*>* InCheckUpEmergQueue;
	Queue<Rover*>* MaintenancePolarQueue;
	Queue<Rover*>* MaintenanceEmergQueue;


public:
	MarsStation(/*UI* UIp*/);


	void addtoQueue(missions* missP); //Add mission created by Event to its aprptiate queue

	//loading functions
	void loadRovers(int EmergencyRoversCount, int PolarRoversCount, int* EmergencyRoverSpeed, int* PolarRoverSpeed, int NumberofMissionsBefCheckUp, int EmergencyCheckUpDuration, int PolarCheckupDuration);
	void LoadEvents(int totnumber, int id, int day, Mission_Type MT, int target, int duration, int significance);//to enqueue event that will be read from the UI 

	int GetPriority(missions* missionP);//to calculate priority of emergency mission before enqueueing in priorty queue
	void Excute_events();//excute events according to their day
	void AssignMissions();//check if there are any waiting missions on this day and assign to rover if availble 
	bool GetAvailableRover(missions* missionP);//check if there is an availble rover according to mission type 
	void AssignAvailableRover(missions* Mission, Rover* RovertobeAssigned);
	void AssignInMaintenanceRover(missions* Mission, Rover* RovertobeAssigned);
	//////////////////////////////

	void UpdateCurrDay(); //increments current day and calls other incrementing functions below
	void CheckCompletedMissions(); //checks if any of the missions in execution are done and move them to completed missions queue
	void UpdateRoverStatus(Rover* rp); //called in CheckCompletedMissions to move rover that just finished a mission to either availbe or check up rovers queue
	void CheckUpduartionEnd(); //check if the rovers in check up finished their check up duration and move them to availble rovers queue
	void updateWaitingTime(); //icrement waiting time of missions that did not get assigned on this day

	/////////////////////////////

	void SaveOutputFile(ofstream& outputF);  //Saving output file (UI calls this function and send the File so it can write into it

	//output file calculations
	float CalculateAvgWaiting();
	int calculateCD(missions* M);
	float AvgExecTime();

	/////////////////////////////

	void StartSim(int t);  //starts simulation in appropriate mode, called in UI

	//Simulation modes
	void InteractiveMode();
	void SilentMode();
	void StepbyStepMode();

	///////////////////////////////

	//Functions to Print in Console, Calls UI to COUT appropriate data
	void printMissionsLine();
	void printInExecMiss_Rovers();
	void Print_Rover_Line();
	void Print_InCheckUp_Rovers();
	void Print_Maintenance_Rovers();
	void Print_CompletedMissions();

	/////////////////////////////

	~MarsStation(); //destructor
};

#endif
