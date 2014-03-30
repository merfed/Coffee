/*
Author:                 Ryan Cornwall
Title:                  Doubly Linked List
Date Created:           4/04/2010 at 3:54 PM
Date Last Modified:     4/28/2010 at 8:36 AM
*/

#ifndef _LIST
#define _LIST

//#include <memory>
#include <algorithm>
#include <iostream>
#include "containerInterface.h"

template <class T>
struct node
{
	T data;
	node<T> *next;
	node<T> *back;
	node(T e){
		data = e;
		next = NULL;
		back = NULL;
	}
};

template <class T>
class list : public containerInterface<T>
{
    public:
        list()
		{
            head = NULL;
        }

        virtual ~list()
        {
            node<T> *p = head;
            while(head) //While head is not NULL
            {
                head = head->next; //Jump head to the next node
                delete p; //Delete the old node
                p = head; //Move the pointer up to where head is
            }
        }

        list(const list& n)
        {
            head = NULL;
            node<T> *p = n.head;
            while(p) //While p isn't NULL
            {
                pushBack(p->data);
                p = p->next;
            }
        }

        list<T>& pushFront(T n)
        {
            node<T> *p = new node<T>(n); //Make a new node
            p->next = head;  //This node comes before head, so its next is head
            if(head) head->back = p; //Prevents NULL assignment
            head = p; //The head is now the new node
            return *this;
        }

        list<T>& pushBack(T n)
        {
            node<T> *p = head;
            while(p && p->next) p = p->next; //While p and p's next aren't NULL
            if(head == NULL) //If list is empty
                head = new node<T>(n);
            else{
                p->next = new node<T>(n);
                p->next->back = p;
                p->next->next = NULL;
            }
            return *this;
        }

        list<T>& popFront(T& n)	throw(BADINDEX)
        {
            if(head) //If the list contains something
            {
                n = head->data;
                node<T> *temp = head;
                head = head->next;
                if(head) head->back = NULL;
                delete temp;
            }
            else throw BADINDEX();
            return *this;
        }

        list<T>& popBack(T& n)	throw(BADINDEX)
        {
            node<T> *p = head;
            while(p && p->next){
                p = p->next;
            }
            if(p)
            {
                n = p->data;
                if(p->back && p != head){
                    node<T> *temp = p; //Points to last node in list
                    p = p->back; //p now points one before last
                    p->next = NULL; //p's next should no longer exist, so NULL
                    delete temp; //Delete the old node that used to be last
                }
                else if(p == head) erase();
            }
            else throw BADINDEX();
            return *this;
        }


        list<T>& popElement(int i) /* Removes element at index i from the list */
        {
            int counter = 0;
            node<T> *p  = head;
            node<T> *pp;
            while(p && p->next && counter < i)
            {
                p = p->next;
                counter++;
            }
            if(p && counter == i) /* If index i was found successfully */
            {
                pp                  = p->back;
                if(p->next && pp)
                {
                    pp->next        = (p->next);
                    (p->next)->back = pp;
                    delete p;
                }
                else if(!pp) /* If index is head */
                {
                    T a; /* Dummy variable */
                    popFront(a);
                }
                else if(!(p->next)) /* If index is back */
                {
                    T a; /* Dummy variable */
                    popBack(a);
                }
            }
            return *this;
        }

        int  getSize()
        {
            if(head)
            {
                int counter=1; //This is intensional.  It should NOT be 0.
                node<T> *p = head;
                while(p && p->next)
                {
                    p = p->next;
                    counter++;
                }
                return counter;
            }
            else return 0;
        }

        bool isEmpty() //Returns true if empty
        {
            if(getSize()) return false;
            else return true;
        }

        bool isFull()
        {
            return false;
        }

        T  front()			throw(BADINDEX)
        {
            if(head)
            {
                return head->data;
            }
            else throw BADINDEX(); //Grr!  The function is mad!  >:(
        }

        T  back()			throw(BADINDEX)
        {
            if(head)
            {
                node<T> *p = head;
                while(p && p->next) p = p->next;
                return p->data;
            }
            else throw BADINDEX();
        }

        T& operator [](int n)	throw(BADINDEX)
        {
            if(n < 0 || n >= getSize()) throw BADINDEX();
            else{
                node<T> *p = head;
                for(int i=0; i < n; i++) p = p->next;
                return p->data;
            }
        }

        int sort(int left=0, int right=-2147483647) //Implementation of quick sort
        {
            int swapsPerformed=0; //Useful in identifying if the list was already sorted
            if(right==-2147483647) right = this->getSize()-1; //Work-around for this->getSize()-1 in arguments
            int i=left, j=right;
            if(i < this->getSize() && j < this->getSize() && this->getSize() > 0){
                T piv = (*this)[(left+right)/2];
            //Partition
                do{
                    if (sizeof(piv) == sizeof(std::string)){ //Tried using decltype, but to no success. Might try C-implementation if possible
                        while(i < right && (*this)[i].compare(piv) < 0) i++;
                        while(j > left  && (*this)[j].compare(piv) > 0) j--;
                    }
                    else{ //Need to know if code ever gets here when it isn't a string
                        while(i < right && (*this)[i] < piv) i++;
                        while(j > left && (*this)[j] > piv) j--;
                    }
                    if(i <= j && i < this->getSize() && j < this->getSize())
                    {
                        swap( (*this)[i],(*this)[j] );
                        i++;
                        j--;
                    }
                }while(i<=j);
            }

            //Recursion
            if(left < j)
                if(i < right)
                    return sort(left, j) + sort(i, right) + swapsPerformed;
                else return sort(left, j) + swapsPerformed;
            else if(i < right)
                if(left < j)
                    return sort(left, j) + sort(i, right) + swapsPerformed;
                else return sort(i, right) + swapsPerformed;
            return swapsPerformed;
        }

        void erase()
        {
            if(head) //If there's work to do, then do it!
            {
                node<T> *p = head;
                while(head)
                {
                    head = head->next;
                    delete p;
                    p = head;
                }
            }
        }

        list<T>& operator= (list& n)
        {
            if(this != &n) //Don't copy yourself, dummy!
            {
                erase();
                node<T> *p = n.head;
                while(p)
                {
                    pushBack(p->data);
                    p = p->next;
                }
            }
            return *this;
        }

    private:
        node<T> *head;
};

#endif
