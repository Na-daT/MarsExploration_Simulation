#pragma once
#ifndef _UI_
#define _UI_

#include <iostream>
using namespace std;
#include <fstream>
#include "../Station_classes/MarsStation.h"
#include "../Defs.h"

//class MarsStation;

class UI
{
	ifstream InputFile;
	ofstream OutputFile;
	MarsStation *MarsP;
public: 
	UI(MarsStation* MarsStP);
	bool LoadStation();
	bool LoadStation(ifstream& inputFile);
	void LoadFormEvents(ifstream& inputFile);
	void GetModeofOperation();
	void SaveFile();
	void prin_CurrentDay(int d);
	void print_waitingMissions(int totwaiting, int* waitEmID, int totWaitEm, int totWaitPolar, int* waitPolarID);
	void Print_inExecMis_Rovers(int totinEx, int NumofEmergency, int NumofPolar,int* EmergencyMissionIDs, int* EmergencyRoversIDs, int* PolarMissionsIDs, int* PolarRoversIDs);
	void WaitForUserClick();
	//void PrintOutput(int currDay, );
};

#endif 