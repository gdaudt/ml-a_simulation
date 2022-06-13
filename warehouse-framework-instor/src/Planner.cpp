#include <stdlib.h>
#include <queue>
#include <map>
#include <unordered_map>
#include <iostream>
#include <string>
#include <algorithm>
#include <chrono>
#include <fstream>

#include "Planner.h"
#include "Node.h"
#include "Grid.h"
#include "Agent.h"

using std::cout;
using std::endl;
using std::string;
using std::to_string;

//local comparator
struct NodeComparator{
    bool operator()(Node* a, Node* b){
        if(a->getF() == b->getF())
            return a->getH() > b->getH();
        else
            return a->getF() > b->getF();
    }
};

bool Planner::isConstraint(int agentID, int x, int y, int time, const std::vector<Constraint> &constraints){
    // std::cout << "checking if (" << x << "," << y << "," << time << ") is a constraint for agent " << agentID << std::endl;
    // std::cout << "constraints size: " << constraints.size() << std::endl;
    for(Constraint c : constraints){
        // c.print();
        if(c.agentId == agentID){
            if(time == c.time and c.cell.x == x and c.cell.y == y){
                // std::cout << "found constraint!" << std::endl;
                return true;
            }
        }
    }
    return false;
}

std::list<Node*> Planner::constraintExpand(const std::vector<Constraint> &constraints, Node* parent){
    //TODO write the isConstraint function to expand valid unconstrainted nodes
    // can copy most of the regular expand function
    std::list<Node*> children;

    // std::cout << "expanding ";
    // parent->print();

    //assigning values to ease code readability
    int parentX = parent->getX();
    int parentY = parent->getY();
    int step = parent->getTime();
    Agent* agent = parent->getAgent();
    Node* child;
    step+=1;

    //expanding up
    if(grid_->isValid(parentX, parentY-1)){
        // std::cout << "is valid: " << grid_->isValid(parentX, parentY-1) << std::endl;
        if(!isConstraint(agent->getId(), parent->getX(), parent->getY()-1, step, constraints)){
            child = new Node(0, 0, parentX, parentY-1, step, parent, agent);
            child->setH(heuristicFunc(grid_, child));
            child->setId(nodesExpanded_++);
            child->setAction(kMoveUp);
            // child->print();
            children.emplace_back(child);
        }        
    }
    // expanding down
    if(grid_->isValid(parentX, parentY+1)){
        // std::cout << "is valid: " << grid_->isValid(parentX, parentY+1) << std::endl;
        if(!isConstraint(agent->getId(), parent->getX(), parent->getY()+1, step, constraints)){
            // child = parent;
            child = new Node(0, 0, parentX, parentY+1, step, parent, agent);
            child->setH(heuristicFunc(grid_, child));
            child->setId(nodesExpanded_++);
            child->setAction(kMoveDown);
            // child->print();
            children.emplace_back(child);
        }        
    }

    //expanding left
    if(grid_->isValid(parentX-1, parentY)){
        // std::cout << "is valid: " << grid_->isValid(parentX-1, parentY) << std::endl;
        if(!isConstraint(agent->getId(), parent->getX()-1, parent->getY(), step, constraints)){
            // child = parent;
            child = new Node(0, 0, parentX-1, parentY, step, parent, agent);
            child->setH(heuristicFunc(grid_, child));
            child->setId(nodesExpanded_++);
            child->setAction(kMoveLeft);
            // child->print();
            children.emplace_back(child);
        }
    }

    //expanding right
    if(grid_->isValid(parentX+1, parentY)){
        // std::cout << "is valid: " << grid_->isValid(parentX+1, parentY) << std::endl;
        if(!isConstraint(agent->getId(), parent->getX()+1, parent->getY(), step, constraints)){
            // child = parent;
            child = new Node(0, 0, parentX+1, parentY, step, parent, agent);
            child->setH(heuristicFunc(grid_, child));
            child->setId(nodesExpanded_++);
            child->setAction(kMoveRight);
            // child->print();
            children.emplace_back(child);
        }
    }

    //expanding wait
    if(grid_->isValid(parentX, parentY)){
        // std::cout << "is valid: " << grid_->isValid(parentX, parentY) << std::endl;
        if(!isConstraint(agent->getId(), parent->getX(), parent->getY(), step, constraints)){
            // child = parent;
            child = new Node(0, 0, parentX, parentY, step, parent, agent);
            child->setH(heuristicFunc(grid_, child));
            child->setId(nodesExpanded_++);
            child->setAction(kWait);
            // child->print();
            children.emplace_back(child);
        }
    }

    nodesExpanded_++;
    return children;
}

