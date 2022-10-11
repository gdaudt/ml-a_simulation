#include "Simulation.h"

#include <iostream>
#include <unistd.h>
#include <GL/glut.h>
#include <ostream>
#include <fstream>
#include <algorithm>
using std::cout;
using std::endl;
using std::find;

#include "Planner.h"


//////////////////////////////////////
///// CONSTRUCTORS & DESTRUCTORS /////
//////////////////////////////////////

Simulation::Simulation(std::string fname, std::string wname)
{
    initialized_ = false;
    mustFinish_ = false;
    running_ = true;

    stepByStep = false;

    std::cout << "Grid: " << fname << std::endl;
    std::cout << "Wave file:" << wname << std::endl;

    grid = new Grid(fname);
    //readWaveFile(wname);
}

Simulation::~Simulation()
{
    if(grid!=NULL)
        delete grid;
}

////////////////////////////////////
///// INITIALIZE & RUN METHODS /////
////////////////////////////////////

void Simulation::initialize(std::string fname)
{
    currentStep_=1;
    float mapCenterX = 14;
    float mapCenterY = 11;
    float scale = 0.4;

    numAgents_ = 3;  
    createAgents();
    // center of warehouse is in 14,11
    setAgentPosition(19, 14, 0);
    std::cout << "agent starting at " << (19-mapCenterX)*scale << "," << (mapCenterY-14)*scale << endl;
    setAgentPosition(19, 11, 1);
    std::cout << "agent starting at " << (19-mapCenterX)*scale << "," << (mapCenterY-11)*scale << endl;
    setAgentPosition(19, 8, 2);
    std::cout << "agent starting at " << (19-mapCenterX)*scale << "," << (mapCenterY-8)*scale << endl;
    
    // setAgentGoal(2, 7, 0);
    // setAgentGoal(2, 1, 1);
    setRandomGoals(1);
    //setRandomGoals();    


    initialized_ = true;
    controlTimer.startLap();
}

void Simulation::run()
{
    if(stepByStep){
        printf("Paused\n");
        while(stepByStep && !runNextInStepByStepMode){
            if(mustFinish_)
                break;
            usleep(100000);
        }
    }    
    
    moveAgentsToGoal("CBS");

    if(mustFinish_)
        running_=false;

    std::cout << "Step " << currentStep_ << " Time - total: " << controlTimer.getTotalTime() << " - from last measurement: " << controlTimer.getLapTime() << std::endl;

    currentStep_++;
    runNextInStepByStepMode=false;

    usleep(100000);
}

//////////////////////////
///// AGENTS METHODS /////
//////////////////////////

void Simulation::createAgents()
{
    Cell* selectedCell;
    GLfloat r,g,b;
    agents_.resize(numAgents_);

    unsigned int w = grid->getMapWidth();
    unsigned int h = grid->getMapHeight();

    for(unsigned int i=0; i<numAgents_; i++){
        
        r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); 
        agents_[i].red = r-0.2;
        agents_[i].green = g-0.2;
        agents_[i].blue = b-0.2;       
        //init agent in a valid pose
        do{
            agents_[i].setPosition(randomPosition());
            agents_[i].setDirection(kRight);
            agents_[i].setId(i);
            //agents_[i].setX(1);
            //agents_[i].setY(1);                         

            selectedCell = grid->getCell(agents_[i].getX(),agents_[i].getY());
        }while(selectedCell->isObstacle || selectedCell->lastOccupied == currentStep_);

        selectedCell->lastOccupied = currentStep_;
        std::cout << "starting agent " << i << " in (" << agents_[i].getX() << "," << agents_[i].getY() << ") \n"; 
        //std::cout << "Agent " << i << ": " << agents_[i].str() << std::endl;
    }
}

// uses the iterator to identify the agent
void Simulation::planPath(int pos){
    
    Planner planner;
    planner.setGrid(grid);
    Pick goal;

    goal = agents_[pos].popPick();
    std::cout << "planning new path for (" << goal.x << "," << goal.y << ")\n";
    planner.astar(&agents_[pos], goal.x, goal.y);
}

