#include "TreeNode.h"

TreeNode::TreeNode() = default;
TreeNode::~TreeNode() = default;

std::vector<std::vector<Goal>> TreeNode::getSolution() const{
    // printSolution();
    return solution;
}

int TreeNode::getCost() const{
    return cost;
}

std::vector<Constraint> TreeNode::getConstraints(){
    return this->constraints;
}

TreeNode::TreeNode(const std::vector<Constraint>& constraints){
    // std::cout << "Creating new node with constraints: " << std::endl;
    this->constraints = constraints;
    // printConstraints();
}

void TreeNode::addConstraint(const Constraint& constraint){
    constraints.emplace_back(constraint);
}

void TreeNode::updateSolution(std::vector<Agent>& agents, Grid* grid){
    LowLevelSolver solver;
    std::cout << "starting low level solver" << std::endl;
    // printConstraints();
    solution = solver.findOptimalPaths(constraints, agents, grid);
}

void TreeNode::updateCost(){
    auto cost = 0;
    for(auto route : solution){
        cost += route.size();
    }
    this->cost = cost;
    std::cout << "Tree node cost: " << cost << std::endl;
}

void TreeNode::printConstraints() const{
    std::cout << "ALL Constraints: " << std::endl;
    for(auto constraint : constraints){
        constraint.print();
    }
}

void TreeNode::printSolution() const{
    std::cout << "Solution: " << std::endl;
    int i = 0;
    for(auto route : solution){
        std::cout << "Agent: " << i << std::endl;
        std::cout << "Route: " << std::endl;
        for(auto goal : route){
            goal.print();
        }
        i++;
    }
}

// prints the TreeNode object's cost and constraints
void TreeNode::print() const{
    std::cout << "TreeNode cost: " << cost << std::endl;
    printConstraints();
}