std::list<Node*> Planner::expand(Node* parent){
    std::list<Node*> children;

    // std::cout << "expanding node (" << parent->getX() << "," << parent->getY() << ") h:" << parent->getH() <<
    // " g:" << parent->getG() << " f:" << parent->getF() << " step:" << parent->getStep() << endl;
    // parent->printAction();
    // std::cout << endl;

    //assigning values to ease code readability
    int parentX = parent->getX();
    int parentY = parent->getY();
    int step = parent->getTime();
    Agent* agent = parent->getAgent();
    int cost = 0;

    //checking for UP movement
    if(grid_->isValid(parentX, parentY-1)){
        // if(grid_->isValidDynamic(parentX, parentY-1, step+1)){
            cost = parent->getG() + cellCost(parentX, parentY-1, kUp);
            Node* child = new Node(cost, 0, parentX, parentY-1, step+1, parent, agent);
            child->setH(heuristicFunc(grid_, child));
            child->setAction(kMoveUp);
            //id setting for testing
            child->setId(nodesExpanded_++);
            children.emplace_back(child);
        // }
        // cost = parent->getG() + cellCost(parentX, parentY-1, kUp);
        // Node* child = new Node(cost, 0, parentX, parentY-1, step+1, parent, agent);
        // child->setH(heuristicFunc(grid_, child));
        // child->setAction(kMoveUp);
        // //id setting for testing
        // child->setId(nodesExpanded_++);
        // children.emplace_back(child);
    }
    //checking for DOWN movement
    if(grid_->isValid(parentX, parentY+1)){
        // if(grid_->isValidDynamic(parentX, parentY+1, step+1)){
            cost = parent->getG() + cellCost(parentX, parentY+1, kDown);
            Node* child = new Node(cost, 0, parentX, parentY+1, step+1, parent, agent);
            child->setH(heuristicFunc(grid_, child));
            child->setAction(kMoveDown);
            //id setting for testing
            child->setId(nodesExpanded_++);
            children.emplace_back(child);
        // }
        // cost = parent->getG() + cellCost(parentX, parentY+1, kDown);
        // Node* child = new Node(cost, 0, parentX, parentY+1, step+1, parent, agent);
        // child->setH(heuristicFunc(grid_, child));
        // child->setAction(kMoveDown);
        // //id setting for testing
        // child->setId(nodesExpanded_++);
        // children.emplace_back(child);
    }
    //checking for LEFT movement
    if(grid_->isValid(parentX-1, parentY)){
        // if(grid_->isValidDynamic(parentX-1, parentY, step+1)){
          cost = parent->getG() + cellCost(parentX, parentY-1, kUp);
            Node* child = new Node(cost-2, 0, parentX-1, parentY, step+1, parent, agent);
            child->setH(heuristicFunc(grid_, child));
            child->setAction(kMoveLeft);
            //id setting for testing
            child->setId(nodesExpanded_++);
            children.emplace_back(child);
        // }
        // cost = parent->getG() + cellCost(parentX, parentY-1, kUp);
        // Node* child = new Node(cost, 0, parentX-1, parentY, step+1, parent, agent);
        // child->setH(heuristicFunc(grid_, child));
        // child->setAction(kMoveLeft);
        // //id setting for testing
        // child->setId(nodesExpanded_++);
        // children.emplace_back(child);
    }
    //checking for RIGHT movement
    if(grid_->isValid(parentX+1, parentY)){
        // if(grid_->isValidDynamic(parentX+1, parentY, step+1)){
            cost = parent->getG() + cellCost(parentX, parentY-1, kUp);
            Node* child = new Node(cost-2, 0, parentX+1, parentY, step+1, parent, agent);
            child->setH(heuristicFunc(grid_, child));
            child->setAction(kMoveRight);
            //id setting for testing
            child->setId(nodesExpanded_++);
            children.emplace_back(child);
        // }
        // cost = parent->getG() + cellCost(parentX, parentY-1, kUp);
        // Node* child = new Node(cost, 0, parentX+1, parentY, step+1, parent, agent);
        // child->setH(heuristicFunc(grid_, child));
        // child->setAction(kMoveRight);
        // //id setting for testing
        // child->setId(nodesExpanded_++);
        // children.emplace_back(child);
    }
    //accounting for WAIT
    // if(grid_->isValidDynamic(parentX+1, parentY, step+1)){
    //     cost = parent->getG() + 2;
    //     Node* child = new Node(cost, 0, parentX, parentY, step+1, parent, agent);
    //     child->setH(parent->getH());
    //     child->setAction(kWait);
    //     child->setId(nodesExpanded_++);
    //     children.emplace_back(child);
    // }
    nodesExpanded_++;

    return children;
}

