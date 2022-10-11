#ifndef __ORDER_H__
#define __ORDER_H__

#include <stdlib.h>
#include <list>

struct Pick{
    int x;
    int y;
};

class Order{   

    public:        
        // adds a pick in the back of the order
        void addPick(Pick p);
        // prints all picks in the order
        void printOrder();
        // returns a pick from the front and removes it from the list
        Pick popPick();
        // sets the order ID
        void setId(int id);
        // returns the order's id
        int getId();
        // returns all the picks in the order
        std::list<Pick> getPicks();
        // sets the completed state of the order
        void setCompleted(bool state);
        // returns true if order is completed, false otherwise
        bool isCompleted();
        // returns true if order has no more picks, false otherwise
        bool isEmpty();
        // returns the list of picks AND empties the picks from the list
        std::list<Pick> popOrder();
        // returns the list of picks sets the order to completed
        std::list<Pick> getOrder();


    private:
        int orderId_;
        std::list<Pick> picks_;     
        bool completed_;               
};

#endif

