#ifndef __WAVE_H__
#define __WAVE_H__

#include <stdlib.h>
#include <list>
#include <vector>
#include "Order.h"

class Wave{
    
    public: 
        // alternate methods
        Wave();
        // adds one order to the top of the list
        void addOrder(Order o);
        // returns the whole list of orders
        std::list<Order> getAllOrders();
        // removes first order from the list and returns it
        Order popOrder();
        // gets the next incomplete order and returns a copy of it, changes it's status to completed
        Order getOrder();
        // sets wave id
        void setId(int id);
        // returns wave id
        int getId();
        // prints the wave
        void printWave();
        // returns true if list is empty, false otherwise
        bool isEmpty();     
        // returns true if the wave has been completed, false otherwise
        bool isCompleted();
        /* changes the value of the completed_ variable         
           checks if all orders are completed, if they are, changes completed to true
        */
        void setCompleted(bool state);  


    private:
        std::list<Order> orders_;
        int waveId_; 
        bool completed_;
};


#endif
