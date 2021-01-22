#include <cstdlib>
#include <time.h>
#include <iostream>
using namespace std;

#include "Restaurant.h"
#include "..\Events\ArrivalEvent.h"

#include "..\Events\CancellationEvent.h"
#include"..\Events\PromotionEvent.h"

Restaurant::Restaurant() 
{
	pGUI = NULL;
}

void Restaurant::RunSimulation()
{
	pGUI = new GUI;
	PROG_MODE mode = pGUI->getGUIMode();
		
	switch (mode)	//Add a function for each mode in next phases
	{
	case MODE_INTR:
		Restaurant_Modes(1);
		break;
	case MODE_STEP:
		Restaurant_Modes(2);
		break;
	case MODE_SLNT:
		Restaurant_Modes(3);
		break;
	};

}



//////////////////////////////////  Event handling functions   /////////////////////////////

//Executes ALL events that should take place at current timestep
void Restaurant::ExecuteEvents(int CurrentTimeStep)
{
	Event *pE;
	while( EventsQueue.peekFront(pE) )	//as long as there are more events
	{
		if(pE->getEventTime() > CurrentTimeStep )	//no more events at current timestep
			return;

		pE->Execute(this);
		EventsQueue.dequeue(pE);	//remove event from the queue
		delete pE;		//deallocate event object from memory
	}

}
//promot Normer orders automatically promoted to VIP after it excess (Autop) time step in waiting List  
void Restaurant::Executepromotion(int CurrentTimeStep)
{

	Order* proOrder;
	while (LNormal_Order.peek(proOrder))                        //as long as there is more waiting Orders
	{
		if ((CurrentTimeStep - proOrder->getArrTime()) < AutoP) //no more event can be promoted in this time step
			return;

		LNormal_Order.DeleteFirst(proOrder);                   //remove order from Normal List
		double money = 0;
		proOrder->Promote(money);
		float priority = proOrder->getPriority();
		QVIP_Order.enqueue(proOrder, priority);                //add it to VIP queue
		numAutoPromOrders++;
	}
	

}
//Serving VIP orders after wating time excess VIP_WT to any cook in Break or Rest
void Restaurant::CheckUrgentOrders(int CurrentTimeStep)
{
	Order**vipordersarr;
	int size;
	vipordersarr = QVIP_Order.toArray(size);
	for (int i = 0; i < size;i++)
	{
		if (CurrentTimeStep - vipordersarr[i]->getArrTime() > VIP_WT && !vipordersarr[i]->isUrgent())
		{

			vipordersarr[i]->setUrgent(true);
			QUrgentOrders.enqueue(&vipordersarr[i]);
			UrgentOredersNum++;
		}
	}
}
bool Restaurant::GetCooksFor_Urgent_VIP(int CurrentTimeStep)
{
	Order** UrgOrder;
	Cook* urgentcook;
	float cookprio;
	bool found = false;
	if (QUrgentOrders.peekFront(UrgOrder))
	{
		if (VcooksQ.isEmpty() && GcooksQ.isEmpty() && NcooksQ.isEmpty())
		{
			if (CooksInBreak.peekFront(urgentcook, cookprio))        //When waiting time exceeds VIP_WT, we check for
			{                                                        // is there is Cooks in break queue
				CooksInBreak.dequeue(urgentcook, cookprio);
				if (urgentcook->GetType() == TYPE_VIP)               //return the cook to it's suitable list
					VcooksQ.enqueue(urgentcook);
				if (urgentcook->GetType() == TYPE_VGAN)
					GcooksQ.enqueue(urgentcook);
				if (urgentcook->GetType() == TYPE_NRM)
					NcooksQ.enqueue(urgentcook);
				found = true;
			}                                                       //if there is no cooks in break we check for
			else if (CooksInRest.peekFront(urgentcook))             //the cooks in rest but they work with half speed
			{
				CooksInRest.dequeue(urgentcook);
				
				urgentcook->Give_Urg(true);
				if (urgentcook->GetType() == TYPE_VIP)
					VcooksQ.enqueue(urgentcook);
				if (urgentcook->GetType() == TYPE_VGAN)
					GcooksQ.enqueue(urgentcook);
				if (urgentcook->GetType() == TYPE_NRM)
					NcooksQ.enqueue(urgentcook);
				found = true;
			}
			
		}
		else
			found = true;
	}
	return found;
}
void Restaurant::Serve_Urgent_VIP(int CurrentTimeStep)
{
	bool flag;
	CheckUrgentOrders(CurrentTimeStep);
	Order** Urgorder;
	UFinfo = "";
		while (QUrgentOrders.peekFront(Urgorder))
		{
			flag = GetCooksFor_Urgent_VIP(CurrentTimeStep);
			if (flag)
			{
				UFinfo = "Urgent-> ";
				Cook* Bcook;
				if (VcooksQ.dequeue(Bcook))           //check if there is available VIP cook
				{
					Bcook->setnumofOrderdServed(Bcook->getnumofOrderdServed() + 1);
					(*Urgorder)->setServTime(CurrentTimeStep); //set serving time with current time step 

					int ST = ceil(float((*Urgorder)->getOrderSize()) / Bcook->getSpeed()); //calculate the surving time
					(*Urgorder)->setServInt(ST);
					(*Urgorder)->setWaitTime();
					(*Urgorder)->setFinishTime();
					(*Urgorder)->setStatus(SRV);
					float priority = ((*Urgorder)->getFinishTime());             //set the priority of serving queue with the inverted finished time
					Bcook->assign_Order((*Urgorder)->GetID());
					InServing.enqueue(*Urgorder, priority);
					busyCooksQ.enqueue(Bcook, priority);                       //enque the cook in priority busy cook queue
					QUrgentOrders.dequeue(Urgorder);

					Userved++;
					UFinfo += "V" + to_string(Bcook->GetID()) + "(" + "V" + to_string((*Urgorder)->GetID()) + ") ";
				}
				else if (NcooksQ.dequeue(Bcook))                              //check if there is available Normal cook when there is no VIP
				{
					Bcook->setnumofOrderdServed(Bcook->getnumofOrderdServed() + 1);
					(*Urgorder)->setServTime(CurrentTimeStep);                       //set serving time with current time step 

					int ST = ceil(float((*Urgorder)->getOrderSize()) / Bcook->getSpeed()); //calculate the surving time
					(*Urgorder)->setServInt(ST);
					(*Urgorder)->setWaitTime();
					(*Urgorder)->setFinishTime();
					(*Urgorder)->setStatus(SRV);
					float priority = ((*Urgorder)->getFinishTime());             //set the priority of serving queue with the inverted finished time
					InServing.enqueue(*Urgorder, priority);
					Bcook->assign_Order((*Urgorder)->GetID());
					busyCooksQ.enqueue(Bcook, priority);                       //enque the cook in priority busy cook queue
					QUrgentOrders.dequeue(Urgorder);

					Userved++;
					UFinfo += "N" + to_string(Bcook->GetID()) + "(" + "V" + to_string((*Urgorder)->GetID()) + ") ";
				}
				else if (GcooksQ.dequeue(Bcook))                              //check if there is available Vegan cook when there is no VIP&&Normal
				{
					Bcook->setnumofOrderdServed(Bcook->getnumofOrderdServed() + 1);
					(*Urgorder)->setServTime(CurrentTimeStep); //set serving time with current time step 

					int ST = ceil(float((*Urgorder)->getOrderSize()) / Bcook->getSpeed()); //calculate the surving time
					(*Urgorder)->setServInt(ST);
					(*Urgorder)->setWaitTime();
					(*Urgorder)->setFinishTime();
					(*Urgorder)->setStatus(SRV);
					float priority = ((*Urgorder)->getFinishTime());             //set the priority of serving queue with the inverted finished time
					InServing.enqueue(*Urgorder, priority);
					Bcook->assign_Order((*Urgorder)->GetID());
					busyCooksQ.enqueue(Bcook, priority);                       //enque the cook in priority busy cook queue
					QUrgentOrders.dequeue(Urgorder);

					Userved++;
					UFinfo += "G" + to_string(Bcook->GetID()) + "(" + "V" + to_string((*Urgorder)->GetID()) + ") ";
				}
				else
				{
					return;                                         //there is no more available cooks in this timestep 
				}
			}
			else
				return;
		}
	
}
void Restaurant::serve_VIP_orders(int CurrentTimeStep)
{
	Order* proOrder;
	float prio;
	VFinfo = "";
	while (QVIP_Order.peekFront(proOrder,prio))  //get orders from VIP waiting Queue to be serve
	{

		if (!proOrder->isUrgent())
		{
			Cook* Bcook;
			if (VcooksQ.dequeue(Bcook))           //check if there is available VIP cook
			{
				Bcook->setnumofOrderdServed(Bcook->getnumofOrderdServed() + 1);

				int ST = ceil(float(proOrder->getOrderSize()) / Bcook->getSpeed()); //calculate the surving time
				
				proOrder->setServInt(ST);
				proOrder->Serve(CurrentTimeStep);
				float priority = (proOrder->getFinishTime());             //set the priority of serving queue with the inverted finished time
				InServing.enqueue(proOrder, priority);

				Bcook->assign_Order(proOrder->GetID());
				busyCooksQ.enqueue(Bcook, priority);                       //enque the cook in priority busy cook queue
				QVIP_Order.dequeue(proOrder, prio);
				Vserved++;
				VFinfo += "V" + to_string(Bcook->GetID())+"("+ "V" + to_string(proOrder->GetID())+") ";
			}
			else if (NcooksQ.dequeue(Bcook))                              //check if there is available Normal cook when there is no VIP
			{
				Bcook->setnumofOrderdServed(Bcook->getnumofOrderdServed() + 1);

				int ST = ceil(float(proOrder->getOrderSize()) / Bcook->getSpeed()); //calculate the surving time
				proOrder->setServInt(ST);
				proOrder->Serve(CurrentTimeStep);
				float priority = (proOrder->getFinishTime());             //set the priority of serving queue with the inverted finished time
				InServing.enqueue(proOrder, priority);
				Bcook->assign_Order(proOrder->GetID());
				busyCooksQ.enqueue(Bcook, priority);                       //enque the cook in priority busy cook queue
				QVIP_Order.dequeue(proOrder, prio);
				
				Vserved++;
				VFinfo += "N" + to_string(Bcook->GetID()) + "(" + "V" + to_string(proOrder->GetID()) + ") ";
			}
			else if (GcooksQ.dequeue(Bcook))                              //check if there is available Vegan cook when there is no VIP&&Normal
			{
				Bcook->setnumofOrderdServed(Bcook->getnumofOrderdServed() + 1);

				int ST = ceil(float(proOrder->getOrderSize()) / Bcook->getSpeed()); //calculate the surving time
				proOrder->setServInt(ST);
				proOrder->Serve(CurrentTimeStep);
				float priority = (proOrder->getFinishTime());             //set the priority of serving queue with the inverted finished time
				InServing.enqueue(proOrder, priority);
				Bcook->assign_Order(proOrder->GetID());
				busyCooksQ.enqueue(Bcook, priority);                       //enque the cook in priority busy cook queue
				QVIP_Order.dequeue(proOrder, prio);
				
				Vserved++;
				VFinfo += "G" + to_string(Bcook->GetID()) + "(" +  "V" + to_string(proOrder->GetID()) + ") ";
			}
			else
			{
				return;                                         //there is no more available cooks in this timestep 
			}
		}
		else
		{
			QVIP_Order.dequeue(proOrder, prio);
		}
	}

}
void Restaurant::serve_Vegan_orders(int CurrentTimeStep)
{
	Order* proOrder;
	GFinfo = "";
	while (Qvegan_Order.peekFront(proOrder))  //get orders from Vegan waiting Queue to be serve
	{
		Cook* Bcook;
		if (GcooksQ.dequeue(Bcook))                              //check if there is available Vegan cook 
		{
			Bcook->setnumofOrderdServed(Bcook->getnumofOrderdServed() + 1);

			int ST = ceil(float(proOrder->getOrderSize() )/ Bcook->getSpeed()); //calculate the surving time
			proOrder->setServInt(ST);
			proOrder->Serve(CurrentTimeStep);
			float priority =  (proOrder->getFinishTime());             //set the priority of serving queue with the inverted finished time
			InServing.enqueue(proOrder, priority);
			Bcook->assign_Order(proOrder->GetID());
			busyCooksQ.enqueue(Bcook, priority);                       //enque the cook in priority busy cook queue
			Qvegan_Order.dequeue(proOrder);
			Gserved++;
			GFinfo += "G" + to_string(Bcook->GetID())+"("+ " G" + to_string(proOrder->GetID())+") ";
		}
		else
		{
			return;                                         //there is no more available cooks in this timestep 
		}
	}
	
}

