#ifndef __TREENODE_H__
#define __TREENODE_H__

#include "LowLevelSolver.h"
#include "Grid.h"
#include "Utils.h"

class TreeNode{
    public:
        void addConstraint(const Constraint& constraint);
        void updateSolution(std::vector<Agent>& agents, Grid* grid);
        void updateCost();
        void printSolution() const;
        void printConstraints() const;

        std::vector<std::vector<Goal>> getSolution() const;
        int getCost() const;

        std::vector<Constraint> getConstraints();

        TreeNode();

        TreeNode(const std::vector<Constraint>& constraints);
        ~TreeNode();

        void print() const;

    private:
        int cost = 0;

        std::vector<Constraint> constraints;
        std::vector<std::vector<Goal>> solution;
};

#endif