//using manhattan distance for heuristic value
//maybe adding the step cost to heuristic to manipulate collisions
int Planner::heuristicFunc(Grid* g, Node* n){
    int heuristic = 0;
    if(grid_->isValid(n->getX(), n->getY())){
        int xdis = abs(goalX_ - n->getX());
        int ydis = abs(goalY_ - n->getY());
        heuristic = xdis+ydis;
    }
    else
        heuristic = 9999;
    return heuristic;
}

std::list<Action> Planner::solution(Node* goal){
    std::list<Action> solution;
    Node* current = goal;
    int offsetX = grid_->getMapWidth() / 2;
    int offsetY = grid_->getMapHeight() / 2;
    int wait = 5;
    int x,y;    
    while(current->getParent()!=NULL){
        // std::cout << "current: " << current->getX() << "," << current->getY();
        solution.emplace_back(current->getAction());
        // std::cout << " Node id:" << current->getId() << " (" << current->getX() << "," << current->getY() << ") ";
        // current->printAction();
        //grid_->getCell(current->getX(), current->getY())->isUsed = true;
        // x = current->getX();
        // y = current->getY();
        // grid_->getCell(current->getX(), current->getY())->lastOccupied = current->getStep();
        // cout << "Occupying cell (" << x << "," << y << ") at step " << current->getStep() << endl;
        //cout << "Adjusted X, Y: " << (current->getX() - offsetX) * 0.4 << "," << (current->getY() - offsetY) * 0.4 << endl;
        current = current->getParent();

    };
    //temporary forced wait after goal reaching
    for(int i = 0; i<wait; i++)
        solution.emplace_back(kWait);
    return solution;
}



std::vector<Goal> Planner::solutionGoals(Node* goal){
    std::vector<Goal> solution;
    Node* current = goal;
    Goal g;
    int offsetX = grid_->getMapWidth() / 2;
    int offsetY = grid_->getMapHeight() / 2;
    int wait = 5;
    int x,y;
    while(current->getParent()!=NULL){
        // std::cout << "current: " << current->getX() << "," << current->getY();
        g.x = current->getX();
        g.y = current->getY();
        solution.emplace_back(g);
        //std::cout << " Node id:" << current->getId() << " (" << current->getX() << "," << current->getY() << ") ";
        // current->printAction();
        //grid_->getCell(current->getX(), current->getY())->isUsed = true;
        // x = current->getX();
        // y = current->getY();
        // grid_->getCell(current->getX(), current->getY())->lastOccupied = current->getStep();
        // cout << "Occupying cell (" << x << "," << y << ") at step " << current->getStep() << endl;
        // cout << "Adjusted X, Y: " << (current->getX() - offsetX) * 0.4 << "," << (current->getY() - offsetY) * 0.4 << endl;
        // std::cout << "Path: (" << current->getX() << "," << current->getY() << ") ";
        current = current->getParent();
    };
    g.x = current->getX();
    g.y = current->getY();
    solution.emplace_back(g);
    // std::cout << "Path: (" << current->getX() << "," << current->getY() << ") " << std::endl;
    std::reverse(solution.begin(), solution.end());    
    return solution;
}

std::vector<Goal> Planner::constraintAstar(const std::vector<Constraint> &constraints, Agent* agent, int goalx, int goaly){
    // std::cout << "start constraint planning" << std::endl;
    Node* start = new Node(0, 0, agent->getX(), agent->getY(), 0, NULL, agent);
    setGoal(goalx, goaly);
    // std::cout << "goal (" << goalx << "," << goaly << ")" << std::endl;
    // id setting for testing
    start->setId(nodesExpanded_);
    start->setH(heuristicFunc(grid_, start));
    //create priority queue
    std::priority_queue<Node*, std::vector<Node*>, NodeComparator> queue;
    //create map
    std::unordered_map<long long int, Node*> map;
    //create successor list
    std::list<Node*> children;
    //push to queue
    queue.push(start);
    map[hashFunction(start)] = start;

    while(!queue.empty()){
        Node* current = queue.top();
        queue.pop();
        children = constraintExpand(constraints, current);
        for(Node* child : children){
            // child->print();
            //test goal
            if(goalCheck(child, goalx, goaly)){
                // std::cout << "goal found " << agent->getId() << std::endl;
                agent->setPath(solution(child));
                agent->setCoordinates(solutionGoals(child));
                // agent->printCpath();
                // agent->printPath();
                return solutionGoals(child);
            }
            unsigned long long int key = hashFunction(child);
            // std::cout << "key: " << key << std::endl;
            //check map
            //if not in map, add to queue
            if(map.find(key) == map.end()){
                // child->print();
                map[key] = child;
                child->setParent(current);
                queue.push(child);
            }
            else if(child->getF() < map[key]->getF()){
                Node* swap = map[key];
                swap->update(child);
                map[key] = swap;
                // swap->print();
            }
        }
    }
}

