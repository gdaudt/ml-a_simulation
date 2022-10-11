#ifndef __GRID_H__
#define __GRID_H__

#include <string>
#include <stack>

class Cell
{
    public:
        bool isObstacle;
        bool isStation;
        bool isShelf;
        bool isGoal;
        // debugging variables for astar
        bool isExpanded;
        bool isUsed;
        // lane constraints for planning
        bool isUplane;
        bool isDownlane;
        bool isUnknown;
        // color and bool for painting objectives        
        bool notReached;
        float r,g,b;
        // marks the step when the cell was last occupied
        int lastOccupied;
};

class Grid
{
    public:
        Grid(std::string mapName);
        Cell* getCell(int x, int y);
        bool isValid(int x, int y);
        bool isValidDynamic(int x, int y, int step);
        // resets cell painting for each pathfinding
        void resetAstar();
        int getMapScale();
        int getMapWidth();
        int getMapHeight();
        Cell** getCells();

        void draw(int viewMode);

    private:
        int mapScale_; // Number of cells per meter
        int mapWidth_, mapHeight_; // in cells

        Cell** cells_;
};

#endif // __GRID_H__
