#ifndef __AGENT_H__
#define __AGENT_H__

#include <stdlib.h>
#include <list>
#include <vector>
#include "Grid.h"
#include "Order.h"
#include "Utils.h"

enum Direction{kUp=0, kDown, kLeft, kRight};
enum Action{kMoveUp=0, kMoveDown, kMoveLeft, kMoveRight, kWait};

class Agent{  

    public:        

        // temporary color parameters
        float red,green,blue;   
        // constructor
        Agent(int x, int y, int h, Direction d);
        // empty constructor
        Agent();
        // returns direction
        Direction direction();
        // sets direction
        void setDirection(Direction d);
        // returns x
        int getX();
        // returns y
        int getY();
        // returns h
        int getH();
        // returns id
        int getId();
        // sets id
        void setId(int id);
        // sets x value
        void setX(int x);
        // sets y value
        void setY(int y);
        // sets both x and y values
        void setPosition(Goal goal);
        // returns agent path
        std::list<Action> getPath();
        // sets agent path
        void setPath(std::list<Action> path);
        // sets agent coordinate goals
        void setGoals(std::vector<Goal> coordinateGoals);
        // set carrying
        void setCarrying(bool carrying);
        // returns carrying
        bool isCarrying();
        // prints path found 
        void printPath();
        // prints coordinate goals found
        void printCpath();
        // moves agent up
        void moveUp();
        // moves agent down
        void moveDown();
        // moves agent left
        void moveLeft();
        // moves agent right
        void moveRight();
        // waits in cell
        void wait();
        // rotates agent up
        void rotateUp();
        // rotates agent down
        void rotateDown();
        // rotates agent left
        void rotateLeft();
        // rotates agent right
        void rotateRight();
        // add action to path
        void addAction(Action a);
        // adds a new goal objetive to the agent
        void addPick(Pick goal);
        // returns an action and removes it from path
        Action popAction();
        // returns a goal and removes it from the goals list
        Pick popPick();
        // returns bool whether goals is empty or not
        bool hasPicks();
        // adds all of the order's picks to the agent
        void addOrder(Order o);
        // returns true if agent has no more goals, false otherwise. purposely redundant with hasGoals, but for better readibility in code
        bool isFree();
        // returns the goals list for the agent
        std::list<Pick> getPicks();
        // returns the next pick on the list for the agent
        Pick getNextPick();
        // returns the coordinateGoals for the agent
        std::vector<Goal> getCoordinateGoals();
        // adds a goal to the coordinateGoals for the agent
        void addGoal(Goal goal);
        //pops a goal from the vector, if available and returns it
        Goal popGoal();
        // pops a goal from the pathCoordinates vector, if available, and returns it
        Goal popPathGoal();
        // adds a goal to the pathCoordinates vector
        void addPathGoal(Goal goal);
        // adds the path Goal to the agent
        void setCoordinates(std::vector<Goal> path);
        // returns the agent's path as a vector
        std::vector<Goal> getCoordinates();
        // the last action to be completed by the agent.
        Pick::PickType next_action;
        Pick::PickType last_action;
        

    private:
        Direction d_;        
        int x_;
        int y_;
        int g_;
        int h_;
        int id_;
        bool carrying_ = false;
        std::list<Action> path_;    
        //Goals listed in Goal object forms
        std::vector<Goal> coordinateGoals_;
        //Goals listed in Pick object forms
        std::list<Pick> goals_;
        //Path listed in Goal struct forms
        std::vector<Goal> pathCoordinates_;
};


#endif