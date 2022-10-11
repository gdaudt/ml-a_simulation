#ifndef UTILS_H
#define UTILS_H

#include <sys/time.h>
#include <string>
#include <iostream>


class Timer{
    public:
        Timer();

        void startCounting();
        void startLap();
        void stopCounting();

        float getTotalTime();
        float getLapTime();

        void waitTime(float t);

    private:
        struct timeval tstart, tlapstart, tnow;
};

struct Goal{
    int x;
    int y;

    bool operator==(const Goal& other) const {
        return x == other.x and y == other.y;
    }

    void print(){
        std::cout << "Goal: (" << x << ", " << y << ")" << std::endl;
    }
};

struct Conflict{
    std::pair<int, int> conflictedAgentsID;
    Goal cell1;
    Goal cell2;
    int time;

    Conflict(){
        conflictedAgentsID = std::make_pair(-1, -1);
    }

    Conflict(int firstAgentID, int secondAgentID, Goal cell1, Goal cell2, int time) :
        cell1(cell1), cell2(cell2), time(time){
            conflictedAgentsID = std::make_pair(firstAgentID, secondAgentID);
        }

    void print(){
        std::cout << "Conflict between agents " << conflictedAgentsID.first << " and " << conflictedAgentsID.second << " at time " << time << std::endl;
        std::cout << "Cell 1: ";
        cell1.print();
        std::cout << "Cell 2: ";
        cell2.print();
    }
};

#endif
