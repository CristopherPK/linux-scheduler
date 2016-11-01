//
//  process.hpp
//  linux-scheduler
//
//  Created by Cristopher Freitas on 9/30/15.
//  Copyright © 2015 Cristopher Freitas. All rights reserved.
//

#ifndef process_hpp
#define process_hpp

#include <iostream>
#include <list>
#include <string>

using namespace std;

class Process {
    
public:
    Process();
    Process(int pid, int nice, int arrivalTime,
            list<int> * cpuBursts, list<int> * ioBursts);
    Process & operator = (const Process & other);
    Process (const Process & other);
    ~Process();
    int getPid();
    int getArrivalTime();
    int getPriority();
    int getTimeSlice();
    int getEndTime();
    int getCpuTime();
    int getIOTime();
    int hasArrived(int clock);
    int end(int clock);
    int calcPriority();
    int calcTimeSlice();
    int execute(int clock);
    int executeIO();
    int cpuFinished();
    int isFinished();
    int isExhausted();
    string toString();
    
private:
    int pid;
    int arrivalTime;
    int startTime;
    int endTime;
    int priority;
    int originalPriority;
    int timeSlice;
    int totalCPU;
    int totalIO;
    list<int> * cpuBursts;
    list<int> * ioBursts;
    
};

#endif /* process_hpp */
