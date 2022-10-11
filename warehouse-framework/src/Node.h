#ifndef __NODE_H__
#define __NODE_H__

#include <stdlib.h>
#include <list>
#include "Agent.h"
#include "iostream"

class Node{       

    public:
        Node();
        // contsructor
        Node(int g, int h, int x, int y, int time, Node* parent, Agent* a);        
        // returns g value of the node
        int getG();
        // returns h value of the node
        int getH();
        // returns x coordinate
        int getX();
        // returns y coordinate
        int getY();
        // returns node id for testing
        int getId();
        // sets the x value
        void setX(int x);
        // sets the y value
        void setY(int y);
        // sets h value
        void setH(int h);
        // sets id value
        void setId(int id);
        // sets the node time
        void setTime(int time);
        // returns action used to reach node
        Action getAction();
        // sets action used to reach node
        void setAction(Action a);
        // returns g+h value of the node
        int getF();
        // returns timestep of the node
        int getTime();
        // returns agent pointer
        Agent* getAgent();
        // returns x of node's agent
        int agentX();
        // returns y of node's agent
        int agentY();
        // prints info of the node
        void print();
        // updates node values if a better one is found (unused for now)
        void update(Node* n);
        // compares current node with parameter
        bool compare(Node* n);
        // returns parent of current node
        Node* getParent();
        // sets n as parent of current node
        void setParent(Node* n);
        //prints action
        void printAction();

    private:
        Agent* agent_;
        int id_;
        int x_;
        int y_;
        int g_;
        int h_;
        int time_;
        Node* parent_;
        // action used to reach the node  
        Action action_;  
};

struct Constraint{
    Goal cell;
    int agentId;
    int time;

    Constraint(int agentId, Goal cell, int time) :
        agentId(agentId), cell(cell), time(time){ }
    
    void print(){
        std::cout << "Constraint: " << "(agent: " << agentId << ") " << "(cell: " << cell.x << "," << cell.y << ") " << "time: " << time << std::endl;
    }
};


#endif