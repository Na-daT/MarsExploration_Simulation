#pragma once
#include "../DataStructures/Queue.h"
#include"../DataStructures/PriorityQueue.h"
#include"../Events/Event.h"

class MarsStation
{
private:
	Queue<Event*>* EventsQueue;
};