void Simulation::moveAgentsToGoal(){

    Cell *currentCell, *nextCell; 
    Action action;  

    for(unsigned int i=0; i<numAgents_; i++){

        currentCell = grid->getCell(agents_[i].getX(),agents_[i].getY());
        std::list<Action> path = agents_[i].getPath();  

        if(!path.empty()){
            action = agents_[i].popAction();
            switch(action){
                    case kMoveUp: 
                        //std::cout << "moving up from (" << agents_[i].getX() << "," << agents_[i].getY() << ") ";
                        agents_[i].moveUp();
                        //std::cout << "to (" << agents_[i].getX() << "," << agents_[i].getY() << ") \n";
                        break;
                    case kMoveDown:
                        //std::cout << "moving down from (" << agents_[i].getX() << "," << agents_[i].getY() << ") ";
                        agents_[i].moveDown(); 
                        //std::cout << "to (" << agents_[i].getX() << "," << agents_[i].getY() << ") \n";
                        break;
                    case kMoveLeft:
                        //std::cout << "moving left from (" << agents_[i].getX() << "," << agents_[i].getY() << ") ";
                        agents_[i].moveLeft();
                        //std::cout << "to (" << agents_[i].getX() << "," << agents_[i].getY() << ")\n "; 
                        break;
                    case kMoveRight: 
                        //std::cout << "moving right from (" << agents_[i].getX() << "," << agents_[i].getY() << ") "; 
                        agents_[i].moveRight(); 
                        //std::cout << "to (" << agents_[i].getX() << "," << agents_[i].getY() << ")\n ";
                        break;
                    case kWait:
                        //std::cout << "waiting in (" << agents_[i].getX() << "," << agents_[i].getY() << ")";
                        agents_[i].wait();
            }
        }
        else if(agents_[i].hasPicks()){
            //plan new path
            std::cout << "Agent " << i << " reached goal ";
            grid->getCell(agents_[i].getX(), agents_[i].getY())->notReached = false;
            // grid->getCell(agents_[i].getX(), agents_[i].getY())->r = agents_[i].red;
            // grid->getCell(agents_[i].getX(), agents_[i].getY())->g = agents_[i].green;
            // grid->getCell(agents_[i].getX(), agents_[i].getY())->b = agents_[i].blue;
            //grid->resetAstar();
            planPath(i);            
        }
        //no more goals, try to get more
        else if(!agents_[i].hasPicks()){
            std::cout << "Agent " << i << " has reached all goals and is idle\n";
            grid->getCell(agents_[i].getX(), agents_[i].getY())->notReached = false;            
            // check if there are waves available
            if(!waves_.empty()){
                //getNewGoals(i);
            }    
                               agents_[i].setDirection(kLeft);
   
        }
    }
}

void Simulation::moveAgentsToGoal(std::string algorithm){
    Cell *currentCell, *nextCell; 
    Goal goal;
    if(algorithm == "CBS"){
        if(pflag_){
            // std::cout << "is cbs" << std::endl;
            Planner planner;
            planner.setGrid(grid);
            planner.solveCBS(agents_);
            pflag_ = false;
        }
        else{
            // std::cout << "entered else" << std::endl;
            for(unsigned int i=0; i<numAgents_; i++){            
                // std::cout << "trying to move agents to goal\n";
                // std::cout << "i value: " << i << std::endl;
                currentCell = grid->getCell(agents_[i].getX(),agents_[i].getY());
                auto path = agents_[i].getCoordinates(); 
                // std::cout << "Agents path: " << std::endl;
                agents_[i].printCpath();
                if(!path.empty()){
                    // std::cout << "path not empty" << std::endl;
                    goal = agents_[i].popPathGoal();
                    // std::cout << "popping: ";
                    goal.print();
                    agents_[i].setPosition(goal);
                }
                else{
                    // std::cout << "path empty" << std::endl;
                }
            }
        }
    }   
    else
        moveAgentsToGoal();
}

void Simulation::moveAgentsRandomly()
{
    Cell *currentCell, *nextCell;

    unsigned int w = grid->getMapWidth();
    unsigned int h = grid->getMapHeight();

    for(unsigned int i=0; i<numAgents_; i++){
        
        currentCell = grid->getCell(agents_[i].getX(),agents_[i].getY());

        int m=rand()%5; // 2 chances to rotate, and the rest is the likelihood of moving forward

        if(m==0){ //rotate right 
            if(agents_[i].direction() == kUp)
                agents_[i].setDirection(kRight);
            else if(agents_[i].direction() == kDown)
                agents_[i].setDirection(kLeft);
            else if(agents_[i].direction() == kLeft)
                agents_[i].setDirection(kUp);
            else if(agents_[i].direction() == kRight)            
                agents_[i].setDirection(kDown);

            currentCell->lastOccupied = currentStep_;

        }else if(m==1){ // rotate left
            if(agents_[i].direction() == kUp)
                agents_[i].setDirection(kLeft);
            else if(agents_[i].direction() == kDown)
                agents_[i].setDirection(kRight);
            else if(agents_[i].direction() == kLeft)
                agents_[i].setDirection(kDown);
            else if(agents_[i].direction() == kRight)            
                agents_[i].setDirection(kUp);

            currentCell->lastOccupied = currentStep_;

        }else{ // try to move forward
            int x=agents_[i].getX();
            int y=agents_[i].getY();
            if(agents_[i].direction() == kUp)
                y--;
            else if(agents_[i].direction() == kDown)
                y++;
            else if(agents_[i].direction() == kLeft)
                x--;
            else if(agents_[i].direction() == kRight)
                x++;
                
            nextCell = grid->getCell(x,y);
            if(nextCell->isObstacle == false && (nextCell->lastOccupied==0 || nextCell->lastOccupied<currentStep_-1)){
                // if cell is empty, move forward
                agents_[i].setX(x);
                agents_[i].setY(y);
                nextCell->lastOccupied = currentStep_;
            }else{
                // else, stay in the same place
                currentCell->lastOccupied = currentStep_;
            }
        }
    }
} 

