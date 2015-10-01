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
#include <queue>
#include "process.hpp"

class CPU {
    int clock;
    Process * current;
    queue<Process> * readyQueue;
    queue<Process> * activeQueue;
    queue<Process> * expiredQueue;
    queue<Process> * ioQueue;
    queue<Process> * finishedQueue;
    
public:
    CPU();
    ~CPU();
};

#endif /* cpu_hpp */
