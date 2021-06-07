#include "UI.h"
#include <conio.h>

UI::UI(MarsStation* MarsStP)
{
	MarsP = MarsStP;
}

bool UI::LoadStation()
{
	///takes input file name from user and opens it
	cout << "Enter file name to load: ";
	string s;
	cin >> s;
	cout << endl;

	s = "input files\\" + s;

	InputFile.open(s, ios::in);

	//if open loads the file
	if (InputFile.is_open())
	{
		LoadStation(InputFile);
		return true;
	}
	else
	{
		cout << "error in input file" << endl;
		return false;
	}
}

bool UI::LoadStation(ifstream& inputFile)
{
	int EmergencyRoversCount, PolarRoversCount;
	int* EmergencyRoverSpeed;
	int* PolarRoverSpeed; //{polar speed, Emergency speed}
	int NumberofMissionsBefCheckUp;
	int EmergencyCheckUpDuration, PolarCheckupDuration; //{polarDuration, Emergency Duration}



	//array containing all the values to be stored instead of reading one by one
	int* arr[] =
	{
		&EmergencyRoversCount, &PolarRoversCount, &NumberofMissionsBefCheckUp, &EmergencyCheckUpDuration, &PolarCheckupDuration
	};

	for (int i = 0; i < 2;i++)
	{
		inputFile >> *arr[i];
	}

	EmergencyRoverSpeed = new int[EmergencyRoversCount];
	PolarRoverSpeed = new int[PolarRoversCount];

	for (int i = 0; i < EmergencyRoversCount; i++) {
		inputFile >> EmergencyRoverSpeed[i];
	}

	for (int i = 0; i < PolarRoversCount; i++) {
		inputFile >> PolarRoverSpeed[i];
	}

	for (int i = 2; i < 5; i++)
	{
		inputFile >> *arr[i];
	}


	if (PolarRoversCount == 0 && EmergencyRoversCount == 0)
	{
		cout << "no rovers input" << endl;
		return false;
	}

	//sends data to mars station to enqueue the rovers and set data members
	MarsP->loadRovers(EmergencyRoversCount, PolarRoversCount, EmergencyRoverSpeed, PolarRoverSpeed, NumberofMissionsBefCheckUp, EmergencyCheckUpDuration, PolarCheckupDuration);

	//send the rest of the file to load Events data
	LoadFormEvents(inputFile);

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
			&Ev_day,
			&Event_ID,
			&TLOC, &MDUR, &SIG

		};
		inputFile >> EventType;
		inputFile >> Missiontype;

		for (int i = 0;i < 5;i++)
		{
			inputFile >> *arr[i];
		}

		Mission_Type MT;
		MT = (Missiontype == 'P' ? Polar : Emergency);

		MarsP->LoadEvents(EventsNO, Event_ID, Ev_day, MT, TLOC, MDUR, SIG);

	}

}

void UI::GetModeofOperation()
{
	int t;
	cout << "please enter mode of operation, 1 for interactive, 2 for silent, 3 for step-by-step" << endl;

	cin >> t;
	MarsP->StartSim(t);

}


////////////// printing functions //////////////
void  UI::SaveFile()
{
	string s;
	cout << "Please Enter File Name: ";
	cin >> s;
	s = "output files\\" + s;

	OutputFile.open(s, ios::out);

	MarsP->SaveOutputFile(OutputFile);
	cout << "Simulation ends, Output file created" << endl;
}

void UI::prin_CurrentDay(int d)
{
	cout << "Current Day:" << d << endl;
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
		cout << waitEmID[i] << ",";
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
	cout << "press any key to review next day..." << endl;
	cout << endl;
	_getch();
}

void UI::SilentModePrintout()
{
	cout << "Silent Mode" << endl << " Simulation Starts..." << endl;
}

void UI::Print_Rover_Line(int totRov, int* AvailableEmergencyIDs, int* AvailablePolerIDs, int AvailableEmrgency, int AvailablePolar)
{
	cout << totRov << "Available Rovers: [";

	if (AvailableEmrgency != 0)
	{
		for (int i = 0; i < AvailableEmrgency;i++)
		{
			cout << AvailableEmergencyIDs[i] << ",";
		}
	}
	cout << "] (";

	if (AvailablePolar != 0)
	{
		for (int i = 0; i < AvailablePolar;i++)
		{
			cout << AvailablePolerIDs[i] << ",";
		}
	}
	cout << ")" << endl;
	cout << "-------------------------------------------------------" << endl;
}

void UI::Print_CheckUp_Rovers(int totInCheckUp, int EmergencyinCheckUP, int PolarInCheckUp, int* EmIDs, int* PolarIDs)
{
	cout << totInCheckUp << " In-Checkup Rovers: [";
	if (EmergencyinCheckUP != 0)
	{
		for (int i = 0;i < EmergencyinCheckUP;i++)
		{
			cout << EmIDs[i] << ",";
		}
	}
	cout << "] (";

	if (PolarInCheckUp != 0)
	{
		for (int i = 0; i < PolarInCheckUp;i++)
		{
			cout << PolarIDs[i] << ",";
		}
	}
	cout << ")" << endl;
	cout << "-------------------------------------------------------" << endl;
}

void UI::Print_Maintenance_Rovers(int totInCheckUp, int EmergencyinCheckUP, int PolarInCheckUp, int* EmIDs, int* PolarIDs)
{
	cout << totInCheckUp << " Maintenance Rovers: [";
	if (EmergencyinCheckUP != 0)
	{
		for (int i = 0; i < EmergencyinCheckUP; i++)
		{
			cout << EmIDs[i] << ",";
		}
	}
	cout << "] (";

	if (PolarInCheckUp != 0)
	{
		for (int i = 0; i < PolarInCheckUp; i++)
		{
			cout << PolarIDs[i] << ",";
		}
	}
	cout << ")" << endl;
	cout << "-------------------------------------------------------" << endl;
}

void UI::CompletedMissions(int totMissions, int EmMissions, int PolarMissions, int* Emergency_IDs, int* Polar_IDs)
{
	cout << totMissions << " Completed Missions: [";
	if (EmMissions != 0)
	{
		for (int i = 0; i < EmMissions;i++)
		{
			cout << Emergency_IDs[i] << ",";
		}
	}
	cout << "] (";

	if (PolarMissions != 0)
	{
		for (int i = 0;i < PolarMissions;i++)
		{
			cout << Polar_IDs[i] << ",";
		}
	}
	cout << ")" << endl;
	cout << "-------------------------------------------------------" << endl;
	cout << endl;

}

UI::~UI()
{
	if (MarsP)
		delete MarsP;
}

/*void UI::InteractiveMode()
{



}
*/
