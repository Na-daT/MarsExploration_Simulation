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

	int* arr[] = 
	{
		&EmergencyRoversCount/*, &PolarRoversCount, &EmergencyRoverSpeed,&PolarRoverSpeed
		&NumberofMissionsBefCheckUp,& EmergencyCheckUpDuration, & */
	};
	
	for (int i = 0; i < 7;i++)
	{

	}


	//loadRovers function
	//call loadEvent(rest of file)

}
