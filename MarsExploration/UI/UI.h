#pragma once
#include <iostream>
using namespace std;
#include <fstream>
#include "../Station_classes/MarsStation.h"

class UI
{
	ifstream InputFile;
	MarsStation* MarsP;
public: 
	UI(MarsStation* MarsStP);
	bool LoadStation();
	bool LoadStation(ifstream& inputFile);
	void LoadFormEvents(ifstream& inputFile);
	SystemMode GetModeofOperation();
	void SaveFile();
};

