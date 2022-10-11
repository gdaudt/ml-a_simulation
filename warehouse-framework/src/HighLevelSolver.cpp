#include "HighLevelSolver.h"
#include <list>
#include <queue>
#include <algorithm>
#include <climits>
#include <iostream>

HighLevelSolver::HighLevelSolver() = default;
HighLevelSolver::~HighLevelSolver() = default;

// Local TreeNode comparator for priority queue
struct TreeNodeComparator {
    bool operator()(const TreeNode& a, const TreeNode& b) const {
        return a.getCost() > b.getCost();
    }
};

// Returns true if there is a conflict between two given routes
bool HighLevelSolver::hasConflict(const std::vector<Goal>& route1, const std::vector<Goal>& route2){
    auto min_index = std::min(route1.size(), route2.size());
    // std::cout << "regular conflict check: " << std::endl;
    for(int i = 0; i < min_index; i++){
        // print both routes for debugging        
        if(route1[i] == route2[i]){
            std::cout << "regular conflict found!" << std::endl;
            std::cout << "Route 1: " << route1[i].x << "," << route1[i].y << " time: " << i+1 << std::endl;
            std::cout << "Route 2: " << route2[i].x << "," << route2[i].y << " time: " << i+1 << std::endl;
            return true;
        }            
    }
    return false;
}

bool HighLevelSolver::hasConflict(const TreeNode& treeNode){
    auto solutions = treeNode.getSolution();
    for(int i = 0; i < solutions.size(); i++){
        for (int j = i+1; j < solutions.size(); j++){
            if (hasConflict(solutions[i], solutions[j])){
                std::cout << "conflict found between agent " << i << " and agent " << j << std::endl;
                return true;
            }
        }
    }
    return false;
}

bool HighLevelSolver::hasEdgeConflict(const std::vector<Goal>& route1, const std::vector<Goal>& route2){
    auto min_route_size = std::min(route1.size(), route2.size()) - 1;
    for(int i = 0; i < min_route_size; i++){        
        if(route1[i] == route2[i+1] and route1[i+1] == route2[i]){
            std::cout << "edge conflict found!" << std::endl;
            // std::cout << "Route 1: " << route1[i].x << "," << route1[i].y << " time: " << i+1 << std::endl;
            // std::cout << "Route 2+1: " << route2[i+1].x << "," << route2[i+1].y << " time: " << i+2 << std::endl;
            // std::cout << "Route 1+1: " << route1[i+1].x << "," << route1[i+1].y << " time: " << i+2 << std::endl;
            // std::cout << "Route 2: " << route2[i].x << "," << route2[i].y << " time: " << i+1 << std::endl;
            return true;
        }       
    }
    return false;
}

int HighLevelSolver::getEdgeConflict(const std::vector<Goal>& route1, const std::vector<Goal>& route2){
    auto min_route_size = std::min(route1.size(), route2.size()) - 1;
    for(int i = 0; i < min_route_size; i++){        
        if(route1[i] == route2[i+1] and route1[i+1] == route2[i]){
            // std::cout << "edge conflict found!" << std::endl;
            // std::cout << "Route 1: " << route1[i].x << "," << route1[i].y << " time: " << i+1 << std::endl;
            // std::cout << "Route 2+1: " << route2[i+1].x << "," << route2[i+1].y << " time: " << i+2 << std::endl;
            // std::cout << "Route 1+1: " << route1[i+1].x << "," << route1[i+1].y << " time: " << i+2 << std::endl;
            // std::cout << "Route 2: " << route2[i].x << "," << route2[i].y << " time: " << i+1 << std::endl;
            return i+1;
        }       
    }
    return -1;
}

// method receives two routes that are pointers to goal vectors and both ids from the agents involved in the routes
// tests for edge conflicts between the routes
// returns two conflicts, one with the first agent, first cell and time and one with the second agent, second cell and time

bool HighLevelSolver::hasEdgeConflict(const TreeNode& treeNode){
    auto solutions = treeNode.getSolution();
    for (int i = 0; i < solutions.size(); i++){
        for(int j = i+1; j < solutions.size(); j++){        
            if(hasEdgeConflict(solutions[i], solutions[j])){
                std::cout << "edge conflict between agent " << i << " and agent " << j << std::endl;
                return true;
            }
        }
    }
    return false;
}

Conflict HighLevelSolver::getEdgeConflict(const TreeNode& treeNode){
    auto solutions = treeNode.getSolution();
    for (int i = 0; i < solutions.size(); i++){
        for(int j = i+1; j < solutions.size(); j++){
            int conflict_index = getEdgeConflict(solutions[i], solutions[j]);
            if(conflict_index != -1){
                // std::cout << "edge conflict between agent " << i << " and agent " << j << std::endl;
                auto conflict = Conflict(i, j, solutions[i][conflict_index], solutions[j][conflict_index], conflict_index);
                // std::cout << "conflict size: " << sizeof(conflict) << std::endl;
                return conflict;
            }
        }
    }
    return Conflict();
}

