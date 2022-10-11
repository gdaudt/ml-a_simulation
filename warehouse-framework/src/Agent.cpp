#include <stdlib.h>
#include <iostream>
#include <ostream>
#include <fstream>
#include <algorithm>

#include "Agent.h"

Agent::Agent(int posx, int posy, int starth, Direction direction){
    //std::cout << "Agent created \n";
    x_ = posx;
    y_ = posy;
    h_ = starth;
    d_ = direction;
}

Agent::Agent(){
    
}

Direction Agent::direction(){
    return d_;
}

int Agent::getX(){
    return x_;
}

int Agent::getY(){
    return y_;
}

int Agent::getH(){
    return h_;
}

int Agent::getId(){
    return id_;
}

std::list<Action> Agent::getPath(){
    return path_;
}

void Agent::setPath(std::list<Action> p){
    for(Action action : p){
        addAction(action);
    }
}

void Agent::setGoals(std::vector<Goal> g){
    coordinateGoals_ = g;
}

void Agent::setX(int posx){
    x_ = posx;
}

void Agent::setY(int posy){
    y_ = posy;
}

void Agent::setPosition(Goal goal){
    x_ = goal.x;
    y_ = goal.y;
}

void Agent::setId(int id){
    id_ = id;
}

void Agent::setDirection(Direction dir){
    d_ = dir;
}

void Agent::printPath(){
    for(Action action : path_){        
        switch(action){
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
}

void Agent::printCpath(){
    int i = 0;
    // std::cout << "Goal: (" << x_ << "," << y_ << ") time: " << 0 << std::endl;
    for(Goal goal : pathCoordinates_){
        std::cout << "Goal: (" << goal.x << "," << goal.y << ")"  << " time: " << i++ << std::endl;
    }
}

void Agent::moveUp(){
    if(d_!=kUp){
        d_ = kUp;
    }
    y_--;
}

void Agent::moveDown(){
    if(d_!=kDown){
        d_ = kDown;
    }
    y_++;
}

void Agent::moveLeft(){
    if(d_!=kLeft){
        d_ = kLeft;
    }
    x_--;
}

void Agent::moveRight(){
    if(d_!=kRight){
        d_ = kRight;
    }
    x_++;
}

void Agent::wait(){
    return;
}

void Agent::addAction(Action a){
    path_.emplace_front(a);
}

void Agent::addPick(Pick goal){
    goals_.emplace_back(goal);
}

Pick Agent::popPick(){
    if(!goals_.empty()){
        Pick pick = goals_.front();
        goals_.pop_front();
        return pick;
    }
}

bool Agent::hasPicks(){
    if(goals_.empty())
        return false;    
    else
        return true;
}

Action Agent::popAction(){
    if(!path_.empty()){
        Action a = path_.front();
        path_.pop_front();
        return a;
    }
}

void Agent::addOrder(Order order){    
    for(Pick pick : order.getPicks()){
        //std::cout << "adding to agent goal (" << pick.x << "," << pick.y << ")\n";
        goals_.emplace_back(pick);
    }
}

void Agent::addPathGoal(Goal goal){
    // std::cout << "emplacing " << goal.x << "," << goal.y << std::endl;
    pathCoordinates_.emplace_back(goal);
}

bool Agent::isFree(){
    return goals_.empty();
}

std::list<Pick> Agent::getPicks(){
    return goals_;
}

std::vector<Goal> Agent::getCoordinateGoals(){
    return coordinateGoals_;
}

void Agent::addGoal(Goal goal){
    coordinateGoals_.push_back(goal);
}

Goal Agent::popGoal(){
    if(!coordinateGoals_.empty()){
        Goal g = coordinateGoals_.back();
        coordinateGoals_.pop_back();
        // g.print();
        return g;
    }
}

Goal Agent::popPathGoal(){
    if(!pathCoordinates_.empty()){
        Goal g = pathCoordinates_.back();
        pathCoordinates_.pop_back();
        return g;
    }
}

void Agent::setCoordinates(std::vector<Goal> goals){
    std::reverse(goals.begin(), goals.end());
    for(auto goal : goals){
        addPathGoal(goal);
    }
}

std::vector<Goal> Agent::getCoordinates(){
    return pathCoordinates_;
}