void Simulation::drawAgents()
{
    
    for(unsigned int i=0; i<numAgents_; i++){        
        //std::cout << "drawing agent with color (" << r << "," << g << "," << b << ")\n";
        glColor3f(agents_[i].red,agents_[i].green,agents_[i].blue);

        float angle;
        if(agents_[i].direction()==kUp)
            angle=180;
        else if(agents_[i].direction()==kDown)
            angle=0;
        else if(agents_[i].direction()==kLeft)
            angle=90;
        else if(agents_[i].direction()==kRight)
            angle=-90;            

        glTranslatef(agents_[i].getX()+0.5,agents_[i].getY()+0.5,0);
        glRotatef(angle,0,0,1);
        glScalef(0.8,0.8,1.0);
        glBegin( GL_POLYGON );
        {
            glVertex2f(-0.5, -0.5);
            glVertex2f( 0.5, -0.5);
            glVertex2f( 0.5,  0.0);
            glVertex2f( 0.0,  0.5);
            glVertex2f(-0.5,  0.0);
        }
        glEnd();
        glScalef(1.0/0.8,1.0/0.8,1.0);
        glRotatef(-angle,0,0,1);
        glTranslatef(-agents_[i].getX()-0.5,-agents_[i].getY()-0.5,0);
    }
}

void Simulation::setAgentColors(){
    
}

/////////////////////////////
///// AUXILIARY METHODS /////
/////////////////////////////

//random goal generator, returns a valid pair of points
Pick Simulation::randomGoal(){

    int w = grid->getMapWidth();
    int h = grid->getMapHeight();    
    int x = rand()%w;
    int y = rand()%h;          
    while(!grid->isValid(x,y)){
        //std::cout << "invalid goal, generating new\n";
        x = rand()%w;
        y = rand()%h;
    }   
    Pick goal;
    goal.x = x;
    goal.y = y;
    return goal;
}

void Simulation::getNewGoals(int agentId){
    if(!currentWave_.isEmpty()){
        Order order = getNewOrder();
        if(!order.isEmpty()){
            agents_[agentId].addOrder(order);
            paintGoals(agentId);
        }                        
    }
    else{
        cout << "No more orders on wave, getting new one" << endl;
        getNewWave();
    }   
}

// reads the wave file, adding the waves to the waves_ list in class
// for now, the wave and order ids are incrementally generated and assigned
// the list is not ordered, currently a FIFO
void Simulation::readWaveFile(std::string waveName){

    std::ifstream wave_file;
    std::string filePath("waves/");
    filePath += waveName;
    wave_file.open(filePath.c_str(), std::ifstream::in);
    int coordinate, wave_size;
    //breaker detector for ending orders
    char breaker;
    int waveId = 0;
    int orderId = 0;
    //read x wave size
    wave_file >> wave_size;

    //reading all waves
    while(!wave_file.eof()){        
        Wave wave;
        wave.setId(waveId);
        wave.setCompleted(false);
        //single wave reading
        for(int i=0; i < wave_size; i++){
            //single order reading
            Order order;
            order.setId(orderId);
            while(breaker != 'X'){
                if(isalpha(wave_file.peek())){
                    wave_file >> breaker;                    
                    break;
                }
                //single pick reading
                Pick pick;
                //reading x coordinate
                wave_file >> coordinate;
                pick.x = coordinate;
                //reading y coordinate
                wave_file >> coordinate;
                pick.y = coordinate;
                order.addPick(pick);
                order.setCompleted(false);                
            }            
            wave.addOrder(order);            
            orderId++;
            //resetting breaker character for new order
            breaker = 'b';
        }
        waves_.emplace_back(wave);
        wave_file >> wave_size;
        waveId++;             
    }  
    wave_file.close();
    std::cout << "Waves file read\n";
    currentWave_ = waves_.front();
}

