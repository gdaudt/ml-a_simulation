#include <stdlib.h>
#include <iostream>

#include "Node.h"
#include "Grid.h"

Node::Node(){
    
}

Node::Node(int initg, int inith, int px, int py, int initstep, Node* p, Agent* a){ 
    g_ = initg;   
    h_ = inith;
    time_ = initstep;
    x_ = px;
    y_ = py;
    parent_ = p;
    agent_ = a;
    //std::cout << "node created \n";
}

int Node::getG(){
    return g_;
}

int Node::getH(){
    return h_;
}

int Node::getX(){
    return x_;
}

int Node::getY(){
    return y_;
}

int Node::getTime(){
    return time_;
}

int Node::getId(){
    return id_;
}

void Node::setH(int newh){
    h_ = newh;
}

void Node::setId(int i){
    id_ = i;
}

void Node::setTime(int t){
    time_ = t;
}

Node* Node::getParent(){
    return parent_;
}

void Node::setParent(Node* p){
    parent_ = p;
}

Action Node::getAction(){
    return action_;
}

void Node::printAction(){
    switch(action_){
            case kMoveUp:
                 std::cout << "Action: move up" << std::endl; 
                 break;
            case kMoveDown: 
                std::cout << "Action: move down" << std::endl; 
                break;
            case kMoveLeft: 
                std::cout << "Action: move left" << std::endl; 
                break;
            case kMoveRight: 
                std::cout << "Action: move right" << std::endl; 
                break;
            case kWait:
                std::cout << "Action: wait" << std::endl;
                break;
    }
}

void Node::setAction(Action a){
    action_ = a;
}

int Node::getF(){
    return g_+h_;
}

int Node::agentX(){
    return agent_->getX();
}

int Node::agentY(){
    return agent_->getY();
}

Agent* Node::getAgent(){
    return agent_;
}

void Node::setX(int x){
    x_ = x;
}

void Node::setY(int y){
    y_ = y;
}

void Node::print(){
    std::cout << "Node: " << id_ << " (" << x_ << "," << y_ << ") " << " g: " << g_ << " h: " << h_ << " f: " << getF() << " time: " << time_ << std::endl;
}

void Node::update(Node* node){
    g_ = node->getG();
    h_ = node->getH();
    parent_ = node->getParent();
    id_ = node->getId();
    action_ = node->getAction();
    time_ = node->getTime();
}

bool Node::compare(Node* n){
    return n->getX() == x_ and n->getY() == y_;
}

// bool Node::compare(Node* n){
//     if(n->agent_ == agent_){
//         if(n->agent_->getX() == agent_->getX() && n->agent_->getY() == agent_->getY()){
//             return true;
//         /*  if(n->getStep == step_){
//                 return true;
//             }*/
//         }
//     } else
//         return false;
// }