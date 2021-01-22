#pragma once

#include "..\Defs.h"

#pragma once
class Cook
{
	int ID;
	ORD_TYPE type;	//for each order type there is a corresponding type (VIP, Normal, Vegan)
	int speed;		//dishes it can prepare in one clock tick (in one timestep)
	int ord_assigned;
	bool Is_inj;
	int rst_ts;
	bool has_Urg;
	int RstPrd;
	int numOrdersBefBreak; 
	int breakDuration; 
	int numofOrderdServed;
	
public:
	

	Cook();
	virtual ~Cook();

	int GetID() const;
	ORD_TYPE GetType() const;

	int getSpeed() const;
	int getNumOrdBbreak() const;
	int getBreakDur() const;
	int getnumofOrderdServed() const;
	//////////////////////////////////////////////////

	void setID(int);
	void setType(ORD_TYPE) ;

	void setSpeed(int); 
	void setNumOrdBbreak(int num);
	void setBreakDur(int bd);
	void setnumofOrderdServed(int num);
	
	void set_RstPrd(int r);

	int get_RstPrd();
	void assign_Order(int order);
	int get_order();
	bool Is_injured();
	void injure(bool hurt);
	void set_RstTime(int rst);
	int get_rstTime();
	void Give_Urg(bool);
	bool Has_Urg();
	float f_speed;
};
