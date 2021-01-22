#ifndef __ORDER_H_
#define __ORDER_H_

#include "..\Defs.h"

class Order
{

protected:
	int ID;         //Each order has a unique ID (from 1 --> 999 )
	ORD_TYPE type;		//order type: Normal, vegan, VIP
	ORD_STATUS status;	//waiting, in-service, done
	int Distance;	//The distance (in meters) between the order location and the resturant 

	double totalMoney;	//Total order money

	int ArrTime, ServTime, FinishTime;	//arrival, service start, and finish times


	int orderSize;
	float Priority;
	int WaitTime;
	int srvInt;
	bool Urgent;
public:
	Order( ORD_TYPE r_Type,int arrtime, int ID, int size,double totalmoney);
	virtual ~Order();

	int GetID();


	ORD_TYPE GetType() const;

	
	void setStatus(ORD_STATUS s);
	ORD_STATUS getStatus() const;

	
	void setPriority();
	void setOrderSize(int size);
	float getPriority();
	int getOrderSize()const;
	int getArrTime()const;
	int getServTime()const;
	int getServInt()const;
	int getWaitTime()const;
	int getFinishTime()const;
	void Promote( double& addedmoney);
	void setArrTime(int& arr);

	void setServTime( int& serv);
	void setServInt(int& serv);
	void setFinishTime();
	void setWaitTime();
	void Serve( int);
	void setUrgent(bool flag);
	bool isUrgent();
};

#endif