#ifndef __RESTAURANT_H_
#define __RESTAURANT_H_

#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\GUI\GUI.h"
#include "..\Generic_DS\Queue.h"
#include "..\Generic_DS\PriorityQueue.h"
#include "..\Events\Event.h"
#include"..\Generic_DS\LinkedList.h"
#include"..\LinkedList.h"

#include <fstream>
#include<time.h>
#include "Order.h"

// it is the maestro of the project
class Restaurant  
{	
private:
	GUI *pGUI;
	Queue<Event*> EventsQueue;	//Queue of all events that will be loaded from file

	string IPfilename;
	string OPfilename;
	float InjProp;

	//Queue for each type of Cooks
	Queue<Cook*> NcooksQ;
	Queue<Cook*> GcooksQ;
	Queue<Cook*> VcooksQ;

	//Priority Queue For Busy Cooks and queues for inBreak cooks
	PriorityQueue<Cook*> busyCooksQ;

	PriorityQueue<Cook*> CooksInBreak;
	Queue<Cook*>CooksInRest;

	//Priority Queue for vip orders depending on Priority Equation
	PriorityQueue<Order*>  QVIP_Order;
	//
	LinkedList<Order*> LNormal_Order;
	Queue<Order*> Qvegan_Order;
	Queue<Order**> QUrgentOrders;
	PriorityQueue<Order*> InServing;
	Queue<Order*>FinishedList;
public:
	Restaurant();
	~Restaurant();
	void ExecuteEvents(int TimeStep);	//executes all events at current timestep
	void RunSimulation();
	void fileLoading();
	void outputFileLoading();
	void SimpleSimulator();
	void Interactive();
	void Restaurant_Modes(int Mode);
	void FillDrawingList();
	void AddtoVIPQueue(Order*& po);
	void AddtoNOList(Order*& po);
	void AddtoVEQueue(Order*& po);
	void cancellorder(int ID);
	void promoteorder(int ID, double exmoney);
	void Executepromotion(int CurrentTimeStep);
	void serve_VIP_orders(int CurrentTimeStep);
	void serve_Normal_orders(int CurrentTimeStep);
	void serve_Vegan_orders(int CurrentTimeStep);
	void getfrombusyCookQ(int CurrentTimeStep);
	void getfromBreakCookQ(int CurrentTimeStep);
	void getfromInServingQ(int CurrentTimeStep);
	void CheckUrgentOrders(int CurrentTimeStep);
	bool GetCooksFor_Urgent_VIP(int CurrentTimeStep);
	void Serve_Urgent_VIP(int CurrentTimeStep);


	int numNcooks, numGcooks, numVcooks, Ncookspeed_min, Ncookspeed_max, Gcookspeed_min, Gcookspeed_max, Vcookspeed_min, Vcookspeed_max;
	int numOrdersBbreak, Nbreak_min, Nbreak_max, Gbreak_min, Gbreak_max, Vbreak_min, Vbreak_max, numofevents;


	int NCookNum , GCookNum , VCookNum;
	int injcooksnum = 0;
	int NWaitNum, GWaitNum, VWaitNum;
	int SRVorders, AutoP, RstPrd, VIP_WT;
	int UrgentOredersNum=0;
	int Userved=0,Vserved = 0, Nserved = 0, Gserved = 0;
	float numAutoPromOrders = 0;
	int originalNormOrdCount = 0;
	bool Health_Emergency(int CurrentTimeStep);
	void getfromRestCookQ(int CurrentTimeStep);
	//This Strings are for storing Cooks and Orders info to be Printed on GUI
	string UFinfo;
	string VFinfo;
	string NFinfo;
	string GFinfo;
	////////////////////
	
	float RandomFloat(float a, float b) {
		//srand((int)time(0));
		float random = ((float)rand()) / (float)RAND_MAX;
		float diff = b - a;
		float r = random * diff;
		return a + r;
	}
	int rangeRandomAlg2(int min, int max) {
		int n = abs(max - min )+ 1;
		int remainder = RAND_MAX % n;
		int x;
		do {
			x = rand();
		} while (x >= RAND_MAX - remainder);
		return min + x % n;
	}

};

#endif