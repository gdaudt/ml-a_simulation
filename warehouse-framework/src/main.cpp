#include <pthread.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <ostream>

#include "Simulation.h"
#include "Agent.h"
#include "Planner.h"

#include "GlutClass.h"

std::string filename;
std::string wavename;

pthread_mutex_t* mutex;

void* startSimulationThread (void* ref)
{
    Simulation* simulation=(Simulation*) ref;

    simulation->initialize(filename);

    while(simulation->isRunning()){
        simulation->run();
    }

	return NULL;
}

void* startGlutThread (void* ref)
{
    GlutClass* glut=GlutClass::getInstance();
    glut->setSimulation((Simulation*) ref);

    glut->initialize();

    glut->process();

	return NULL;
}

int main(int argc, char* argv[])
{
    srand(time(NULL));
    filename = "map.txt";

    if(argc > 2){
        if (!strncmp(argv[1], "-m", 2)) {
            filename = argv[2];
            wavename = argv[3];
        }
        else if (!strncmp(argv[1], "-M", 2)){
            filename = argv[2];
        }
    }

    Simulation* sim;
    sim = new Simulation(filename, wavename);

    pthread_t SimulationThread, glutThread;
    mutex = new pthread_mutex_t;
    pthread_mutex_unlock(mutex);

    pthread_create(&(SimulationThread),NULL,startSimulationThread,(void*)sim);
    pthread_create(&(glutThread),NULL,startGlutThread,(void*)sim);

    pthread_join(SimulationThread, 0);
    pthread_join(glutThread, 0);

    // Agent* agent = new Agent(1, 1, 0, Agent::Right);
    // std::cout << "starting plan with agent A" << std::endl;
    // Planner planner;
    // planner.set_grid(sim->grid);    
    // planner.astar(agent, 11, 11);
    // agent->print_path(); 
    // unsigned int w = sim->grid->getMapWidth();
    // unsigned int h = sim->grid->getMapHeight();
    // for(int i = 0; i < h; i++){
    //     for(int ii = 0; ii<w; ii++){
    //         sim->grid->isValid(ii, i);
    //     }
    // }    
    return 0;
}