bool Restaurant::Health_Emergency(int curr_ts)
{
	Cook* temp;
	float pri_temp;
	Order* tempOrd;
	int no_dishes_left;
	float priority;
	if (busyCooksQ.peekFront(temp, pri_temp) && InServing.peekFront(tempOrd, pri_temp)&&temp->Is_injured()==false)
	{
		busyCooksQ.dequeue(temp, pri_temp);
		InServing.dequeue(tempOrd, pri_temp);
		no_dishes_left = tempOrd->getOrderSize() - (curr_ts - tempOrd->getServTime()) * temp->getSpeed();

			temp->f_speed = (float(temp->getSpeed()) / 2);
			temp->setSpeed(temp->getSpeed() / 2);
		
		int ST = curr_ts - tempOrd->getServTime()+ceil(float(no_dishes_left) / (temp->getSpeed())); //calculate the surving time
		tempOrd->setServInt(ST);
		tempOrd->setFinishTime();
		priority = tempOrd->getFinishTime();
		temp->set_RstTime(tempOrd->getFinishTime());//set the finish of rest time
		temp->injure(true);//make the flag on
		
		busyCooksQ.enqueue(temp, priority);
		
		InServing.enqueue(tempOrd, priority);
		
		injcooksnum++;
		return true;
	}
	else
		return false;


}


