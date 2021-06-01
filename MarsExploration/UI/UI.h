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
	SystemMode GetModeofOperation();
	void SaveFile();
	void InteractiveMode();
	void SilentMode();
	void StepbyStepMode();
};

#endif 