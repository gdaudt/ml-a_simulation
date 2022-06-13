#ifndef __LOWLEVELSOLVER_H__
#define __LOWLEVELSOLVER_H__

#include "Utils.h"
#include "Node.h"
#include "Grid.h"
#include "Agent.h"
#include "Planner.h"
#include <vector>


class LowLevelSolver{
    public:

        std::vector<Goal> solve(const std::vector<Constraint> &constraints, Agent &agent, Grid* grid);
        std::vector<std::vector<Goal>> findOptimalPaths(const std::vector<Constraint> &constraints, std::vector<Agent> &agents, Grid* grid);       

    private:
        std::vector<std::vector<Goal>> optimalPaths_;
        std::vector<Goal> optimalPath_;

};

#endif