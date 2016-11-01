//
//  process.cpp
//  linux-scheduler
//
//  Created by Cristopher Freitas on 9/30/15.
//  Copyright © 2015 Cristopher Freitas. All rights reserved.
//

#include "process.hpp"

Process:: Process(){
    
}

Process & Process::operator=(const Process &other){
    if (this != &other) {
        
        this->pid = other.pid;
        this->arrivalTime = other.arrivalTime;
        this->endTime = other.endTime;
        this->startTime = other.startTime;
        this->totalCPU = other.totalCPU;
        this->totalIO = other.totalIO;
        this->priority = other.priority;
        this->originalPriority = other.originalPriority;
        this->timeSlice = other.timeSlice;
        
        this->cpuBursts = new list<int>(*other.cpuBursts);
        this->ioBursts = new list<int>(*other.ioBursts);
    }
    
    return *this;
}

Process::Process(const Process & other){
    if (this != &other) {
        this->pid = other.pid;
        this->arrivalTime = other.arrivalTime;
        this->endTime = other.endTime;
        this->startTime = other.startTime;
        this->totalCPU = other.totalCPU;
        this->totalIO = other.totalIO;
        this->priority = other.priority;
        this->originalPriority = other.originalPriority;
        this->timeSlice = other.timeSlice;
        
        this->cpuBursts = new list<int>(*other.cpuBursts);
        this->ioBursts = new list<int>(*other.ioBursts);
    }
}

Process::Process(int pid, int nice, int arrivalTime,
                  list<int> * cpuBursts, list<int> * ioBursts){

    //Setting attributes.
    this->pid = pid;
    this->arrivalTime = arrivalTime;
    this->cpuBursts = cpuBursts;
    this->ioBursts = ioBursts;
    this->startTime = -1;
    this->totalCPU = 0;
    this->totalIO = 0;

    float x = (float)(nice + 20)/39;
    //Calculating original/static priority based on nice value.
    this->originalPriority = (x*30 + 0.5) + 105;
    
    this->priority = this->originalPriority;
    
}

Process::~Process(){
    this->cpuBursts = 0;
    delete this->cpuBursts;
    this->ioBursts = 0;
    delete this->ioBursts;
}

int Process::getPid(){
    return this->pid;
}

int Process::getArrivalTime(){
    return this->arrivalTime;
}

int Process::getPriority(){
    return this->priority;
}

int Process::getTimeSlice(){
    return this->timeSlice;
}

int Process::getEndTime(){
    return this->endTime;
}

int Process::getCpuTime(){
    return this->totalCPU;
}

int Process::getIOTime(){
    return this->totalIO;
}

int Process::hasArrived(int clock){
    if(clock >= this->arrivalTime) {
        return 0;
    } else {
        return -1;
    }
}

int Process::end(int clock){
    this->endTime = clock;
    return 0;
}

int Process::calcPriority(){
    int bonus = 0;
    
    //cout << this->totalCPU << " / " << this->totalIO << endl;
    
    if(this->totalCPU < this->totalIO){
        float x =(float)this->totalCPU/this->totalIO;
        bonus = ( (1 - x)*(-5) ) - 0.5;
    } else if(this->totalIO != 0){
        float x =(float)this->totalCPU/this->totalIO;
        bonus = ( (1 - x) * 5 ) + 0.5;
    }
    
    //cout << "bonus" << bonus << endl;
    
    //Calculating new dynamic priority after process exhausts its time slice.
    this->priority = this->originalPriority + bonus;
    
    return 0;
}

int Process::calcTimeSlice(){
    this->timeSlice = (int)((1 - ((float)this->priority/150))*395*0.5) + 5;
    return this->timeSlice;
    
}

int Process::execute(int clock){
    if(this->startTime == -1)
        this->startTime = clock;
    
    if(this->timeSlice > 0 && this->cpuBursts->size() > 0){
        //cout << "Time Slice: " << timeSlice << " CPU Time: " << this->cpuBursts->front() <<endl;
        this->cpuBursts->front()--;
        this->totalCPU++;
        this->timeSlice--;
    }
    return 0;
}

int Process::executeIO(){
    //cout << this->ioBursts << endl;
    if(this->ioBursts->size() > 0){
        this->ioBursts->front()--;
        this->totalIO++;
        if(this->ioBursts->front() == 0){
            this->ioBursts->pop_front();
            return 0;
        }
    } else if(this->ioBursts->size() == 0){
        return 0;
    }
    return -1;
}
int Process::cpuFinished(){
    if(this->cpuBursts->front() == 0){
        this->cpuBursts->pop_front();
        return 0;
    } else {
        return -1;
    }
}

int Process::isFinished(){
    if(this->cpuBursts->size() == 0 && this->ioBursts->size()==0){
        return 0;
    } else {
        return -1;
    }
}

int Process::isExhausted(){
    if(this->timeSlice == 0)
        return 0;
    else return -1;
}

string Process::toString(){
    string buffer;
    buffer = "pid: " + to_string(this->pid) + " arrival time: " + to_string(this->arrivalTime);
    return buffer;
}