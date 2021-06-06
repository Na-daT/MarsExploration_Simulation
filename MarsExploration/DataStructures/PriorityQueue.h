#pragma once
#pragma once
#ifndef _PriorityQueue_H
#define _PriorityQueue_H

#include <iostream>
#include "PriorityNode.h"

using namespace std;

template <typename T>
class PriorityQueue
{

	PriorityNode<T>* front;		
	int count;				

public:

	PriorityQueue()
	{
		front = nullptr;	
		count = 0;			
	}

	~PriorityQueue()
	{
		count = 0;
		T destructor;
		while (front)
			dequeue(destructor);
	}


	bool isEmpty()
	{
		if (front)			
			return false;		

		return true;			
	}

	// enqueue function to add new items to queue and sort them according to priority

	bool enqueue(const T& newEntry, int priority)
	{

		PriorityNode<T>* NewNodePtr = new PriorityNode<T>(newEntry, priority);


		if (isEmpty())
		{
			front = NewNodePtr;		
			count++;				
			return true;			
		}



		if (front->getPriority() < priority)
		{
			NewNodePtr->setNext(front);		
			front = NewNodePtr;			
			count++;						
			return true;					
		}
		else
		{

			PriorityNode<T>* temp = front;
			PriorityNode<T>* temp2 = front->getNext();


			while (temp2 && temp2->getPriority() >= priority)
			{
				temp = temp->getNext();
				temp2 = temp2->getNext();
			}

			NewNodePtr->setNext(temp2);			
			temp->setNext(NewNodePtr);
			count++;						
			return true;					
		}
	}

	bool dequeue(T& frontEntry)
	{

		if (isEmpty())
			return false;

		PriorityNode<T>* tempDel = front;			
		frontEntry = front->getItem();			
		front = front->getNext();					

		delete tempDel;							
		count--;									
		return true;								
	}

	

	bool peekFront(T& frontEntry)
	{

		if (isEmpty())
			return false;

		frontEntry = front->getItem();			
		return true;							
	}


}; // end queue
#endif
