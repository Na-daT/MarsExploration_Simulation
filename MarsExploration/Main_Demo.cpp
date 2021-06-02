#include "Station_classes/MarsStation.h"
#include "UI/UI.h"

int main()
{
	MarsStation* MS = new MarsStation();
	UI* uiP = new UI(MS);
	uiP->GetModeofOperation();
}