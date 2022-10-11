#include "Utils.h"

#include <unistd.h>
#include <stdlib.h>
#include <sstream>
#include "Agent.h"


//////////////////////////////////
///// METHODS OF CLASS TIMER /////
//////////////////////////////////

Timer::Timer()
{
    startCounting();
}

void Timer::startCounting()
{
    gettimeofday(&tstart, NULL);
    gettimeofday(&tlapstart, NULL);
}

void Timer::startLap()
{
    gettimeofday(&tlapstart, NULL);
}

void Timer::stopCounting()
{
    gettimeofday(&tnow, NULL);
}

float Timer::getTotalTime()
{
    gettimeofday(&tnow, NULL);

    if (tstart.tv_usec > tnow.tv_usec) {
        tnow.tv_usec += 1000000;
        tnow.tv_sec--;
    }

    return (float)(tnow.tv_sec - tstart.tv_sec) +
           ((float)tnow.tv_usec - (float)tstart.tv_usec)/1000000.0;
}

float Timer::getLapTime()
{
    gettimeofday(&tnow, NULL);

    if (tlapstart.tv_usec > tnow.tv_usec) {
        tnow.tv_usec += 1000000;
        tnow.tv_sec--;
    }

    float t = (float)(tnow.tv_sec - tlapstart.tv_sec) +
              ((float)tnow.tv_usec - (float)tlapstart.tv_usec)/1000000.0;

    tlapstart = tnow;
    return t;
}

void Timer::waitTime(float t){
    float l;
    do{
        usleep(1000);
        l = getLapTime();
    }while(l < t);
    startLap();
}

