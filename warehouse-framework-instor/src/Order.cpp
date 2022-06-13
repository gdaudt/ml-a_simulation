#include <stdlib.h>
#include <iostream>
#include "Order.h"
using std::cout;
using std::endl;

void Order::addPick(Pick p){
    picks_.emplace_back(p);
}

void Order::printOrder(){
    cout << "Order " << orderId_ << endl << "Picks: ";    
    for(Pick pick : picks_){
        cout << "(" << pick.x << "," << pick.y << ") ";
    }
    cout << endl;
}

Pick Order::popPick(){
    if(!picks_.empty()){
        Pick p = picks_.front();    
        picks_.pop_front();
        return p;
    }   
}

void Order::setId(int id){
    orderId_ = id;
}

int Order::getId(){
    return orderId_;
}

std::list<Pick> Order::getPicks(){
    return picks_;
}

bool Order::isEmpty(){
    return picks_.empty();
}

bool Order::isCompleted(){
    return completed_;
}

void Order::setCompleted(bool state){
    completed_ = state;
}

std::list<Pick> Order::popOrder(){
    std::list<Pick> order;
    for(Pick p : picks_){
        order.emplace_back(p);
        picks_.pop_front();
    }
    return order;
}

std::list<Pick> Order::getOrder(){
    completed_ = true;
    return picks_;
}

