#pragma once

#include "Event.h"
class CancellationEvent:public Event
{
	//there is no more data
public:
	CancellationEvent(int Time, int ID);

	virtual void Execute(Restaurant* pRest);	//override execute function
};

