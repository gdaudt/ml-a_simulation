#include "algorithm"
#include "LowLevelSolver.h"
#include <iostream>

std::vector<Goal> LowLevelSolver::solve(const std::vector<Constraint> &constraints, Agent &agent, Grid* grid){
    // std::cout << "starting low level solver for agent "<< agent.getId() << " (" << agent.getX() << "," << agent.getY() << ") " << std::endl;
    // optimalPath_.clear();
    Planner p;
    p.setGrid(grid);
    Goal goal = agent.popGoal();
    return p.constraintAstar(constraints, &agent, goal.x, goal.y);    
}

std::vector<std::vector<Goal>> LowLevelSolver::findOptimalPaths(const std::vector<Constraint> &constraints, std::vector<Agent> &agents, Grid* grid){
    for(Agent a : agents){        
        optimalPaths_.emplace_back(solve(constraints, a, grid));
        // std::cout << "emplacing agent " << a.getId() << " solution" << std::endl;
    }
    return optimalPaths_;
}