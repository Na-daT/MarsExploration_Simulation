#include "UI.h"


UI::UI(MarsStation* MarsStP)
{
	MarsP = MarsStP;
}

bool UI::LoadStation()
{
	string s;
	cin >> s;

	s = "input files\\" + s;

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

	MarsP->loadRovers(EmergencyRoversCount, PolarRoversCount, EmergencyRoverSpeed, PolarRoverSpeed, NumberofMissionsBefCheckUp, EmergencyCheckUpDuration, PolarCheckupDuration);

	LoadFormEvents(inputFile);
	//loadRovers function
	//call loadEvent(rest of file) 
	return true;
}

void UI::LoadFormEvents(ifstream& inputFile)
{
	int EventsNO;
	inputFile >> EventsNO;

	char EventType;
	char Missiontype;

	int Ev_day;
	int Event_ID;
	int TLOC;
	int MDUR;
	int SIG;


	for (int i = 0; i < EventsNO; i++)
	{
		

		int* arr[] =
		{
			&Event_ID,
			& Ev_day,
			& TLOC, & MDUR, & SIG

		};
		inputFile >> EventType;
		inputFile >> Missiontype;
		
		for (int i = 0;i < 5;i++)
		{
			inputFile >> *arr[i];
		}

		Mission_Type MT;
		MT = (Missiontype == 'P' ? Polar : Emergency);

		MarsP->LoadEvents(EventsNO,Event_ID, Ev_day,MT,TLOC,MDUR,SIG );

	}

}

void UI::GetModeofOperation()
{
	int t;
	cout << "please enter mode of operation, 1 for interactive, 2 for silent, 3 for step-by-step";

	cin >> t;
	MarsP->StartSim(t);
	
}

void  UI::SaveFile()
{
	string s;
	cout << "Please Enter File Name: ";
	cin >> s;
	s ="output files\\" + s;

	OutputFile.open(s, ios::out);

	
	MarsP->SaveOutputFile(OutputFile);

}

void UI::prin_CurrentDay(int d)
{
	cout << "Current Day:" <<d << endl;
}

void UI::print_waitingMissions(int totwaiting, int* waitEmID, int totWaitEm, int totWaitPolar, int* waitPolarID)
{
	cout << totwaiting << " " << "Waiting Missions: " << "[";

	if (totwaiting == 0)
	{
		cout << "] ()" << endl;
		cout << "-------------------------------------------------------" << endl;
		return;
	}
	for (int i = 0; i < totWaitEm; i++)
	{
		cout << waitEmID[i] <<",";
	}
	cout << "] (";
	for (int i = 0; i < totWaitPolar; i++)
	{
		cout << waitPolarID[i] << ",";
	}
	cout << ")" << endl;
	cout << "-------------------------------------------------------" << endl;

}

void UI::Print_inExecMis_Rovers(int totinEx, int NumofEmergency, int NumofPolar, int* EmergencyMissionIDs, int* EmergencyRoversIDs, int* PolarMissionsIDs, int* PolarRoversIDs)
{
	cout << totinEx << " In-Execution Missions/Rovers: " << " [";
	for (int i = 0; i < NumofEmergency; i++)
	{
		cout << EmergencyMissionIDs[i] << "/" << EmergencyRoversIDs[i] << ", ";
	}
	cout << "] ";

	cout << "(";
	for (int i = 0; i < NumofPolar;i++)
	{
		cout << PolarMissionsIDs[i] << "/" << PolarRoversIDs[i] << " ,";
	}
	cout << ") " << endl;

	cout << "-------------------------------------------------------" << endl;
}

void UI::WaitForUserClick()
{
	cout << "press Enter to review next day...";
	cin.get();
}

/*void UI::InteractiveMode()
{
	


}
*/
