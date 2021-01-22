#include "PromotionEvent.h"

#include "..\Rest\Restaurant.h"

PromotionEvent::PromotionEvent(int Time, int ID, double extramony) :Event(Time, ID)
{

	ExMony = extramony;
}


void PromotionEvent::Execute(Restaurant* pRest)
{
	pRest->promoteorder(OrderID, ExMony);
}