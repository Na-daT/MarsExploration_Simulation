#include "missions.h"

missions::missions(int id, int FD, int targ_loc, int duration, int sig):
	ID(id), FormDay(FD), Tarloc(targ_loc), MissDur(duration), MissSign(sig)
{

}

Mission_Type missions::getType()
{
	return Mission_Type();
}

int missions::getMissSign()
{
	return MissSign;
}

int missions::getTarloc()
{
	return Tarloc;
}

int missions::getMissDur()
{
	return MissDur;
}

int missions::getFormD()
{
	return FormDay;
}
