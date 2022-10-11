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
    //generates an x and y number for a random goal
    //both values have to be a valid grid coordinate, and unnocupied
    //if the coordinate is not valid, generate new random numbers
    Goal randomPosition();
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

    unsigned int currentStep_;

    std::vector<Agent> agents_;
    std::list<Wave> waves_;
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
