#pragma once
#include <iostream>
using namespace std;
#include <fstream>
#include "../Station_classes/MarsStation.h"

class UI
{
	ifstream InputFile;
public: 
	bool LoadStation();
	bool LoadStation(ifstream& inputFile);



};

