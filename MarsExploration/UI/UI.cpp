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
	MarsStation* MarsP;


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

	MarsP->loadRovers(EmergencyRoversCount, PolarRoversCount, EmergencyRoverSpeed, PolarRoverSpeed, NumberofMissionsBefCheckUp, EmergencyCheckUpDuration, PolarCheckupDuration);

	LoadFormEvents(inputFile);
	//loadRovers function
	//call loadEvent(rest of file) 

}

void UI::LoadFormEvents(ifstream& inputFile)
{
	int EventsNO;
	inputFile >> EventsNO;
	

	for (int i = 0; i < EventsNO; i++)
	{
		char Event;
		inputFile >> Event;
		char Missiontype;

		inputFile >> Missiontype;

		switch (Missiontype)
		{
		case('E'):
			
		case ('P'):
			
		}

	}

}