void Restaurant::serve_Normal_orders(int CurrentTimeStep)
{
	Order* proOrder;
	NFinfo = "";
	while (LNormal_Order.peek(proOrder))                   //get orders from Normal list to be serve
	{
		Cook* Bcook;
		if (NcooksQ.dequeue(Bcook))                              //check if there is available Normal cook 
		{
			Bcook->setnumofOrderdServed(Bcook->getnumofOrderdServed() + 1);

			int ST = ceil(float(proOrder->getOrderSize()) / Bcook->getSpeed()); //calculate the surving time
			proOrder->setServInt(ST);
			proOrder->Serve(CurrentTimeStep);
			float priority =  (proOrder->getFinishTime());             //set the priority of serving queue with the inverted finished time
			Bcook->assign_Order(proOrder->GetID());
			InServing.enqueue(proOrder, priority);
			busyCooksQ.enqueue(Bcook, priority);                       //enque the cook in priority busy cook queue
			LNormal_Order.DeleteFirst(proOrder);
			
			Nserved++;
			NFinfo += "N" + to_string(Bcook->GetID())+"("+ " N" + to_string(proOrder->GetID())+") ";
		}
		else if (VcooksQ.dequeue(Bcook))           //check if there is available VIP cook when there is no Normal
		{
			Bcook->setnumofOrderdServed(Bcook->getnumofOrderdServed() + 1);

			int ST = ceil(float(proOrder->getOrderSize() )/ Bcook->getSpeed()); //calculate the surving time
			proOrder->setServInt(ST);
			proOrder->Serve(CurrentTimeStep);
			float priority =  (proOrder->getFinishTime());             //set the priority of serving queue with the inverted finished time
			InServing.enqueue(proOrder, priority);
			Bcook->assign_Order(proOrder->GetID());
			busyCooksQ.enqueue(Bcook, priority);                       //enque the cook in priority busy cook queue
			LNormal_Order.DeleteFirst(proOrder);
			
			Nserved++;
			NFinfo += "V" + to_string(Bcook->GetID()) + "(" + " N" + to_string(proOrder->GetID()) + ") ";
		}
		else
		{
			return;                                                   //there is no more available cooks in this timestep 
		}


	}

}