Conflict HighLevelSolver::getFirstConflict(const TreeNode& treeNode){
    auto solutions = treeNode.getSolution();
    std::cout << "getting first conflict" << std::endl;
    for(int i = 0; i < solutions.size(); i++){
        for(int j = i+1; j < solutions.size(); j++){
            auto min_index = std::min(solutions[i].size(), solutions[j].size());
            // std::cout << "solution i size: " << solutions[i].size() << std::endl;
            // std::cout << "solution j size: " << solutions[j].size() << std::endl;
            for(int k = 0; k < min_index; k++){
                // std::cout << "k value: " << k << std::endl;
                if(solutions[i][k] == solutions[j][k]){
                    // std::cout << "agent: " << i << ", agent: " << j << " time: " << k << std::endl;
                    return Conflict(i, j, solutions[i][k], solutions[j][k], k);
                }                    
            }
        }
    }
}

// Returns min cost on tree
int HighLevelSolver::getMinCost(const std::vector<TreeNode> &tree) {
	int min = INT_MAX;

	for (const auto &node : tree) {
		if (node.getCost() < min)
			min = node.getCost();
	}
	return min;
}

// Returns first node with minCost
TreeNode HighLevelSolver::findBestNode(const std::vector<TreeNode> &tree) {
	auto minCost = getMinCost(tree);
	for (const auto &node : tree) {
		if (node.getCost() == minCost)
			return node;
	}
	return TreeNode();
}

inline bool HighLevelSolver::isEmpty(const std::vector<TreeNode> &tree) {
	return tree.empty();
}

std::vector<std::vector<Goal>> HighLevelSolver::solve(std::vector<Agent>& agents, Grid* grid){

    std::cout << "starting high level solver" << std::endl;
    std::vector<TreeNode> tree;
    std::priority_queue<TreeNode, std::vector<TreeNode>, TreeNodeComparator> treeQueue;

    auto root = TreeNode();
    root.updateSolution(agents, grid);
    // std::cout << "updated solution" << std::endl;
    root.updateCost();

    tree.emplace_back(root);
    treeQueue.emplace(root);

    while(!treeQueue.empty()){
        TreeNode P;
        TreeNode treeNode = treeQueue.top();
        // treeNode.print();
        // P = findBestNode(tree);     
        auto conflict = getEdgeConflict(treeNode);        
        if(!hasConflict(treeNode) and conflict.conflictedAgentsID.first == -1){
            std::cout << "No more conflicts found" << std::endl;
            return treeNode.getSolution();
        }
        // NEED TO FIX FLAWED ELSE LOGIC
        else if(hasConflict(treeNode)){
            // std::cout << "conflict between paths found" << std::endl;
            auto conflict = getFirstConflict(treeNode);
            // conflict.print();

            // std::cout << "popping node from tree" << std::endl;
            treeQueue.pop();
            for (int i = 0; i < 2; i++){
                // Initialize new node with current nodes constraints
                TreeNode A(treeNode.getConstraints());

                auto newConstraint = Constraint(conflict.conflictedAgentsID.first, conflict.cell1, conflict.time);

                if(i == 1){
                    newConstraint = Constraint(conflict.conflictedAgentsID.second, conflict.cell2, conflict.time);
                }

                A.addConstraint(newConstraint);
                // std::cout << "added constraint" << std::endl;
                // newConstraint.print();
                A.updateSolution(agents, grid);
                A.updateCost();
                
                // if solution was found, push it to the tree
                if(A.getCost() < INT_MAX){
                    // std::cout << "pushing new node into tree 1" << std::endl;
                    // A.printConstraints();
                    treeQueue.emplace(A);
                }
            }
        }
        // NEED TO FIX FLAWED EDGE CONFLICT GENERATION
        else{              
            std::cout << "edge conflict between paths found" << std::endl;
            // conflict.print();
            treeQueue.pop();
            for(int i = 0; i < 2; i++){
                TreeNode A(treeNode.getConstraints());
                // std::cout << "entered for loop" << std::endl;
                auto newConstraint = Constraint(conflict.conflictedAgentsID.first, conflict.cell1, conflict.time );
                // newConstraint.print();
                auto constraint2 = Constraint(conflict.conflictedAgentsID.first, conflict.cell2, conflict.time+1);
                // constraint2.print();
                if(i==1){
                    auto newConstraint = Constraint(conflict.conflictedAgentsID.second, conflict.cell2, conflict.time);
                    // newConstraint.print();
                    auto constraint2 = Constraint(conflict.conflictedAgentsID.second, conflict.cell1, conflict.time+1);
                    // constraint2.print();
                }

                // add new constraints, solve with low level and update
                A.addConstraint(newConstraint);
                A.addConstraint(constraint2);
                // std::cout << "adding constraint ";
                // newConstraint.print();
                // std::cout << " and ";
                // constraint2.print();
                A.updateSolution(agents, grid);
                A.updateCost();

                // if a solution was found, push it to the treeNode
                if(A.getCost() < INT_MAX){
                    // std::cout << "pushing new node into tree 2" << std::endl;
                    treeQueue.emplace(A);
                }
            }             
        }
    }
    return std::vector<std::vector<Goal>>();    
}





