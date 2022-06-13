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
            cells_[x][y].isText = false;
            cells_[x][y].nextDelivery = false;
            cells_[x][y].nextPickup = false;
            cells_[x][y].lastOccupied = 0;

            file >> read;
            std::cout << read;
            switch(read)
            {
                case 'X':{
                    cells_[x][y].isObstacle = true;
                }
                    break;
                case 'L':{
                    cells_[x][y].nextDelivery = true;
                }
                    break;
                case 'I':{
                    cells_[x][y].isText = true;
                }
                    break;
                case 'T':{
                    cells_[x][y].nextPickup = true;
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
                case 'H':
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
                case 'C':{
                    cells_[x][y].isUnknown = true;
                    cells_[x][y].isObstacle = true;
                }
                    break;
                case 'R':{
                    cells_[x][y].isRed = true;
                    cells_[x][y].isObstacle = true;
                }
                    break;
                case 'Y':{
                    cells_[x][y].isYellow = true;
                    cells_[x][y].isObstacle = true;
                }
                    break;
                case 'G':{
                    cells_[x][y].isGreen = true;
                    cells_[x][y].isObstacle = true;
                }
                    break;
                case 'V':{
                    cells_[x][y].isViolet = true;
                    cells_[x][y].isObstacle = true;
                }
                    break;
                case 'B':{
                    cells_[x][y].isBlue = true;
                    cells_[x][y].isObstacle = true;
                }
                    break;
                case 'P':{
                    cells_[x][y].isPink = true;
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

void Grid::setCellAvailable(int x, int y, bool state){
    cells_[x][y].isAvailable = state;
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
    float border_width = 0.1;
    if(viewMode==0 || viewMode==2){
        for(int x=0;x<mapWidth_;x++){
            for(int y=0;y<mapHeight_;y++){                
                if(cells_[x][y].isShelf)
                    glColor3f(1,1,1);
                else if (cells_[x][y].isStation)
                    glColor3f(0.0,0.5,0.5);
                else if(cells_[x][y].isUnknown)
                    glColor3ub(190, 190, 190);              
                // else if (cells_[x][y].isGoal)
                //     glColor4f(0.0,0.5,0.0,0.08);
                else if (cells_[x][y].notReached)
                    glColor3f(cells_[x][y].r, cells_[x][y].g, cells_[x][y].b);
                else if(cells_[x][y].isUsed && cells_[x][y].isExpanded)
                    glColor4f(0,1,0,0.1);
                else if(cells_[x][y].isExpanded)
                    glColor4f(1,0,0,0.1); 
                else if(cells_[x][y].isRed)
                    glColor4ub(245, 66, 84, cells_[x][y].a);
                else if(cells_[x][y].isYellow)
                    glColor4ub(255, 234, 41, cells_[x][y].a);
                else if(cells_[x][y].isGreen)
                    glColor4ub(54, 173, 76, cells_[x][y].a);
                else if(cells_[x][y].isViolet)
                    glColor4ub(169, 74, 247, cells_[x][y].a);
                else if(cells_[x][y].isBlue)
                    glColor4ub(141, 247, 228, cells_[x][y].a);
                else if(cells_[x][y].isPink)
                    glColor4ub(247, 141, 236, cells_[x][y].a);
                else if(cells_[x][y].isObstacle)
                    glColor3f(0.0,0.0,0.0);
                else if(cells_[x][y].nextDelivery){                    
                    // std::cout << "r: " << cells_[x][y].r << " g: " << cells_[x][y].g << " b: " << cells_[x][y].b << endl;
                    glColor4ub(cells_[x][y].r, cells_[x][y].g, cells_[x][y].b, 255);      
                }   
                else if(cells_[x][y].nextPickup){
                    // std::cout << "r: " << cells_[x][y].r << " g: " << cells_[x][y].g << " b: " << cells_[x][y].b << endl;
                    glColor4ub(cells_[x][y].r, cells_[x][y].g, cells_[x][y].b, 255);      
                }                                            
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
                // Draw borders of cells using the border_width parameter
                // only if cells are red, yellow, green, violet, blue, pink or unknown
                if(cells_[x][y].isRed || cells_[x][y].isYellow || 
                    cells_[x][y].isGreen || cells_[x][y].isViolet || 
                    cells_[x][y].isBlue || cells_[x][y].isPink)
                {
                    glColor3f(0,0,0);
                    glLineWidth(3);
                    glBegin( GL_LINE_LOOP );
                    {
                        glVertex2f(x  ,y); 
                        glVertex2f(x+1,y);
                        glVertex2f(x+1,y+1);
                        glVertex2f(x ,y+1);
                    }
                    glEnd();
                }
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
