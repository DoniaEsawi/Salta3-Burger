#include "Order.h"
#include<cmath>
Order::Order(ORD_TYPE r_Type,int arrtime, int id, int size, double totalmoney)
{
	ID = (id>0&&id<1000)?id:0;	//1<ID<999
	type = r_Type;
	status = WAIT;
	WaitTime = 0;//initially 0 and will increment each time step
	totalMoney =(totalmoney > 0)?totalmoney:0;
	setArrTime(arrtime);
	setOrderSize(size);
	Priority = 0;
	Urgent = false;
	
}

Order::~Order()
{
}

int Order::GetID()
{
	return ID;
}


ORD_TYPE Order::GetType() const
{
	return type;
}




void Order::setStatus(ORD_STATUS s)
{
	status = s;
}

ORD_STATUS Order::getStatus() const
{
	return status;
}
void Order::setOrderSize(int size)
{

	orderSize = (size>0)?size:0;
}

void Order::setPriority()
{
	Priority = 10*float(ArrTime * orderSize)/(totalMoney);

}
int Order::getOrderSize()const
{
	return orderSize;
}
int Order::getArrTime()const
{
	return ArrTime;
}
int Order::getServTime()const
{
	return ServTime;
}
int Order::getServInt()const
{
	return srvInt;
}
int Order::getWaitTime()const
{
	return WaitTime;
}
int Order::getFinishTime()const
{
	return FinishTime;
}

float Order::getPriority()
{
	return  Priority;
}
void Order::Promote(double& addedmoney)
{
	type = TYPE_VIP;
	totalMoney += abs(addedmoney);
	setPriority();
}
void Order::setArrTime(int& arr)
{
	ArrTime = abs(arr);
}

void  Order::setServTime( int& serv)///as TimeStep
{
	ServTime = serv;
}
void  Order::setServInt( int& serv)////as time interval
{
	srvInt = serv;/////will depend on the cook
}
void Order::setWaitTime()
{
	WaitTime = ServTime -ArrTime;
}
void Order::setFinishTime()
{
	FinishTime = ServTime+srvInt ;
}
void Order::Serve(int crrTS)
{
	////Uhmmm ... lets talk a bit about the Algorithm I am following
	//first and after making the queue of the events ... I take the orders by dequeuing 
	//each event"I guess that will be implementing in ArrivalEvent class" so that i prepare the queue of the Orders
	//... During every iterartion I make an object of class order and construct it 
	////with the data members from the input file 
	///in case of VTP orders We call fundtion setPriority to calculate the priority of the order then assign it to priority queue 
	///when we assign an order to a cook .. depending on the cook speed we calculate the service interval 
	//and call the function of the order object ,i.e:order.setServInt(cookSpeed>=orderSize?1:Ceil(float(ordersize)/cookspeed))
	///and finally we call Serve function by passing the current time step we are in to it
	///so that we get the finish time 
	setServTime(crrTS);
	setWaitTime();
	setFinishTime();
	setStatus(SRV);

}

void Order::setUrgent(bool flag)
{
	Urgent = flag;
}

bool Order::isUrgent()
{
	return Urgent;
}