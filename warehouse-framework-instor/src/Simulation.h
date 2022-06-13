#ifndef SIMULATION_H
#define SIMULATION_H

#include <string>
#include <vector>
#include <list>
#include <map> 

#include "Grid.h"
#include "Utils.h"
#include "Agent.h"
#include "Wave.h"

struct Mission{
    
    Pick start;
    Pick end;
    int type;

    Mission(Pick start, Pick end){
        this->start = start;
        this->end = end;
    }
    Mission(){
        this->start = Pick();
        this->end = Pick();
    }

    friend bool operator==(const Mission& m1, const Mission& m2){
        return (m1.start == m2.start && m1.end == m2.end);
    }

    bool isValid(){
        return (start.x != 0 && start.y != 0 && end.x != 0 && end.y != 0);
    }

};


class Simulation
{
public:
    Simulation(std::string fname, std::string wname);
    ~Simulation();

    void initialize(std::string fname);
    void run();

    bool initializationCompleted();
    bool isRunning();
    void finish();

    void drawAgents();

    Grid* grid;
    bool stepByStep;
    bool runNextInStepByStepMode;

protected:

    void createAgents();
    void createAgentsTest();
    // plans the path for an individual agent after it has reached current goal
    void planPath(int agentId);
    void moveAgentsRandomly();
    // reads agent's path and executes it (blindly)
    void moveAgentsToGoal();
    // reads agent's path depending on algorithm used
    void moveAgentsToGoal(std::string Algorithm);
    // generates and returns a random valid pair of coordinates    
    Pick randomGoal();
    // sets a random amount of goals for every agent
    // used for testing purposes
    void setRandomGoals();
    // sets a random amount of goals for every agent
    // used for testing purposes, but with a fixed amount of goals
    void setRandomGoals(int amount);
    void setRandomPicks(int amount);
    // sets a random pick and delivery to agent
    void setRandomPickAndDelivery(int agentId);
    //generates an x and y number for a random goal
    //both values have to be a valid grid coordinate, and unnocupied
    //if the coordinate is not valid, generate new random numbers
    Goal randomPosition();
    Pick randomPick();
    // used to hard-coding goal for testing
    void setTestGoal(int x, int y); 
    // used to hard-coding goal for testing with agent id
    void setTestGoal(int x, int y, int agentId);
    // used to hard-code individual agent position after creation
    void setAgentPosition(int x, int y, int agentId); 
    // setting unique colors for each agent
    void setAgentColors();
    // seting Goal to an agent 
    void setAgentGoal(int x, int y, int agentId);
    // setting a color to the agent according to enum
    void setAgentColor(int agentId, int color);
    // reads the wave file and adds them to the wave list
    void readWaveFile(std::string waveName);
    // prints the waves_ list
    void printWaves();
    // test method for one wave
    void testWave();
    // internal method for grid painting
    void paintGoals(int agentId);
    // checks if the wave is completed, if it is, adds its ID to the completedWaves list.
    // then, checks if the waves_ list is empty, if it isn't, updates the currentWave, otherwise returns and end message 
    void getNewWave();
    // returns the next order of the current wave, if wave is empty, calls getNewWave method
    Order getNewOrder();
    // returns true if there are waves available, false otherwise
    bool wavesAvailable();
    // regular cycle through waves and orders, separated into a method to make testing other things easier
    void getNewGoals(int agentId);
    //adds all optimal path to each of the respective agents
    void addOptimalPaths(const std::vector<std::vector<Goal>>& solutions);
    // counts randomnly in a range to also randomly determine if a pickup is ready
    void setRandomPickupReady();
    // counts randomnly in a range to also randomly determine if a delivery is ready
    void setRandomDeliveryReady();
    // returns a valid pick if there is one, otherwise returns a null pick
    Pick getNextPickup();
    // returns a valid delivery if there is one, otherwise returns a null pick
    Pick getNextDelivery();
    // returns the first valid pickup location out of all the possible ones
    Pick getFirstPickup();
    void updateNextDeliveryColor();
    void updateNextPickupColor();
    void setCellColor(int x, int y, int r, int g, int b);
    Mission getNextPickupMission();
    Mission getNextDeliveryMission();
    void setPickupAvailable(int x, int y);
    void setDeliveryAvailable(int x, int y);
    

    unsigned int currentStep_;

    std::vector<Agent> agents_;
    std::list<Wave> waves_;
    std::list<Pick> pickup_list_;
    std::list<Pick> delivery_list_;
    typename std::list<int> completedWaves_;
    bool idle_;
    unsigned int numAgents_;
    // auxiliary variable to manipulate the waves
    Wave currentWave_;

    bool initialized_;
    bool running_;
    bool mustFinish_;
    bool pflag_ = true;

    Timer controlTimer;
};

#endif // Simulation_H