// sets a random amount of goals for every agent
// used for testing purposes
void Simulation::setRandomGoals(){

    int goal_amount;

    for(unsigned int i=0; i<numAgents_; i++){
        goal_amount = rand()%10;
        for(int g=0; g<goal_amount; g++){            
            Pick goal = randomGoal();
            std::cout << "adding to agent " << i << " goal (" << goal.x << "," << goal.y << ")\n";            
            grid->getCell(goal.x, goal.y)->r = agents_[i].red;
            grid->getCell(goal.x, goal.y)->g = agents_[i].green;
            grid->getCell(goal.x, goal.y)->b = agents_[i].blue;
            grid->getCell(goal.x, goal.y)->notReached = true;
            agents_[i].addPick(goal); 
        }
    }   
}

//sets a random amount of goals for every agent
//used for testing purposes, set amount with parameters
void Simulation::setRandomGoals(int amount){

    for(unsigned int i=0; i<numAgents_; i++){        
        for(int g=0; g<amount; g++){            
            Goal goal = randomPosition();
            std::cout << "adding to agent " << i << " goal (" << goal.x << "," << goal.y << ")\n";            
            grid->getCell(goal.x, goal.y)->r = agents_[i].red;
            grid->getCell(goal.x, goal.y)->g = agents_[i].green;
            grid->getCell(goal.x, goal.y)->b = agents_[i].blue;
            grid->getCell(goal.x, goal.y)->notReached = true;
            agents_[i].addGoal(goal); 
        }
    }   
}

//generates an x and y number for a random goal
//both values have to be a valid grid coordinate, and unnocupied
//if the coordinate is not valid, generate new random numbers
Goal Simulation::randomPosition(){
    int w = grid->getMapWidth();
    int h = grid->getMapHeight();    
    int x = rand()%w;
    int y = rand()%h;          
    while(!grid->isValid(x,y)){
        //std::cout << "invalid goal, generating new\n";
        x = rand()%w;
        y = rand()%h;
    }   
    Goal goal;
    goal.x = x;
    goal.y = y;
    return goal;
}

void Simulation::getNewWave(){
    if(currentWave_.isCompleted()){
        cout << "Wave " << currentWave_.getId() << " is completed, getting new wave" << endl;
        completedWaves_.emplace_back(currentWave_.getId());
    }
    if(!waves_.empty()){
        if(find(completedWaves_.begin(), completedWaves_.end(), currentWave_.getId()) != completedWaves_.end()){  
            waves_.pop_front(); 
            if(!waves_.empty()){
                currentWave_ = waves_.front();
                cout << "New wave is Wave " << currentWave_.getId() << endl;
            } 
        }
    } else{
        cout << "All waves have been completed" << endl;
    }    
}

Order Simulation::getNewOrder(){
    // use lists now
    // assign current wave
    // add orders to agents, update order completion to list
    // after all orders have been completed, add wave to completedWaves list       
    if(!currentWave_.isCompleted()){
        cout << "Wave " << currentWave_.getId() << " is not completed, getting new order" << endl;
        Order order = currentWave_.popOrder();
        order.printOrder();
        return order;
    }    
}

void Simulation::addOptimalPaths(const std::vector<std::vector<Goal>>& optimalPaths){
    for(int i = 0; i < agents_.size(); i++){
        agents_[i].setCoordinates(optimalPaths[i]);
    }
}

// internal method for grid painting
void Simulation::paintGoals(int agentId){
    cout << "paiting agent " << agentId << endl;
    for(Pick pick : agents_[agentId].getPicks()){
        grid->getCell(pick.x, pick.y)->notReached = true;
        grid->getCell(pick.x, pick.y)->r = agents_[agentId].red;
        grid->getCell(pick.x, pick.y)->g = agents_[agentId].green;
        grid->getCell(pick.x, pick.y)->b = agents_[agentId].blue;
    }
}

void Simulation::setTestGoal(int x, int y){
    Pick goal;
    goal.x = x;
    goal.y = y;
    agents_[0].addPick(goal);
}

void Simulation::setTestGoal(int x, int y, int agentId){
    Pick goal;
    goal.x = x;
    goal.y = y;
    agents_[agentId].addPick(goal);    
}

void Simulation::setAgentGoal(int x, int y, int agentId){
    Goal g;
    g.x = x;
    g.y = y;
    agents_[agentId].addGoal(g);
}

void Simulation::setAgentPosition(int x, int y, int agentId){
    agents_[agentId].setX(x);
    agents_[agentId].setY(y);
    cout << "Agent " << agentId << " starting at (" << x << "," << y << ")" << endl;
}

void Simulation::printWaves(){
    for(Wave wave : waves_){
        wave.printWave();
    }
}

bool Simulation::initializationCompleted()
{
    return initialized_;
}

void Simulation::finish()
{
    mustFinish_=true;
}

bool Simulation::isRunning()
{
    return running_;
}
 