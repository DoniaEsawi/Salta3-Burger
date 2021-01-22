#include "Cook.h"
#include<cmath>

Cook::Cook()
{
	RstPrd = 0;
	Is_inj = false;
	has_Urg =false;
}


Cook::~Cook()
{
}


int Cook::GetID() const
{
	return ID;
}


ORD_TYPE Cook::GetType() const
{
	return type;
}

int Cook::getSpeed() const
{
	return speed;
}

int Cook::getNumOrdBbreak() const
{
	return numOrdersBefBreak;
}

int Cook::getBreakDur() const
{
	return breakDuration;
}


int Cook::getnumofOrderdServed() const
{
	return numofOrderdServed;
}

void Cook::setID(int id)
{
	ID = id;
}

void Cook::setType(ORD_TYPE t)
{
	type = t;
}

void Cook::setSpeed(int s)
{
	if (s >= 1)
		speed = s;
	else
		speed = 1;
}

void Cook::setNumOrdBbreak(int num)
{
	
	numOrdersBefBreak = abs(num);
}

void Cook::setBreakDur(int bd)
{
	breakDuration = abs(bd);
}

void Cook::setnumofOrderdServed(int num)
{
	numofOrderdServed = num;
}


void Cook::set_RstPrd(int r)
{
	if (r >= 0)
		RstPrd = r;

}

int Cook::get_RstPrd()
{
	return RstPrd;
}

void Cook::assign_Order(int order)
{
	ord_assigned = order;
}
int Cook::get_order()
{
	return ord_assigned;
}

void Cook::injure(bool hurt)
{
	Is_inj = hurt;
}

bool Cook::Is_injured()
{
	return Is_inj;
}

void Cook::set_RstTime(int rst)
{
	rst_ts = RstPrd + rst;
}

int Cook::get_rstTime()
{
	return rst_ts;
}

void Cook::Give_Urg(bool urg)
{
	has_Urg = urg;

}
bool Cook::Has_Urg()
{
	return has_Urg;
}