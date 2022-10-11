#ifndef __HIGHLEVELSOLVER_H__
#define __HIGHLEVELSOLVER_H__

#include "Utils.h"
#include "TreeNode.h"
#include "Grid.h"
#include "Node.h"

class TreeNode;

class HighLevelSolver
{
    public:
        HighLevelSolver();
        ~HighLevelSolver();
        static std::vector<std::vector<Goal>> solve(std::vector<Agent>& agents, Grid* grid);

    private:
        static bool hasConflict(const std::vector<Goal>& route1, const std::vector<Goal>& route2);
        static bool hasConflict(const TreeNode& T);
        static bool hasEdgeConflict(const std::vector<Goal>& route1, const std::vector<Goal>& route2);
        static int getEdgeConflict(const std::vector<Goal>& route1, const std::vector<Goal>& route2);
        static bool hasEdgeConflict(const TreeNode& T);
        static Conflict getEdgeConflict(const TreeNode& T);
        std::pair<Constraint, Constraint> getEdgeConflicts(const std::vector<Goal>& route1, const std::vector<Goal>& route2, int id1, int id2);
        static int getMinCost(const std::vector<TreeNode>& tree);
        static Conflict getFirstConflict(const TreeNode& T);
        static TreeNode findBestNode(const std::vector<TreeNode>& tree);
        static inline bool isEmpty(const std::vector<TreeNode>& tree);
};

#endif