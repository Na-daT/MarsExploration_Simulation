#pragma once
#ifndef _UI_
#define _UI_

#include <iostream>
using namespace std;
#include <fstream>
#include "../Station_classes/MarsStation.h"
#include "../Defs.h"



class UI
{
	ifstream InputFile;
	ofstream OutputFile;
	MarsStation *MarsP;

public: 
	UI(MarsStation* MarsStP);

	bool LoadStation(); //takes files name from user and calls appropraite loading functions
	bool LoadStation(ifstream& inputFile); //reads all data from file and calls LoadFormEvents
	void LoadFormEvents(ifstream& inputFile); //reads events data from file
	void GetModeofOperation(); //cin mode of operation from user
	void SaveFile(); //cin name of output file from user and loads appropraite printing functions


	//printing functions, calls approprate functions from mars station and prints the data on the console
	void prin_CurrentDay(int d);
	void print_waitingMissions(int totwaiting, int* waitEmID, int totWaitEm, int totWaitPolar, int* waitPolarID);
	void Print_inExecMis_Rovers(int totinEx, int NumofEmergency, int NumofPolar,int* EmergencyMissionIDs, int* EmergencyRoversIDs, int* PolarMissionsIDs, int* PolarRoversIDs);
	void WaitForUserClick(); //waits for any key press (used in interactive mode)
	void SilentModePrintout();
	void Print_Rover_Line(int totRov, int* AvailableEmergencyIDs, int* AvailablePolerIDs, int AvailableEmrgency, int AvailablePolar);
	void Print_CheckUp_Rovers(int totInCheckUp, int EmergencyinCheckUP, int PolarInCheckUp, int* EmIDs, int* PolarIDs);
	void CompletedMissions(int totMissions, int EmMissions, int PolarMissions, int* Emergency_IDs, int* Polar_IDs);

	~UI(); //destructor
};

#endif 