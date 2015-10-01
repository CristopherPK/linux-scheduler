//
//  main.cpp
//  linux-scheduler
//
//  Created by Cristopher Freitas on 9/30/15.
//  Copyright © 2015 Cristopher Freitas. All rights reserved.
//

#include <iostream>

int readFile(FILE *f){
    return 0;
}

//The program will receive the filename on the execution.
int main(int argc, const char * argv[]) {
    
    //Defining total CPU/IO time.
    int totalCPU;
    int totalIO;
    
    //Bonus points are given to processes that either use too much or too little CPU time.
    int bonus;
    
    if(totalCPU < totalIO){
        bonus = (int)(((1- totalCPU/(double)totalIO)*(-5))-0.5);
    } else {
        bonus = (int)(((1- totalCPU/(double)totalIO)*(5))+0.5);
    }
    
    //Defining nice number.
    int nice;
    
    //Defining priority.
    int priority;
    
    //Calculating original/static priority based on nice value.
    int originalPriority = (int)(((nice + 20)/39)*30 + 0.5) + 105;
    
    //Calculating new dynamic priority after process exhausts its time slice.
    priority = originalPriority + bonus;
    
    //Calculating time slice.
    int timeSlice;
    timeSlice = (int)((1 - priority/150)*395*0.5) + 5;
    
    return 0;
}
