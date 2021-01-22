
#pragma once


template < typename T>
class PriorityNode
{
private:
	T item;// A data item

	float priority;//priority of data to insert in the queue
	PriorityNode<T>* next; // Pointer to next node
public:
	PriorityNode();
	PriorityNode(const T& r_Item,float& pri);	//passing by const ref.
	PriorityNode(const T& r_Item,float& pri, PriorityNode<T>* nextNodePtr);
	void setItem(const T& r_Item);
	void setNext(PriorityNode<T>* nextNodePtr);
	void setPriority(float& priority);
	float GetPriority()const;
	T getItem() const;
	PriorityNode<T>* getNext() const;
}; // end p. Node


template < typename T>
PriorityNode<T>::PriorityNode()
{
	next = nullptr;
	priority = 0;
}

template < typename T>
PriorityNode<T>::PriorityNode(const T& r_Item,float& pri)
{
	priority = pri;
	item = r_Item;
	next = nullptr;
}

template < typename T>
PriorityNode<T>::PriorityNode(const T& r_Item, float& pri, PriorityNode<T>* nextNodePtr)
{
	setPriority(pri);
	item = r_Item;
	next = nextNodePtr;
}
template < typename T>
void PriorityNode<T>::setItem(const T& r_Item)
{
	item = r_Item;
}
template < typename T>
void PriorityNode<T>::setPriority(float& pri)
{
	if(pri>=0)
		priority = pri;
}
template < typename T>
void PriorityNode<T>::setNext(PriorityNode<T>* nextNodePtr)
{
	next = nextNodePtr;
}

template < typename T>
T PriorityNode<T>::getItem() const
{
	return item;
}
template < typename T>
float  PriorityNode<T>::GetPriority() const
{
	return priority;
}
template < typename T>
PriorityNode<T>* PriorityNode<T>::getNext() const
{
	return next;
}