void Restaurant::getfrombusyCookQ(int CurrentTimeStep)
{
	Cook* Acook;
	float priority;
	while (busyCooksQ.peekFront(Acook, priority))
	{
		if ((priority) <= CurrentTimeStep && Acook->Is_injured() == true&&Acook->Has_Urg()==false)
		{
			busyCooksQ.dequeue(Acook, priority);

			if (Acook->getnumofOrderdServed() == Acook->getNumOrdBbreak())
				Acook->setnumofOrderdServed(0);
			CooksInRest.enqueue(Acook);
			
		}
		
		else if((priority) <= CurrentTimeStep && Acook->getnumofOrderdServed() == Acook->getNumOrdBbreak())     //the cook servesed number of orders it should take break
		{
		
			busyCooksQ.dequeue(Acook, priority);
			if (Acook->Is_injured() == true && Acook->Has_Urg() == true)
			{
				Acook->injure(false);         ///if he was injured and was assigned to an urgent cook
				Acook->Give_Urg(false);   ////so its speed is still the half until he has his break
				//if (Acook->f_speed != float(Acook->getSpeed()))
				float original_speed = Acook->f_speed * 2;
				Acook->setSpeed(int(original_speed));
			}
			Acook->setnumofOrderdServed(0);
			float F = (Acook->getBreakDur() + CurrentTimeStep);
			CooksInBreak.enqueue(Acook, F);
		}
		else if (( priority) <= CurrentTimeStep)                      //Finish time equal the current time step
		{
			busyCooksQ.dequeue(Acook, priority);
			if (Acook->GetType() == TYPE_VIP)
				VcooksQ.enqueue(Acook);
			if (Acook->GetType() == TYPE_VGAN)
				GcooksQ.enqueue(Acook);
			if (Acook->GetType() == TYPE_NRM)
				NcooksQ.enqueue(Acook);
		}
		else
		{
			return;                                                     //there is no more finished cooks
		}
	}


}
void Restaurant::getfromBreakCookQ(int CurrentTimeStep)
{
	Cook* Acook;
	float priority;
	while (CooksInBreak.peekFront(Acook, priority))
	{
		if ((priority) <= CurrentTimeStep)               //If there is a cooks finished it's break
		{
		
			CooksInBreak.dequeue(Acook, priority);
			
			if (Acook->GetType() == TYPE_VIP)
				VcooksQ.enqueue(Acook);
			if (Acook->GetType() == TYPE_VGAN)
				GcooksQ.enqueue(Acook);
			if (Acook->GetType() == TYPE_NRM)
				NcooksQ.enqueue(Acook);

		}
		else
		{
			return;                                         //there is no more cooks finished it's break
		}

	}



}
void Restaurant::getfromInServingQ(int CurrentTimeStep)
{
	Order* proOrder;
	float prio;
	while (InServing.peekFront(proOrder, prio))
	{
		if (prio > CurrentTimeStep)
			return;                        //there is no more finished orders

		InServing.dequeue(proOrder, prio);
		proOrder->setStatus(DONE);
		FinishedList.enqueue(proOrder);
	}



}
void Restaurant::getfromRestCookQ(int CurrentTimeStep)
{
	Cook* Rcook;
	while (CooksInRest.peekFront(Rcook))
	{
		
		if ((Rcook->get_rstTime()) <= CurrentTimeStep) //check if there is a cooks finished his rest time
		{
		
				CooksInRest.dequeue(Rcook);
				Rcook->injure(false);
				//if(Rcook->f_speed!=float(Rcook->getSpeed()))
				float original_speed = Rcook->f_speed * 2;
				Rcook->setSpeed(int(original_speed));
				switch (Rcook->GetType())
				{
				case TYPE_VIP:
					VcooksQ.enqueue(Rcook);
					break;
				case TYPE_VGAN:
					GcooksQ.enqueue(Rcook);
					break;
				case TYPE_NRM:
					NcooksQ.enqueue(Rcook);
					break;
				default:
					break;
				}
			
		
		}
		else
		{
			return; //there is no more cooks finished their rest 
		}

	}

}


