#include "missions.h"

missions::missions(int id, int FD, int targ_loc, int duration, int sig, Mission_Type MT):
	ID(id), FormDay(FD), Tarloc(targ_loc), MissDur(duration), MissSign(sig)
{
	MissionType = MT;
	waiting_time = 0;
	Status = Waiting;
}

int missions::getID()
{
	return ID;
}

Mission_Type missions::getType()
{
	return MissionType;
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

void missions::setStatus(Mission_Status St)
{
	Status = St;
}

void missions::setEndDate(int d)
{
	ExecutionEndDay = d + MissDur + timeForRoverFromAndToTLOC;
}

int missions::getMissEndDay()
{
	return ExecutionEndDay;
}

void missions::setPriority(int p)
{
	priority = p;
}

int missions::getWaitingtime()
{
	return waiting_time;
}

void missions::setWaitingtime(int t)
{
	waiting_time = t;
}

void missions::setTimeFromToTLOC(int n)
{
	timeForRoverFromAndToTLOC = n;
}

int missions::getTimeFromToTLOC()
{
	return timeForRoverFromAndToTLOC;
}

int missions::getFullTimeEx()
{
	int n = MissDur + timeForRoverFromAndToTLOC;
	return n;
}

void missions::setnewFormulationDay(int d)
{
	FormDay = d;
}

int missions::getPriority()
{
	return priority;
}
