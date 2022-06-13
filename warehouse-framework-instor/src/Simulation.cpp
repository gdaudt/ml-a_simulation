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
Pick RED_P_ORIGIN = Pick(16, 40);
Pick RED_P_DESTINATION = Pick(16, 4);
Pick YELLOW_P_ORIGIN = Pick(16, 43);
Pick YELLOW_P_DESTINATION = Pick(16, 7);
Pick GREEN_P_ORIGIN = Pick(16, 46);
Pick GREEN_P_DESTINATION = Pick(16, 10);
Pick VIOLET_P_ORIGIN = Pick(16, 49);
Pick VIOLET_P_DESTINATION = Pick(16, 13);
Pick BLUE_P_ORIGIN = Pick(16, 52);
Pick BLUE_P_DESTINATION = Pick(16, 16);
Pick PINK_P_ORIGIN = Pick(16, 55);
Pick PINK_P_DESTINATION = Pick(16, 19);
Pick RED_D_ORIGIN = Pick(16, 3);
Pick RED_D_DESTINATION = Pick(16, 41);
Pick YELLOW_D_ORIGIN = Pick(16, 6);
Pick YELLOW_D_DESTINATION = Pick(16, 44);
Pick GREEN_D_ORIGIN = Pick(16, 9);
Pick GREEN_D_DESTINATION = Pick(16, 47);
Pick VIOLET_D_ORIGIN = Pick(16, 12);
Pick VIOLET_D_DESTINATION = Pick(16, 50);
Pick BLUE_D_ORIGIN = Pick(16, 15);
Pick BLUE_D_DESTINATION = Pick(16, 53);
Pick PINK_D_ORIGIN = Pick(16, 18);
Pick PINK_D_DESTINATION = Pick(16, 56);
Pick RED_PICKUP = Pick(16, 40);
Pick YELLOW_PICKUP = Pick(16, 43);
Pick GREEN_PICKUP = Pick(16, 46);
Pick VIOLET_PICKUP = Pick(16, 49);
Pick BLUE_PICKUP = Pick(16, 52);
Pick PINK_PICKUP = Pick(16, 55);
Pick RED_DELIVERY = Pick(16, 4);
Pick YELLOW_DELIVERY = Pick(16, 7);
Pick GREEN_DELIVERY = Pick(16, 10);
Pick VIOLET_DELIVERY = Pick(16, 13);
Pick BLUE_DELIVERY = Pick(16, 16);
Pick PINK_DELIVERY = Pick(16, 19);
int pickup_ready_count = 0;
bool pickup_setup = true;
int delivery_ready_count = 0;
bool delivery_setup = true;
enum Station{kRed, kYellow, kGreen, kViolet, kBlue, kPink};

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

inline Pick colorPickup(int color){
    switch (color)
    {
    case 0:
        return RED_P_ORIGIN;
        break;
    case 1:
       return YELLOW_P_ORIGIN;
       break;
    case 2:
        return GREEN_P_ORIGIN;
        break;
    case 3:
        return VIOLET_P_ORIGIN;
        break;
    case 4:
        return BLUE_P_ORIGIN;
        break;
    case 5:
        return PINK_P_ORIGIN;
        break;
    }     
}

inline Pick colorDelivery(int color){
    switch (color)
    {
    case 0:
        return RED_D_ORIGIN;
        break;
    case 1:
       return YELLOW_D_ORIGIN;
       break;
    case 2:
        return GREEN_D_ORIGIN;
        break;
    case 3:
        return VIOLET_D_ORIGIN;
        break;
    case 4:
        return BLUE_D_ORIGIN;
        break;
    case 5:
        return PINK_D_ORIGIN;
        break;
    }     
}

inline Pick getDeliveryFromPickup(Pick pickup){
    if(pickup == RED_PICKUP or pickup == RED_P_ORIGIN)
        return RED_P_DESTINATION;
    else if(pickup == YELLOW_PICKUP or pickup == YELLOW_P_ORIGIN)
        return YELLOW_P_DESTINATION;
    else if(pickup == GREEN_PICKUP or pickup == GREEN_P_ORIGIN)
        return GREEN_P_DESTINATION;
    else if(pickup == VIOLET_PICKUP or pickup == VIOLET_P_ORIGIN)
        return VIOLET_P_DESTINATION;
    else if(pickup == BLUE_PICKUP or pickup == BLUE_P_ORIGIN)
        return BLUE_P_DESTINATION;
    else if(pickup == PINK_PICKUP or pickup == PINK_P_ORIGIN)
        return PINK_P_DESTINATION;
    else
        return Pick();
}