Restaurant::~Restaurant()
{
		if (pGUI)
			delete pGUI;
}

//Filling GUI with Orders and Cooks of All types

void Restaurant::FillDrawingList()
{
	//Converting The  Cooks Q to Array to iterate on it and add cooks to drawing list
	Cook** pC= NcooksQ.toArray(NCookNum);
	for (int i = 0; i < NCookNum; i++)
	{
		pGUI->AddToDrawingList(pC[i]);
	}
	pC = GcooksQ.toArray(GCookNum);
	for (int i = 0; i < GCookNum; i++)
	{
		pGUI->AddToDrawingList(pC[i]);
	}
	pC = VcooksQ.toArray(VCookNum);
	for (int i = 0; i < VCookNum; i++)
	{
		pGUI->AddToDrawingList(pC[i]);
	}
	/////////////////////////////////////////
	//end of drawing all cooks types
	//////////////////////////////////////
	//Converting The  Orders Q to Array to iterate on it and add ordrs to drawing list
	Order** pON= LNormal_Order.toArray(NWaitNum);
		for (int i = 0; i < NWaitNum;i++)
		 pGUI->AddToDrawingList(pON[i]);

	Order** pOG = Qvegan_Order.toArray(GWaitNum);
	for (int i = 0; i < GWaitNum ;i++)
		pGUI->AddToDrawingList(pOG[i]);

	int usize = 0;
	Order*** PU = QUrgentOrders.toArray(usize);
	for (int i = 0; i < usize; i++)
		pGUI->AddToDrawingList(*PU[i]);

	Order** pOV = QVIP_Order.toArray(VWaitNum);
	for (int i = 0; i < VWaitNum;i++)
	{
		if(!pOV[i]->isUrgent())
		pGUI->AddToDrawingList(pOV[i]);
	}

	int size = 0;
	Order** SO = InServing.toArray(SRVorders);
		for (int i = 0; i < SRVorders;i++)
			pGUI->AddToDrawingList(SO[i]);
		size = 0;
	Order** FD = FinishedList.toArray(size);
		for (int i = 0; i < size;i++)
			pGUI->AddToDrawingList(FD[i]);


}
/////////////////////////////////////////////////////
//Reading inputs from File and filling suitable lists
/////////////////////////////////////////////////////
void Restaurant::fileLoading() 
{
	pGUI->PrintMessage("Enter input file name","Maiandra GD");
	IPfilename = pGUI->GetString();

	ifstream InFile(IPfilename);
	
	if (InFile.is_open())
	{
		

		InFile >> numNcooks >> numGcooks >> numVcooks >> Ncookspeed_min>> Ncookspeed_max>> Gcookspeed_min>> Gcookspeed_max>> Vcookspeed_min>> Vcookspeed_max;

		InFile >> numOrdersBbreak >> Nbreak_min>> Nbreak_max>> Gbreak_min>> Gbreak_max>> Vbreak_min>> Vbreak_max >> InjProp >> RstPrd >> AutoP>> VIP_WT>>numofevents;

		int numAllcooks = numNcooks + numGcooks + numVcooks;
		int* arrCIDs = new int[numAllcooks+1];
		for (int i = 1; i <= numAllcooks; i++)
		{
			arrCIDs[i] = i;
		}
		srand((unsigned)time(0));
		for (int i = 0; i < numNcooks; i++)
		{
			Cook* newNCook = new Cook();
	
			newNCook->setID(arrCIDs[i + 1]);
			newNCook->setType(TYPE_NRM);
			newNCook->setSpeed(rangeRandomAlg2(Ncookspeed_min, Ncookspeed_max));
			
			newNCook->setNumOrdBbreak(numOrdersBbreak);
			newNCook->setBreakDur(rangeRandomAlg2(Nbreak_min, Nbreak_max));
	
			newNCook->set_RstPrd(RstPrd);
			
			newNCook->setnumofOrderdServed(0);

			NcooksQ.enqueue(newNCook);
		}

		for (int i = 0; i < numGcooks; i++)
		{

			Cook* newGCook = new Cook;

			newGCook->setID(arrCIDs[i + numNcooks + 1]);
			newGCook->setType(TYPE_VGAN);
			
			newGCook->setSpeed(rangeRandomAlg2(Gcookspeed_min, Gcookspeed_max));

		
			newGCook->setNumOrdBbreak(numOrdersBbreak);
			
			newGCook->setBreakDur(rangeRandomAlg2(Gbreak_min, Gbreak_max));
			

			newGCook->set_RstPrd(RstPrd);
			
			newGCook->setnumofOrderdServed(0);
			GcooksQ.enqueue(newGCook);
		}

		for (int i = 0; i < numVcooks; i++)
		{
			Cook* newVCook = new Cook;

			newVCook->setID(arrCIDs[i + numNcooks + numGcooks + 1]);
			newVCook->setType(TYPE_VIP);
			
			newVCook->setSpeed(rangeRandomAlg2(Vcookspeed_min, Vcookspeed_max));
		
			newVCook->setNumOrdBbreak(numOrdersBbreak);
			
			newVCook->setBreakDur(rangeRandomAlg2(Vbreak_min, Vbreak_max));
			newVCook->set_RstPrd(RstPrd);
			
			newVCook->setnumofOrderdServed(0);
			VcooksQ.enqueue(newVCook);
		}

		
		for (int i = 0; i < numofevents; i++)
		{
			char typeofevent;
			InFile >> typeofevent;
			Event* pEv;
			if (typeofevent == 'R')
			{
				char ordertype;
				int Ordertype, arrivaltime, ID, Size;
					double Mony;
				InFile >> ordertype>>arrivaltime>> ID>> Size>> Mony;
				if (ordertype == 'N')
				{
					pEv = new ArrivalEvent(arrivaltime, ID, TYPE_NRM, Size, Mony);
					originalNormOrdCount++;
				}
				else if (ordertype == 'G')
					pEv = new ArrivalEvent(arrivaltime, ID, TYPE_VGAN, Size, Mony);
				else if (ordertype == 'V')
					pEv = new ArrivalEvent(arrivaltime, ID, TYPE_VIP, Size, Mony);

			}
			if (typeofevent == 'P')
			{
				int promotiontime, ID;
				double exmony;
				InFile >> promotiontime >> ID >> exmony;
				pEv = new PromotionEvent(promotiontime,ID,exmony);
			}
			if (typeofevent == 'X')
			{
				int cancellationtime, ID;
				InFile >> cancellationtime >> ID;
				pEv = new CancellationEvent(cancellationtime, ID);
				originalNormOrdCount--;

			}

				
			
			EventsQueue.enqueue(pEv);
		}
	}
	
}
void Restaurant::AddtoVIPQueue(Order*& po)
{
	po->setPriority();
	float priority = po->getPriority();
	QVIP_Order.enqueue(po, priority);

}
void Restaurant::AddtoNOList(Order*& po)
{
	LNormal_Order.InsertEnd(po);

}
void Restaurant::AddtoVEQueue(Order*& po)
{
	Qvegan_Order.enqueue(po);

}
void Restaurant::cancellorder(int Id)
{

	Order* del;
	Node<Order*>* prv = LNormal_Order.getHead();
	if (prv&&prv->getItem()->GetID() == Id)
		LNormal_Order.DeleteFirst(del);
	else if(prv) 
	{
		Node<Order*>* Head = prv->getNext();
		while (Head)
		{
			if (Head->getItem()->GetID() == Id)
			{
				if (!Head->getNext())
				{
					LNormal_Order.settail(prv);
				}
				prv->setNext(Head->getNext());
				delete Head;
				break;
			}
			else
			{
				prv = Head;
				Head = Head->getNext();
			}
		}
	}
}


