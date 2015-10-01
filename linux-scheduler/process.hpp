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

using namespace std;

class Process {
    int pid;
    int nice;
    int arrivalTime;
    int startTime;
    int endTime;
    int priority;
    int timeSlice;
    list<int> * cpuBursts;
    list<int> * ioBursts;
    
public:
    Process();
    Process(int pid, int nice, int arrivalTime,
            list<int> * cputBursts, list<int> * ioBursts);
    ~Process();
    int start(int clock);
    int end(int clock);
    int totalCPU();
    int totalIO();
};

#endif /* process_hpp */