inline Pick getPickupFromDelivery(Pick pickup){
    if(pickup == RED_DELIVERY or pickup == RED_D_ORIGIN)
        return RED_D_DESTINATION;
    else if(pickup == YELLOW_DELIVERY or pickup == YELLOW_D_ORIGIN)
        return YELLOW_D_DESTINATION;
    else if(pickup == GREEN_DELIVERY or pickup == GREEN_D_ORIGIN)
        return GREEN_D_DESTINATION;
    else if(pickup == VIOLET_DELIVERY or pickup == VIOLET_D_ORIGIN)
        return VIOLET_D_DESTINATION;
    else if(pickup == BLUE_DELIVERY or pickup == BLUE_D_ORIGIN)
        return BLUE_D_DESTINATION;
    else if(pickup == PINK_DELIVERY or pickup == PINK_D_ORIGIN)
        return PINK_D_DESTINATION;
    else
        return Pick();
}

inline int getDeliveryColor(Pick location){
    if(location == RED_P_DESTINATION or location == RED_P_ORIGIN){
        return 0;
    }
    else if(location == YELLOW_P_DESTINATION or location == YELLOW_P_ORIGIN){
        return 1;
    }
    else if(location == GREEN_P_DESTINATION or location == GREEN_P_ORIGIN){
        return 2;
    }
    else if(location == VIOLET_P_DESTINATION or location == VIOLET_P_ORIGIN){
        return 3;
    }
    else if(location == BLUE_P_DESTINATION or location == BLUE_P_ORIGIN){
        return 4;
    }
    else if(location == PINK_P_DESTINATION or location == PINK_P_ORIGIN){
        return 5;
    }
    else{
        return -1;
    }
}

inline int getPickupColor(Pick location){
    if(location == RED_D_DESTINATION or location == RED_D_ORIGIN){
        return 0;
    }
    else if(location == YELLOW_D_DESTINATION or location == YELLOW_D_ORIGIN){
        return 1;
    }
    else if(location == GREEN_D_DESTINATION or location == GREEN_D_ORIGIN){
        return 2;
    }
    else if(location == VIOLET_D_DESTINATION or location == VIOLET_D_ORIGIN){
        return 3;
    }
    else if(location == BLUE_D_DESTINATION or location == BLUE_D_ORIGIN){
        return 4;
    }
    else if(location == PINK_D_DESTINATION or location == PINK_D_ORIGIN){
        return 5;
    }
    else{
        return -1;
    }
}

