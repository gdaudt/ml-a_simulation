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
        // variables for ml simulation
        bool isReady;
        bool isAvailable = true;
        // colors for ml simulation
        bool isRed;
        bool isYellow;
        bool isGreen;
        bool isViolet;
        bool isBlue;
        bool isPink;
        // color and bool for painting objectives        
        bool notReached;
        bool isText;
        bool nextDelivery;
        bool nextPickup;
        float r,g,b;
        float a = 65;
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
        void setCellAvailable(int x, int y, bool state);

        void draw(int viewMode);

    private:
        int mapScale_; // Number of cells per meter
        int mapWidth_, mapHeight_; // in cells

        Cell** cells_;
};

#endif // __GRID_H__