void Restaurant::promoteorder(int Id, double exmoney)
{
	Node<Order*>* prv = LNormal_Order.getHead();
	if (!prv)
		return;
	if (prv->getItem()->GetID() == Id)
	{
		Order* proOrder;
		LNormal_Order.DeleteFirst(proOrder);
	    proOrder->Promote(exmoney);
		float priority = proOrder->getPriority();
		QVIP_Order.enqueue(proOrder, priority);
		return;
		
	}
	else if (prv->getNext())
	{


		while (prv->getNext()->getNext())
		{
			if (prv->getNext()->getItem()->GetID() == Id)
			{
				Order* proOrder;
				Node<Order*>* temp = new Node<Order*>;
				temp = prv->getNext();
				prv->setNext(temp->getNext());
				proOrder = temp->getItem();
				proOrder->Promote(exmoney);
				float priority = proOrder->getPriority();
				QVIP_Order.enqueue(proOrder, priority);
				delete temp;
				return;

			}
			else
				prv = prv->getNext();

		}

		if (prv && prv->getNext() && prv->getNext()->getItem()->GetID() == Id)
		{
			Order* proOrder;
			LNormal_Order.DeleteLast( proOrder);
			proOrder->Promote(exmoney);
			float priority = proOrder->getPriority();
			QVIP_Order.enqueue(proOrder, priority);
			return;


		}
	}
	return; ///if ID isn't in Qnormal
}




