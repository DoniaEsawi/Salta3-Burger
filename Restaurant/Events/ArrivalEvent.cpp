#include "ArrivalEvent.h"
#include "..\Rest\Restaurant.h"


ArrivalEvent::ArrivalEvent(int eTime, int oID, ORD_TYPE oType):Event(eTime, oID)
{
	OrdType = oType;
}

ArrivalEvent::ArrivalEvent(int eTime, int oID, ORD_TYPE oType, int Osize, double Omoney): Event(eTime, oID)
{
	OrdType = oType;
	OrderSize = Osize;
	OrdMoney = Omoney;
	

}

void ArrivalEvent::Execute(Restaurant* pRest)
{
	//This function should create an order and fills its data 
	// Then adds it to normal, vegan, or VIP order lists that you will create in phase1

	////TYPE_NRM,	//normal order
	////	TYPE_VGAN,	//vegan
	////	TYPE_VIP,	//VIP
	
	//Order(ORD_TYPE r_Type, int arrtime, int ID, int size, int totalmoney);
	///For the sake of demo, this function will just create an order and add it to DemoQueue
	///Remove the next code lines in phases 1&2
	Order* pOrd = new Order(OrdType, EventTime,OrderID, OrderSize, OrdMoney);
	//pRest->AddtoDemoQueue(pOrd);
	pOrd->setStatus(WAIT);
	if (OrdType == TYPE_NRM)
	{
		pRest->AddtoNOList(pOrd);
	}
	else if (OrdType == TYPE_VGAN)
	{
		pRest->AddtoVEQueue(pOrd);
	}
	else if (OrdType == TYPE_VIP)
	{
		pRest->AddtoVIPQueue(pOrd);
	}

}
