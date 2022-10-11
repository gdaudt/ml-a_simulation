#include <stdlib.h>
#include <iostream>
#include <ostream>
#include <fstream>
using std::cout;
using std::endl;

#include "Wave.h"

Wave::Wave(){

}

void Wave::addOrder(Order o){
    orders_.emplace_back(o);
}

std::list<Order> Wave::getAllOrders(){
    return orders_;
}

Order Wave::popOrder(){    
    Order o = orders_.front();
    orders_.pop_front();
    return o;
}

Order Wave::getOrder(){
    for(Order order : orders_){
        if(!order.isCompleted()){
            order.setCompleted(true);
            return order;
        }
    }
}

void Wave::setId(int id){
    waveId_ = id;
}

int Wave::getId(){
    return waveId_;
}

void Wave::printWave(){
    cout << "Wave " << waveId_ << ":" << endl;
    for(Order order : orders_){
        order.printOrder();
    }
}

bool Wave::isEmpty(){
    return orders_.empty();
}

bool Wave::isCompleted(){
    if(!orders_.empty())
        return false;
    else
        return true;
}

void Wave::setCompleted(bool state){
    completed_ = state;
}
