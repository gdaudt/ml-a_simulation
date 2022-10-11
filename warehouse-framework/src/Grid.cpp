#include <cstdio>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "Grid.h"
#include "math.h"

using std::cout;
using std::endl;

Grid::Grid (std::string mapName)
{
    mapScale_ = 2; // 2 cells per meter

    std::string name("maps/");
    name += mapName;
    std::ifstream file;
    file.open(name.c_str(), std::ifstream::in);

    if( !file.good() )
    {
        std::cerr << "The file \"" << name << "\"  does not exit!" << std::endl;
        return;
    }

    // Read dimensions.
    file >> mapWidth_ >> mapHeight_;
    std::cout << "map.width " << mapWidth_ << " map.height " << mapHeight_ << std::endl;

    cells_ = new Cell*[mapWidth_];
        for(int i=0;i<mapWidth_;i++)
            cells_[i] = new Cell[mapHeight_];

    // Read grid from file.
    char read;
    for(int y=0; y < mapHeight_; y++)
    {
        for(int x=0; x < mapWidth_; x++)
        {
            cells_[x][y].isObstacle = false;
            cells_[x][y].isStation = false;
            cells_[x][y].isShelf = false;
            cells_[x][y].isGoal = false;
            cells_[x][y].isDownlane = false;
            cells_[x][y].isUplane = false;
            cells_[x][y].isUnknown = false;
            cells_[x][y].lastOccupied = 0;

            file >> read;
            std::cout << read;
            switch(read)
            {
                case 'X':{
                    cells_[x][y].isObstacle = true;
                }
                    break;
                case 'S':{
                    cells_[x][y].isStation = true;
                    cells_[x][y].isObstacle = false;
                }
                    break;
                case 'N':{
                    cells_[x][y].isShelf = true;
                    cells_[x][y].isObstacle = true;
                }
                    break;
                case 'G':
                    cells_[x][y].isGoal = true;
                    break;
                case 'U':
                    cells_[x][y].isUplane = true;
                    break;
                case 'E':{
                    cells_[x][y].isUnknown = true;
                    cells_[x][y].isObstacle = true;
                }
                    break;
                case 'D':
                    cells_[x][y].isDownlane = true;
                    break;
            }
        }
        // file >> read;
        std::cout << std::endl;
    }

    file.close();
}

Cell* Grid::getCell (int x, int y)
{
    if(x>=0 && x<mapWidth_ && y>=0 && y<mapHeight_)
        return &(cells_[x][y]);
    else
        return NULL;
}

Cell** Grid::getCells(){
    return cells_;
}

bool Grid::isValid(int x, int y){
    if(cells_[x][y].isObstacle){
        //std::cout << "cell " << x << "," << y << " is an obstacle\n";
        return false;
    }            
    else{
        //std::cout << "cell " << x << "," << y << " is valid\n";
        return true;
    }   
}

bool Grid::isValidDynamic(int x, int y, int step){
    int occupied = abs(cells_[x][y].lastOccupied - step);
    if(occupied < 2){
        cout << "Difference between " << step << " and " << cells_[x][y].lastOccupied << " is " << occupied << endl;
        cout << "Cell (" << x << "," << y << ") is occupied at step " << cells_[x][y].lastOccupied << endl;
        return false;
    }
    else
        return true;    
}

int Grid::getMapScale()
{
    return mapScale_;
}

int Grid::getMapWidth()
{
    return mapWidth_;
}

int Grid::getMapHeight()
{
    return mapHeight_;
}

void Grid::resetAstar(){
    for(int y=0; y < mapHeight_; y++)
    {
        for(int x=0; x < mapWidth_; x++)
        {
            cells_[x][y].isUsed = false;
            cells_[x][y].isExpanded = false;
        }
    }
}

void Grid::draw(int viewMode)
{
    if(viewMode==0 || viewMode==2){
        for(int x=0;x<mapWidth_;x++){
            for(int y=0;y<mapHeight_;y++){

                if(cells_[x][y].isShelf)
                    glColor3f(0.7,0.7,0.7);
                else if (cells_[x][y].isStation)
                    glColor3f(0.0,0.5,0.5);
                else if (cells_[x][y].isObstacle)
                    glColor3f(0.0,0.0,0.0);
                // else if (cells_[x][y].isGoal)
                //     glColor4f(0.0,0.5,0.0,0.08);
                else if (cells_[x][y].notReached)
                    glColor3f(cells_[x][y].r,cells_[x][y].g,cells_[x][y].b);
                else if(cells_[x][y].isUsed && cells_[x][y].isExpanded)
                    glColor4f(0,1,0,0.1);
                else if(cells_[x][y].isExpanded)
                    glColor4f(1,0,0,0.1);       
                else if(cells_[x][y].isUnknown)
                    glColor3f(0.7,0.7,0.7);                                              
                else
                    glColor3f(1.0,1.0,1.0);
                cells_[x][y].lastOccupied = 0;
                glBegin( GL_QUADS );
                {
                    glVertex2f(x  ,y  );
                    glVertex2f(x+1,y  );
                    glVertex2f(x+1,y+1);
                    glVertex2f(x  ,y+1);
                }
                glEnd();
            }
        }
    }else if(viewMode==1){
        for(int x=0;x<mapWidth_;x++){
            for(int y=0;y<mapHeight_;y++){

                if (cells_[x][y].isObstacle)
                    glColor3f(0.0,0.0,0.0);
                else
                    glColor3f(1.0,1.0,1.0);

                glBegin( GL_QUADS );
                {
                    glVertex2f(x  ,y  );
                    glVertex2f(x+1,y  );
                    glVertex2f(x+1,y+1);
                    glVertex2f(x  ,y+1);
                }
                glEnd();
            }
        }
    }

    if(viewMode==2){

        //draw text in each cell
        for(int x=0;x<mapWidth_;x++){
            for(int y=0;y<mapHeight_;y++){

                std::stringstream s;
                s << cells_[x][y].lastOccupied;

                if(cells_[x][y].isObstacle)
                    glColor3f(1.0,1.0,1.0);
                else
                    glColor3f(0.0,0.0,0.0);

                glRasterPos2f(x+0.5, y+0.5);

                std::string text=s.str();
                for (unsigned int i=0; i<text.size(); i++)
                {
                    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, text[i]);
                }
                s.clear();
            }
        }
    }

}
