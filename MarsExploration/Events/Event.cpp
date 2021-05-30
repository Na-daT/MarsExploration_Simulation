#include "Event.h"

Event::Event(int id, int Etime):
	ID(id),Event_day(Etime)
{
}

int Event::getEvent_day()
{
	return Event_day;
}
