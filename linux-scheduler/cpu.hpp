//
//  cpu.hpp
//  linux-scheduler
//
//  Created by Cristopher Freitas on 9/30/15.
//  Copyright © 2015 Cristopher Freitas. All rights reserved.
//

#ifndef cpu_hpp
#define cpu_hpp

#include <iostream>
#include <iomanip>
#include <list>
#include "process.hpp"

class CPU {
    
public:
    CPU(list<Process> * processes);
    ~CPU();
    int tick();
    int submit();
    int sched();
    int isDone();
    int report();
    
private:
    int clock;
    Process * curr;
    list<Process> * startQueue;
    list<Process> * activeQueue;
    list<Process> * expiredQueue;
    list<Process> * ioQueue;
    list<Process> * finishedQueue;
    int isEmpty();
    int executeIO();
    
};

#endif /* cpu_hpp */