void Simulation::initialize(std::string fname)
{
    currentStep_=1;

    numAgents_ = 1;  
    createAgents();
    // setRandomPickAndDelivery(0);
    setAgentPosition(RED_PICKUP.x, RED_PICKUP.y, 0);

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
    
    moveAgentsToGoal();
    setRandomDeliveryReady();
    setRandomPickupReady();
    updateNextDeliveryColor();
    updateNextPickupColor();

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
        
        r = 75;
        g = 75;
        b = 75; 
        agents_[i].red = r;
        agents_[i].green = g;
        agents_[i].blue = b;       
        //init agent in a valid pose
        do{
            agents_[i].setPosition(randomPosition());
            agents_[i].setDirection(kRight);
            agents_[i].setId(i);
            agents_[i].setCarrying(false);
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
    agents_[pos].next_action = goal.type;
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
            std::cout << "Agent " << i << " reached goal " << std::endl;
            std::cout << "Agent " << i << " next action: " << agents_[i].next_action << std::endl;
            if(agents_[i].next_action == Pick::kPickup){
                agents_[i].setCarrying(true);
                setAgentColor(i, getPickupColor(agents_[i].getNextPick()));                
                setDeliveryAvailable(agents_[i].getX(), agents_[i].getY());
                std::cout << "next pickup: (" << agents_[i].getNextPick().x << "," << agents_[i].getNextPick().y << ")" << std::endl;
                agents_[i].last_action = Pick::kPickup;
            }
            else if(agents_[i].next_action == Pick::kDelivery){
                agents_[i].setCarrying(true);
                setAgentColor(i, getDeliveryColor(agents_[i].getNextPick()));
                setPickupAvailable(agents_[i].getX(), agents_[i].getY());
                std::cout << "next delivery: (" << agents_[i].getNextPick().x << "," << agents_[i].getNextPick().y << ")" << std::endl;
                agents_[i].last_action = Pick::kDelivery;
            }            
            else if(agents_[i].next_action == Pick::kNone){
                agents_[i].setCarrying(false);
                agents_[i].red = 75;
                agents_[i].green = 75;
                agents_[i].blue = 75;
                agents_[i].last_action = Pick::kNone;
            }
            planPath(i);            
        }
        //no more goals, try to get more
        else if(!agents_[i].hasPicks()){
            std::cout << "agent next action: " << agents_[i].next_action << std::endl;
            if(agents_[i].last_action == Pick::kPickup){
                Mission mission = getNextPickupMission();
                if(mission.isValid()){                    
                    agents_[i].addPick(mission.start);
                    agents_[i].addPick(mission.end);
                }
                else{
                    mission = getNextDeliveryMission();
                    if(mission.isValid()){
                        agents_[i].addPick(mission.start);
                        agents_[i].addPick(mission.end);
                    }    
                }
            } else{
                Mission mission = getNextDeliveryMission();
                if(mission.isValid()){
                    agents_[i].addPick(mission.start);
                    agents_[i].addPick(mission.end);
                }
                else {
                    mission = getNextPickupMission();
                    if(mission.isValid()){
                        agents_[i].addPick(mission.start);
                        agents_[i].addPick(mission.end);
                    }
                }
            }
        }
            // check if there are waves available
            // if(!waves_.empty()){
            //     //getNewGoals(i);
            // }    
            // agents_[i].setDirection(kLeft);           
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
        glColor3ub(agents_[i].red,agents_[i].green,agents_[i].blue);

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
        // turning into square to symbolize carrying
        if(agents_[i].isCarrying()){
            glBegin(GL_POLYGON);
            {
                glVertex2f(-0.5, -0.5);
                glVertex2f(-0.5, 0.5);
                glVertex2f(0.5, 0.5);
                glVertex2f(0.5, -0.5);
            }
            glEnd();
        }
        else{
            glBegin( GL_POLYGON );
            {
                glVertex2f(-0.5, -0.5);
                glVertex2f( 0.5, -0.5);
                glVertex2f( 0.5,  0.0);
                glVertex2f( 0.0,  0.5);
                glVertex2f(-0.5,  0.0);
            }
            glEnd();            
        }               
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

void Simulation::setRandomPicks(int amount){
    for(unsigned int i=0; i<numAgents_; i++){        
        for(int g=0; g<amount; g++){            
            Pick pick = randomPick();
            std::cout << "adding to agent " << i << " goal (" << pick.x << "," << pick.y << ")\n";            
            grid->getCell(pick.x, pick.y)->r = agents_[i].red;
            grid->getCell(pick.x, pick.y)->g = agents_[i].green;
            grid->getCell(pick.x, pick.y)->b = agents_[i].blue;
            grid->getCell(pick.x, pick.y)->notReached = true;
            agents_[i].addPick(pick); 
        }
    }   
}

// generates random positions on the pick and delivery stations.
// always generates a pickup objective first, then a delivery objective
// adds them directly to the agent at the 0 position
// make two different inline methods, one for generating a random pickup location as Pick
// second inline method for a random delivery station as Pick


void Simulation::setRandomPickupReady(){    
    if(pickup_setup){
        pickup_ready_count = currentStep_ + (rand()%30 + 60);
        pickup_setup = false;
    }
    if(pickup_ready_count < currentStep_){  
        int color = rand()%5;
        Pick pick = colorPickup(color);              
        if(std::find(pickup_list_.begin(), pickup_list_.end(), pick) == pickup_list_.end()){
            grid->getCell(pick.x+1, pick.y)->a = 255;
            grid->getCell(pick.x+2, pick.y)->a = 255;
            grid->getCell(pick.x+1, pick.y+1)->a = 255;
            grid->getCell(pick.x+2, pick.y+1)->a = 255;
            grid->getCell(pick.x, pick.y)->isAvailable = false;
            pickup_list_.emplace_back(pick);
            std::cout << "pickup ready at (" << pick.x << "," << pick.y << ") available: " << grid->getCell(pick.x, pick.y)->isAvailable << "\n";        
        }
        pickup_ready_count = 0;
        pickup_setup = true;        
    }   
}

void Simulation::setRandomDeliveryReady(){
    if(delivery_setup){
        delivery_ready_count = currentStep_ + (rand()%30 + 70);
        delivery_setup = false;
    }
    if(delivery_ready_count < currentStep_){
        int color = rand()%5;
        Pick pick = colorDelivery(color);
        if(std::find(delivery_list_.begin(), delivery_list_.end(), pick) == delivery_list_.end()){
            grid->getCell(pick.x+1, pick.y)->a = 255;
            grid->getCell(pick.x+2, pick.y)->a = 255;
            grid->getCell(pick.x+1, pick.y+1)->a = 255;
            grid->getCell(pick.x+2, pick.y+1)->a = 255;
            grid->getCell(pick.x, pick.y)->isAvailable = false;
            delivery_list_.emplace_back(pick);
            std::cout << "delivery ready at (" << pick.x << "," << pick.y << ") available: " << grid->getCell(pick.x, pick.y)->isAvailable << "\n";
        }
        delivery_ready_count = 0;
        delivery_setup = true;
    }
}

void Simulation::setPickupAvailable(int x, int y){
    grid->getCell(x,y)->isAvailable = true;
    grid->getCell(x+2,y)->a = 65;
    grid->getCell(x+1,y)->a = 65;
    grid->getCell(x+2,y+1)->a = 65;
    grid->getCell(x+1,y+1)->a = 65;
}

void Simulation::setDeliveryAvailable(int x, int y){
    grid->getCell(x,y)->isAvailable = true;
    grid->getCell(x+1,y)->a = 65;
    grid->getCell(x+2,y)->a = 65;
    grid->getCell(x+1,y+1)->a = 65;
    grid->getCell(x+2,y+1)->a = 65;
}

// needs to check if the delivery location to the corresponding pickup location is free
Pick Simulation::getNextPickup(){
    if(pickup_list_.size() > 0){
        std::list<Pick>::iterator it;
        for(it = pickup_list_.begin(); it != pickup_list_.end(); it++){
            Pick p = getDeliveryFromPickup(Pick(it->x, it->y));
            if(p.x != 0){
                if(grid->getCell(p.x, p.y)->isAvailable){
                    Pick pick = Pick(it->x, it->y);
                    pickup_list_.erase(it);
                    return pick;
                }
            }            
        }
    }
    else{
        return Pick();
    }
}

// need to check if there are any pickup spots available before returning a delivery
Pick Simulation::getNextDelivery(){
    if(delivery_list_.size() > 0){
        std::list<Pick>::iterator it;
        for(it = delivery_list_.begin(); it != delivery_list_.end(); it++){ 
            Pick p = getPickupFromDelivery(Pick(it->x, it->y));           
            if(grid->getCell(p.x, p.y)->isAvailable){
                Pick pick = *it;
                delivery_list_.erase(it);
                return pick;
            }
        }
    }
    else{
        return Pick();
    }
}

Mission Simulation::getNextPickupMission(){
    if(pickup_list_.size() > 0){
        std::list<Pick>::iterator it;
        for(it = pickup_list_.begin(); it != pickup_list_.end(); it++){
            int x = it->x;
            int y = it->y;
            Pick p = getDeliveryFromPickup(Pick(x, y));
            if(p.x != 0){                                
                Pick pick = Pick(x, y);
                pickup_list_.erase(it);
                pick.type = Pick::kDelivery;
                p.type = Pick::kNone;
                return Mission(pick, p);                
            }
            else{
                return Mission();
            }            
        }
    }
    else{
        return Mission();
    }
}

Mission Simulation::getNextDeliveryMission(){
    if(delivery_list_.size() > 0){
        std::list<Pick>::iterator it;        
        for(it = delivery_list_.begin(); it != delivery_list_.end(); it++){ 
            int x = it->x;
            int y = it->y;
            Pick p = getPickupFromDelivery(Pick(x,y));           
            if(p.x != 0){                
                Pick pick = Pick(x, y);
                delivery_list_.erase(it);
                pick.type = Pick::kPickup;
                p.type = Pick::kNone;
                return Mission(pick, p);
            }
            else{
                return Mission();
            }
        }
    }
    else{
        return Mission();
    }
}


void Simulation::setCellColor(int x, int y, int r, int g, int b){
    grid->getCell(x,y)->r = r;
    grid->getCell(x,y)->g = g;
    grid->getCell(x,y)->b = b;
}
// RED = glColor3ub(245, 66, 84);
// YELLOW = glColor3ub(255, 234, 41);
// GREEN = glColor3ub(54, 173, 76);
// VIOLET = glColor3ub(169, 74, 247);
// BLUE = glColor3ub(141, 247, 228);
// PINK = glColor3ub(247, 141, 236);
void Simulation::updateNextDeliveryColor(){
    if(delivery_list_.size() > 0){
        Pick pick = delivery_list_.front();
        int color = getPickupColor(pick);
        std::cout << "pick: " << pick.x << "," << pick.y << " color: " << color << "\n";
        std::cout << "color: " << color << "\n";
        switch(color){
            case 0:
                // std::cout << "RED\n";
                setCellColor(36, 8, 245, 66, 84);
                setCellColor(35, 8, 245, 66, 84);
                setCellColor(36,7, 245, 66, 84);
                setCellColor(35, 7, 245, 66, 84);
                break;
            case 1:
                // std::cout << "YELLOW\n";
                setCellColor(36, 8, 255, 234, 41);
                setCellColor(35, 8, 255, 234, 41);
                setCellColor(36,7, 255, 234, 41);
                setCellColor(35, 7, 255, 234, 41);
                break;
            case 2:
                // std::cout << "GREEN\n";
                setCellColor(36, 8, 54, 173, 76);
                setCellColor(35, 8, 54, 173, 76);
                setCellColor(36,7, 54, 173, 76);
                setCellColor(35, 7, 54, 173, 76);
                break;
            case 3:
                // std::cout << "VIOLET\n";
                setCellColor(36, 8, 169, 74, 247);
                setCellColor(35, 8, 169, 74, 247);
                setCellColor(36,7, 169, 74, 247);
                setCellColor(35, 7, 169, 74, 247);
                break;
            case 4:
                // std::cout << "BLUE\n";
                setCellColor(36, 8, 141, 247, 228);
                setCellColor(35, 8, 141, 247, 228);
                setCellColor(36,7, 141, 247, 228);
                setCellColor(35, 7, 141, 247, 228);
                break;
            case 5:
                // std::cout << "PINK\n";
                setCellColor(36, 8, 247, 141, 236);
                setCellColor(35, 8, 247, 141, 236);
                setCellColor(36,7, 247, 141, 236);
                setCellColor(35, 7, 247, 141, 236);
                break;
            default:
                break;
        }
    }
    else{
        // std::cout << "NO DELIVERY\n";
        setCellColor(36, 8, 190, 190, 190);
        setCellColor(35, 8, 190, 190, 190);
        setCellColor(36,7, 190, 190, 190);
        setCellColor(35, 7, 190, 190, 190);
    }
}

void Simulation::updateNextPickupColor(){
    if(pickup_list_.size() > 0){
        Pick pick = pickup_list_.front();
        int color = getDeliveryColor(pick);
        switch(color){
            case 0:
                // std::cout << "RED\n";
                setCellColor(36, 45, 245, 66, 84);
                setCellColor(35, 45, 245, 66, 84);
                setCellColor(36,44, 245, 66, 84);
                setCellColor(35, 44, 245, 66, 84);
                break;
            case 1:
                // std::cout << "YELLOW\n";
                setCellColor(36, 45, 255, 234, 41);
                setCellColor(35, 45, 255, 234, 41);
                setCellColor(36,44, 255, 234, 41);
                setCellColor(35, 44, 255, 234, 41);
                break;
            case 2:
                // std::cout << "GREEN\n";
                setCellColor(36, 45, 54, 173, 76);
                setCellColor(35, 45, 54, 173, 76);
                setCellColor(36,44, 54, 173, 76);
                setCellColor(35, 44, 54, 173, 76);
                break;
            case 3:
                // std::cout << "VIOLET\n";    
                setCellColor(36, 45, 169, 74, 247);
                setCellColor(35, 45, 169, 74, 247);
                setCellColor(36,44, 169, 74, 247);
                setCellColor(35, 44, 169, 74, 247);
                break;
            case 4:
                // std::cout << "BLUE\n";
                setCellColor(36, 45, 141, 247, 228);
                setCellColor(35, 45, 141, 247, 228);
                setCellColor(36,44, 141, 247, 228);
                setCellColor(35, 44, 141, 247, 228);
                break;
            case 5:
                // std::cout << "PINK\n";
                setCellColor(36, 45, 247, 141, 236);
                setCellColor(35, 45, 247, 141, 236);
                setCellColor(36,44, 247, 141, 236);
                setCellColor(35, 44, 247, 141, 236);
                break;
            default:
                break;
        }
    }
    else{
        // std::cout << "NO PICKUP\n";
        setCellColor(36, 45, 190, 190, 190);
        setCellColor(35, 45, 190, 190, 190);
        setCellColor(36,44, 190, 190, 190);
        setCellColor(35, 44, 190, 190, 190);
    }

}

// Mission Simulation::getNextDeliveryMission(){
//     std::cout << "getting new delivery mission\n";
//     if(delivery_list_.size() > 0){
//         Pick start = delivery_list_.front();
//         delivery_list_.pop_front();
//         Pick end = getPickupFromDelivery(start);
//     }
//     else{
//         return Mission();
//     }
// }

// checks through all the pickup locations and returns the first one that is available
Pick Simulation::getFirstPickup(){
    if(grid->getCell(RED_PICKUP.x, RED_PICKUP.y)->isAvailable)
        return RED_PICKUP;
    else if(grid->getCell(YELLOW_PICKUP.x, YELLOW_PICKUP.y)->isAvailable)
        return YELLOW_PICKUP;
    else if(grid->getCell(GREEN_PICKUP.x, GREEN_PICKUP.y)->isAvailable)
        return GREEN_PICKUP;
    else if(grid->getCell(VIOLET_PICKUP.x, VIOLET_PICKUP.y)->isAvailable)
        return VIOLET_PICKUP;
    else if(grid->getCell(BLUE_PICKUP.x, BLUE_PICKUP.y)->isAvailable)
        return BLUE_PICKUP;
    else if(grid->getCell(PINK_PICKUP.x, PINK_PICKUP.y)->isAvailable)
        return PINK_PICKUP;
    else
        return Pick();
}


// RED = glColor3ub(245, 66, 84);
// YELLOW = glColor3ub(255, 234, 41);
// GREEN = glColor3ub(54, 173, 76);
// VIOLET = glColor3ub(169, 74, 247);
// BLUE = glColor3ub(141, 247, 228);
// PINK = glColor3ub(247, 141, 236);
void Simulation::setAgentColor(int agentID, int color){
    switch (color)
    {
    case 0:
        agents_[agentID].red = 245;
        agents_[agentID].green = 66;
        agents_[agentID].blue = 84;
        break;
    case 1:
         agents_[agentID].red = 255;
         agents_[agentID].green = 234;
         agents_[agentID].blue = 41;
         break;
    case 2:
        agents_[agentID].red = 54;
        agents_[agentID].green = 173;
        agents_[agentID].blue = 76;
        break;
    case 3:
        agents_[agentID].red = 169;
        agents_[agentID].green = 74;
        agents_[agentID].blue = 247;
        break;
    case 4:
        agents_[agentID].red = 141;
        agents_[agentID].green = 247;
        agents_[agentID].blue = 228;
        break;
    case 5:
        agents_[agentID].red = 247;
        agents_[agentID].green = 141;
        agents_[agentID].blue = 236;
        break;
    }
}

void Simulation::setRandomPickAndDelivery(int agentID){
    int color = rand()%5;
    std::cout << "color: " << color << std::endl; 
    Pick pick = colorPickup(color);
    Pick delivery = colorDelivery(color);
    std::cout << "Adding to agent " << agentID << " pickup (" << pick.x << "," << pick.y << ")\n";
    agents_[agentID].addPick(pick);
    std::cout << "Adding to agent " << agentID << " delivery (" << delivery.x << "," << delivery.y << ")\n";
    agents_[agentID].addPick(delivery);
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

Pick Simulation::randomPick(){
    int w = grid->getMapWidth();
    int h = grid->getMapHeight();    
    int x = rand()%w;
    int y = rand()%h;          
    while(!grid->isValid(x,y)){
        //std::cout << "invalid goal, generating new\n";
        x = rand()%w;
        y = rand()%h;
    }   
    Pick pick;
    pick.x = x;
    pick.y = y;
    return pick;
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
 