void Restaurant::Restaurant_Modes(int Mode)
{
	bool injured;
	srand((int)time(0));
	if (Mode == 1)
	{
		pGUI->PrintMessage("Welcome To Our Restaurant .... Interactive Mode, Click To Continue","Maiandra GD");
		pGUI->waitForClick();
		fileLoading();
		int CurrentTimeStep = 1;
		
		while (!EventsQueue.isEmpty() || !InServing.isEmpty() || !QVIP_Order.isEmpty() || !Qvegan_Order.isEmpty() || !LNormal_Order.isEmpty())
		{
			pGUI->ClearStatusBar();
			char timestep[100];
			itoa(CurrentTimeStep, timestep, 10);
			pGUI->PrintMessage(timestep, 1);
			ExecuteEvents(CurrentTimeStep);
			getfromBreakCookQ(CurrentTimeStep);
			getfrombusyCookQ(CurrentTimeStep);
			getfromInServingQ(CurrentTimeStep);
			
			
			float R= static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			
			if (R <= InjProp)
			{
				injured = Health_Emergency(CurrentTimeStep);
			}
			getfromRestCookQ(CurrentTimeStep);
			
			Serve_Urgent_VIP(CurrentTimeStep);
			Executepromotion(CurrentTimeStep);
			serve_VIP_orders(CurrentTimeStep);
			serve_Vegan_orders(CurrentTimeStep);
			serve_Normal_orders(CurrentTimeStep);
			
			FillDrawingList();
			
			//Printing Cooks and Orders Information
			pGUI->PrintMessage("Wating Orders ->  Normal : " + to_string(NWaitNum) + " Vegan :" + to_string(GWaitNum) + " VIP : " + to_string(VWaitNum), 2);
			pGUI->PrintMessage("Available Cooks - >  Normal : " + to_string(NCookNum) + " Vegan :" + to_string(GCookNum) + " VIP :" + to_string(VCookNum), 3);
			if(!UFinfo.empty()||!VFinfo.empty()||!GFinfo.empty() ||!NFinfo.empty())
				pGUI->PrintMessage(UFinfo+VFinfo +GFinfo+NFinfo, 4);
			else
				pGUI->PrintMessage("No Served Orders", 4);
			pGUI->PrintMessage("Total Served Orders Till Now-> Normal : " + to_string(Nserved) + " Vegan :" + to_string(Gserved) + " VIP :" + to_string(Vserved)+" Urgent :"+to_string(Userved), 5);
			pGUI->UpdateInterface();
			pGUI->waitForClick();
			CurrentTimeStep++;
			pGUI->ResetDrawingList();
		}
		while (!CooksInBreak.isEmpty() || !CooksInRest.isEmpty())
		{
			pGUI->PrintMessage("No More Orders...Please Do More Clicks, Some Cooks are in {Break, Rest}","Maiandra GD");
			getfromBreakCookQ(CurrentTimeStep);
			getfrombusyCookQ(CurrentTimeStep);
			getfromRestCookQ(CurrentTimeStep);
			FillDrawingList();
			pGUI->UpdateInterface();
			pGUI->ResetDrawingList();
			pGUI->waitForClick();
			CurrentTimeStep++;
		}
		outputFileLoading();
	}
	else if (Mode == 2)
	{
		pGUI->PrintMessage("Welcome To Our Restaurant .... Step-by-Step Mode, Click To Continue","Maiandra GD");
		pGUI->waitForClick();
		fileLoading();
		
		int CurrentTimeStep = 1;
		while (!EventsQueue.isEmpty() || !InServing.isEmpty()||!QVIP_Order.isEmpty()|| !Qvegan_Order.isEmpty() ||!LNormal_Order.isEmpty())
		{
			pGUI->ClearStatusBar();
			char timestep[100];
			itoa(CurrentTimeStep, timestep, 10);
			pGUI->PrintMessage(timestep, 1);
			ExecuteEvents(CurrentTimeStep);
			getfromBreakCookQ(CurrentTimeStep);
			getfrombusyCookQ(CurrentTimeStep);
			getfromInServingQ(CurrentTimeStep);
	
			float R = RandomFloat(0.0,1.0);
		
			if (R <= InjProp)
			{
				injured = Health_Emergency(CurrentTimeStep);
				
			}
			
			getfromRestCookQ(CurrentTimeStep);
			
			Serve_Urgent_VIP(CurrentTimeStep);
			Executepromotion(CurrentTimeStep);
			serve_VIP_orders(CurrentTimeStep);
			serve_Vegan_orders(CurrentTimeStep);
			serve_Normal_orders(CurrentTimeStep);
			
			FillDrawingList();
			//Printing Cooks and Orders Information
			pGUI->PrintMessage("Wating Orders ->  Normal : " + to_string(NWaitNum) + " Vegan :" + to_string(GWaitNum) + " VIP : " + to_string(VWaitNum), 2);
			pGUI->PrintMessage("Available Cooks - >  Normal : " + to_string(NCookNum) + " Vegan :" + to_string(GCookNum) + " VIP :" + to_string(VCookNum), 3);
			if (!UFinfo.empty() || !VFinfo.empty() || !GFinfo.empty() || !NFinfo.empty())
				pGUI->PrintMessage(UFinfo + VFinfo + GFinfo + NFinfo, 4);
			else
				pGUI->PrintMessage("No Served Orders", 4);
			pGUI->PrintMessage("Total Served Orders Till Now-> Normal : " + to_string(Nserved) + " Vegan :" + to_string(Gserved) + " VIP :" + to_string(Vserved) + " Urgent :" + to_string(Userved), 5);
			pGUI->UpdateInterface();
			Sleep(1000);
			CurrentTimeStep++;
			pGUI->ResetDrawingList();
		}
		while (!CooksInBreak.isEmpty() || !CooksInRest.isEmpty())
		{
			pGUI->PrintMessage("No More Orders...Please Wait, Some Cooks are in{ Break, Rest }","Maiandra GD");
			getfromBreakCookQ(CurrentTimeStep);
			getfrombusyCookQ(CurrentTimeStep);
			getfromRestCookQ(CurrentTimeStep);
			FillDrawingList();
			pGUI->UpdateInterface();
			pGUI->ResetDrawingList();
			Sleep(1000);
			CurrentTimeStep++;
		}
		outputFileLoading();
	}
	else if (Mode == 3)
	{
		pGUI->PrintMessage("Welcome To Our Restaurant .... Silent Mode, Click To Continue","Maiandra GD");
		pGUI->waitForClick();
		fileLoading();
		int CurrentTimeStep = 1;
		while (!EventsQueue.isEmpty() || !InServing.isEmpty() || !QVIP_Order.isEmpty() || !Qvegan_Order.isEmpty() || !LNormal_Order.isEmpty()|| !CooksInBreak.isEmpty() || !CooksInRest.isEmpty())
		{
			
			ExecuteEvents(CurrentTimeStep);
			getfromBreakCookQ(CurrentTimeStep);
			getfrombusyCookQ(CurrentTimeStep);
			getfromInServingQ(CurrentTimeStep);
			getfromRestCookQ(CurrentTimeStep);
			Serve_Urgent_VIP(CurrentTimeStep);
			
			float R = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			
			if (R <= InjProp)
			{
				
				injured = Health_Emergency(CurrentTimeStep);
			}
			
			
			Executepromotion(CurrentTimeStep);
			serve_VIP_orders(CurrentTimeStep);
			serve_Vegan_orders(CurrentTimeStep);
			serve_Normal_orders(CurrentTimeStep);
			
			
			CurrentTimeStep++;
			
		}

		outputFileLoading();
	}
	pGUI->PrintMessage("End of Simulation....Click to Exit","Maiandra GD");
	pGUI->waitForClick();
}


