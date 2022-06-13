#ifndef __PLANNER_H__
#define __PLANNER_H__

#include <stdlib.h>
#include <list>
#include "Node.h"
#include "Grid.h"
#include "Agent.h"
#include "Utils.h"
#include "HighLevelSolver.h"

class Planner{

    public:
        //expands the current node to all valid subsequent nodes
        std::list<Node*> expand(Node* n);
        //expands the current node to all valid and non-constrainted subsequent nodes
        std::list<Node*> constraintExpand(const std::vector<Constraint> &constraints, Node* parent);
        //calculates heuristic value for current node;
        int heuristicFunc(Grid* g, Node* n);
        //returns the solution used for reaching goal Node n
        std::list<Action> solution(Node* n);
        //returns the list of goals used for reaching Node n. used by LowLevelSolver
        std::vector<Goal> solutionGoals(Node* n);
        //runs astar pathfinding for agent a
        void astar(Agent* a, int goalx, int goaly);
        //sets the grid to plan on
        void setGrid(Grid* g);
        //temporary goal setting;
        void setGoal(int x, int y);
        //goal checking function, returns true if node is a goal node
        bool goalCheck(Node* n, int goalx, int goaly);
        //hash function for priority queue
        unsigned long long int hashFunction(Node* node);   
        //returns cell cost according to movement (lane testing)
        int cellCost(int x, int y, Direction d);
        //runs the CBS algorithm for every agent, using astar method
        std::vector<std::vector<Goal>> solveCBS(std::vector<Agent>& agents);
        //runs astar pathfinding with constraints
        std::vector<Goal> constraintAstar(const std::vector<Constraint> &constraints, Agent* a, int goalx, int goaly);
        //returns whether the parameter moment is a constraint or not
        bool isConstraint(int agentID, int x, int y, int time, const std::vector<Constraint> &constraints);

    private:
        Grid* grid_;
        int goalX_;
        int goalY_;
        int nodesExpanded_;
        
};

#endif 