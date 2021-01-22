
#pragma once


/*This code is an updated version from "Data Abstraction & Problem Solving with C++,WALLS AND MIRRORS ,SIXTH EDITION"*/

/*
This is a program that implements the queue abstract data type using a linked list.
The queue is implemented as a chain of linked nodes that has two pointers,
a frontPtr pointer for the front of the queue and a backPtr pointer for the back of the queue.
*/

/*

				The Node: item of type T and a "next" pointer
					-------------
					| item| next | --->
					-------------
General Queue case:

				 frontPtr																backPtr
					\											   						/
					 \											  					   /
					------------- 	  ------------- 	  ------------- 	  -------------
					| item| next |--->| item| next |--->  | item| next |--->  | item| next |---> NULL
					------------- 	  ------------- 	  ------------- 	  -------------

Empty Case:

				 frontptr	 backptr
						\	 /
						 \	/
					---- NULL ------


Single Node Case:
				 frontPtr	 backPtr
					\		/
					 \	   /
					--------
					|	|nxt -->NULL
					--------

*/

#include "PriorityNode.h"

template <typename T>
class PriorityQueue
{
private:

	PriorityNode<T>* backPtr;
	PriorityNode<T>* frontPtr;
public:
	PriorityQueue();
	bool isEmpty() const;
	bool enqueue(const T& newEntry, float& prio);
	bool dequeue(T& frntEntry,float &prio);
	bool peekFront(T& frntEntry,float& prio)  const;
	T* toArray(int& count);	//returns array of T (array if items)
	~PriorityQueue();
};
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: Queue()
The constructor of the Queue class.

*/

template <typename T>
PriorityQueue<T>::PriorityQueue()
{
	backPtr = nullptr;
	frontPtr = nullptr;

}
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: isEmpty
Sees whether this queue is empty.

Input: None.
Output: True if the queue is empty; otherwise false.
*/
template <typename T>
bool PriorityQueue<T>::isEmpty() const
{
	if (frontPtr == nullptr)
		return true;
	else
		return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

/*Function:enqueue
Adds newEntry at the back of this queue.

Input: newEntry .
Output: True if the operation is successful; otherwise false.
*/

template <typename T>
bool PriorityQueue<T>::enqueue(const T& newEntry, float& prio)
{
	PriorityNode<T>* newNodePtr = new PriorityNode<T>(newEntry,prio);
	// Insert the new node
	if (isEmpty())
	{
		frontPtr = newNodePtr; // The queue is empty
		backPtr = newNodePtr;
	}
	else
	{
		// The queue was not empty
		if (newNodePtr->GetPriority() < frontPtr->GetPriority())
		{
			newNodePtr->setNext(frontPtr);
			frontPtr = newNodePtr;
	
		}
		else
		{
			PriorityNode<T>* prev = frontPtr;
			PriorityNode<T>* curr = prev->getNext();
			while (curr)
			{
				if (newNodePtr->GetPriority() < curr->GetPriority())
				{
					prev->setNext(newNodePtr);
					newNodePtr->setNext(curr);
					break;
				}
				else
				{
					prev = curr;
					curr = curr->getNext();
				}
			}
			if (!curr)
			{
				backPtr->setNext(newNodePtr);
				backPtr = newNodePtr;
			}
		}
		
	}
	
	backPtr->setNext(nullptr);
		return true;
	
} // end enqueue


/////////////////////////////////////////////////////////////////////////////////////////////////////////

/*Function: dequeue
Removes the front of this queue. That is, removes the item that was added
earliest.

Input: None.
Output: True if the operation is successful; otherwise false.
*/

template <typename T>
bool PriorityQueue<T>::dequeue(T& frntEntry,float&prio)
{
	if (isEmpty())
		return false;

	PriorityNode<T>* nodeToDeletePtr = frontPtr;
	frntEntry = frontPtr->getItem();
	prio = frontPtr->GetPriority();
	frontPtr = frontPtr->getNext();
	// Queue is not empty; remove front
	if (nodeToDeletePtr == backPtr)	 // Special case: one node in queue
		backPtr = nullptr;

	// Free memory reserved by the dequeued node
	delete nodeToDeletePtr;


	return true;

}

/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: peekFront
gets the front of this queue. The operation does not modify the queue.

Input: None.
Output: The front of the queue.
return: flase if Queue is empty
*/
template <typename T>
bool PriorityQueue<T>::peekFront(T& frntEntry,float& prio) const
{
	if (isEmpty())
		return false;

	frntEntry = frontPtr->getItem();
	prio = frontPtr->GetPriority();
	return true;

}
///////////////////////////////////////////////////////////////////////////////////

template <typename T>
PriorityQueue<T>::~PriorityQueue()
{
}

/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: toArray
returns an array of "T"
Output: count: the length of the returned array (zero if Queue is empty)
returns: The array of T. (nullptr if Queue is empty)
*/

template <typename T>
T* PriorityQueue<T>::toArray(int& count)
{
	count = 0;

	if (!frontPtr)
		return nullptr;
	//counting the no. of items in the Queue
	PriorityNode<T>* p = frontPtr;
	while (p)
	{
		count++;
		p = p->getNext();
	}


	T* Arr = new T[count];
	p = frontPtr;
	for (int i = 0; i < count; i++)
	{
		Arr[i] = p->getItem();
		p = p->getNext();
	}
	return Arr;
}