void Restaurant::outputFileLoading()
{
	pGUI->PrintMessage("Enter output file name","Maiandra GD");
	OPfilename = pGUI->GetString();

	ofstream OutFile(OPfilename);

	float totalwaittime = 0;
	float totalServtime = 0;

	int ordsCount = Nserved + Vserved + Gserved;
	int cooksCount = numNcooks + numGcooks + numVcooks;

	Order** FinishedOrdsArray = FinishedList.toArray(ordsCount);

	if (OutFile.is_open())
	{
		OutFile << "FT    ID    AT    WT    ST" << endl;
	
		for (int i = 0; i < ordsCount; i++)
		{
			for (int j = 0; j < ordsCount - 1; j++)
			{
				if (FinishedOrdsArray[j]->getFinishTime() == FinishedOrdsArray[j + 1]->getFinishTime())
				{
					if (FinishedOrdsArray[j]->getServInt() > FinishedOrdsArray[j + 1]->getServInt())
					{
						Order* temp = FinishedOrdsArray[j];
						FinishedOrdsArray[j] = FinishedOrdsArray[j + 1];
						FinishedOrdsArray[j + 1] = temp;
					}
				}
			}
		}

		for (int i = 0; i < ordsCount; i++)
		{
			OutFile << FinishedOrdsArray[i]->getFinishTime() << "     "
				<< FinishedOrdsArray[i]->GetID() << "     "
				<< FinishedOrdsArray[i]->getArrTime() << "     "
				<< FinishedOrdsArray[i]->getWaitTime() << "     "
				<< FinishedOrdsArray[i]->getServInt() << endl;

			totalwaittime = totalwaittime + FinishedOrdsArray[i]->getWaitTime();
			totalServtime = totalServtime + FinishedOrdsArray[i]->getServInt();
		}

		
		OutFile << "Orders: " << ordsCount << " [Norm:" << Nserved << ", Veg:" << Gserved << ", VIP:" << Vserved << "]" << endl;
		OutFile << "cooks: " << cooksCount << " [Norm:" << numNcooks << ", Veg:" << numGcooks << ", VIP:" << numVcooks
			<< ", injured:" << injcooksnum << "]" << endl;

		if (ordsCount != 0)
		{
			OutFile << "Avg Wait = " << totalwaittime / ordsCount
				<< ", Avg Serv = " << totalServtime / ordsCount << endl;
		}
		OutFile << "Urgent orders: " << UrgentOredersNum;
		if (originalNormOrdCount != 0)
		{
			OutFile<< ",  Auto-promoted: " << (1 - ((originalNormOrdCount - numAutoPromOrders) / originalNormOrdCount)) * 100 << "%";
		}

	}

	for (int i = 0; i < ordsCount; i++)
	{
		delete FinishedOrdsArray[i];
	}


}