void Planner::astar(Agent* agent, int goalx, int goaly){
    //nodes_expanded = 0;
    std::cout << "start planning \n";
    Node* start = new Node(0, 0, agent->getX(), agent->getY(), 0, NULL, agent);
    setGoal(goalx, goaly);
    // id setting for testing
    start->setId(nodesExpanded_);
    start->setH(heuristicFunc(grid_, start));
    //create priority queue
    std::priority_queue<Node*, std::vector<Node*>, NodeComparator> queue;
    //create map
    std::unordered_map<long long int, Node*> map;
    //push to queue
    queue.push(start);
    map[hashFunction(start)] = start;
    //while queue is not empty
    while(!queue.empty()){
        Node* current = queue.top();
        queue.pop();
        std::list<Node*> children = expand(current);
        //grid_->getCell(current->getX(), current->getY())->isExpanded = true;
        for(Node* child : children){
            //test goal
            if(goalCheck(child, goalx, goaly)){
                std::cout << "goal found \n";
                agent->setPath(solution(child));
                // agent->setGoals(solutionGoals(child));
                agent->printPath();
                return;
            }
            unsigned long long int key = hashFunction(child);
            // cout << "child hash:" << key << endl;
            //check map
            //if not in map, add to queue
            if(map.find(key) == map.end()){
                // std::cout << "adding to queue:\n (" << child->getX() << "," << child->getY() << ") h:" << child->getH() << " g:" << child->getG()
                // << " f:" << child->getF() << " step:" << child->getStep() << " ";
                // child->printAction();
                // std::cout << "\n";
                map[key] = child;
                child->setParent(current);
                queue.push(child);
            }
            else if(child->getF() < map[key]->getF()){
                Node* swap = map[key];
                swap->update(child);
                map[key] = swap;
            }
        }
    }
}

void Planner::setGrid(Grid* g){
    grid_ = g;
}

void Planner::setGoal(int x, int y){
    goalX_ = x;
    goalY_ = y;
}

//local method for goal checking
bool Planner::goalCheck(Node* n, int goalX_, int goalY_){
    if(n->getX() == goalX_ && n->getY() == goalY_)
        return true;
    else
        return false;
}

//local hash function
// need to do something to account for step, to know they aren't the same
unsigned long long int Planner::hashFunction(Node* node){
    unsigned long long int hash = 0;
    hash = ( (node->getY() << 16 ) ^ node->getX());
    // string s1 = to_string(node->getX());
    // string s2 = to_string(node->getY());
    // string s3 = to_string(node->getStep());
    // string s = s1+s2+s3;
    // hash = std::stoi(s);
    return hash;
}

//values need more testing, for now it favors correctly up/down lanes
int Planner::cellCost(int x, int y, Direction dir){
    int cost = 2;
    //std::cout << "initial cost:" << cost << "\n";
    if(dir == kDown or dir == kUp){
        cost+=1;    
    }
    //std::cout << "returning cost: " << cost << "\n";
    return cost;
}

void printSolution(std::vector<std::vector<Goal>> optimalPaths) {
    int i = 0;
    int j = 0;
    float mapCenterX = 14;
    float mapCenterY = 11;
    float scale = 0.4;
    std::ofstream path_file;
    path_file.open("/home/gdaudt/path_test/paths.txt", std::ios::in | std::ios::out | std::ios::app);   
	for (auto path : optimalPaths) {
		std::cout << "Optimal path of agent " << i << ": " << std::endl;
        path_file << "([";
		for (auto cell : path) {
			std::cout << "(" << cell.x << ", " << cell.y << ") ";
            if(j != 0)
                path_file << "[" << to_string((cell.x - mapCenterX) * scale) << "," << to_string((mapCenterY - cell.y) * scale) << "]";
            if(j != (path.size() - 1) and j != 0)
                path_file << ",";
            j++;
		}
        std::cout << std::endl;
        path_file << "])" << std::endl;        
        i++;
        j = 0;
	}
    path_file.close();
}

std::vector<std::vector<Goal>> Planner::solveCBS(std::vector<Agent>& agents){
    cout << "starting cbs" << endl;
    auto started = std::chrono::high_resolution_clock::now();

    std::vector<std::vector<Goal>> optimalPaths;
    HighLevelSolver solver;
    optimalPaths = solver.solve(agents, grid_);
    // std::cout << optimalPaths.size() << std::endl;
    printSolution(optimalPaths);

    auto done = std::chrono::high_resolution_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count();
    std::cout << "Planning time: " << elapsedTime << " ms" << std::endl;
    return optimalPaths;
}
