#pragma once
#include "Node.h"
template <class T>

//This Data Structure Depends On LIFO

class Stack
{
private:
	Node<T>* Top;  //A pointer that points to last element entered stack
public:
	Stack();
	bool Push(T element); //Pushing an element to stack
	bool isEmpty(); //Checking if stack empty or not
	bool pop(); // This Function Removes The Last Element in Stack
	bool getTop(T& stacktop); //Get Last Elemnt Enterd the Stack
	~Stack();
};

////////////////////////////////////////

//constuctor of Stack
template <class T>
Stack<T>::Stack()
{
	Top = nullptr;
}

template <class T>
bool Stack<T>::Push(T element)
{
	Node<T>* newnode = new Node<T>;
	if (newnode == nullptr)
		return false;
	newnode->setItem(element);
	newnode->setNext(Top);
	Top = newnode;
	return true;
}

template <class T>
bool Stack<T> ::isEmpty()
{
	return Top == nullptr;
}

template <class T>
bool Stack<T> ::pop()
{
	if (isEmpty())
	{
		return false;
	}
	else
	{
		Node<T>* temp = Top;  //temporary pointer to remove last element
		Top = Top->getNext();
		temp->setNext(nullptr);
		delete temp;
		return true;
	}
}

template <class T>
bool Stack<T> ::getTop(T& stacktop)
{
	if (isEmpty())
		return false;
	else
	{
		stacktop = Top->getItem();
		return true;
	}
}

template <class T>
Stack<T> ::~Stack()
{
}