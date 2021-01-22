#include "CancellationEvent.h"

#include "..\Rest\Restaurant.h"

CancellationEvent::CancellationEvent(int Time, int ID) :Event(Time, ID)
{
	//there is no more data member
}

void CancellationEvent::Execute(Restaurant* pRest)
{
	pRest->cancellorder(OrderID);   //here we cancelled the order from Normal orders Queue



}