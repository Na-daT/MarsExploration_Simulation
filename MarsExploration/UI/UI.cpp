#include "UI.h"



bool UI::LoadStation()
{
	string s;
	cin >> s;

	InputFile.open(s, ios::in);

	if (InputFile.is_open())
	{
		LoadStation(InputFile);
		return true;
	}
	else return false;
}

bool UI::LoadStation(ifstream& inputFile)
{
	int EmergencyRoversCount, PolarRoversCount;
	int EmergencyRoverSpeed, PolarRoverSpeed; //{polar speed, Emergency speed}
	int NumberofMissionsBefCheckUp;
	int EmergencyCheckUpDuration, PolarCheckupDuration; //{polarDuration, Emergency Duration}

	// creating an array containing pointers to these variables in order to easily loop over them
	// to store the data from the input file
	int* arr[] =
	{
		&EmergencyRoversCount, & PolarRoversCount, & EmergencyRoverSpeed,
		& PolarRoverSpeed, & NumberofMissionsBefCheckUp, & EmergencyCheckUpDuration, & PolarCheckupDuration
		
	};


	
	for (int i = 0; i < 7;i++)
	{
		inputFile >> *arr[i];

	}


	//loadRovers function
	//call loadEvent(rest of file) 

}

