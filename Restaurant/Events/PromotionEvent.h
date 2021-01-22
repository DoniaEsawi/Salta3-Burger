#pragma once

#include "Event.h"

class PromotionEvent :public Event
{
	double ExMony;
public:
	PromotionEvent(int Time, int ID, double extramony);
	virtual void Execute(Restaurant* pRest);	//override execute function


};
