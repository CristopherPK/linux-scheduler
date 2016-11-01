//
//  cpu.cpp
//  linux-scheduler
//
//  Created by Cristopher Freitas on 9/30/15.
//  Copyright © 2015 Cristopher Freitas. All rights reserved.
//

#include "cpu.hpp"

//Comparing arrival time.
bool comparePriority(Process& first, Process& second)
{
    if(first.getPriority() < second.getPriority())
        return true;
    else return false;
    
}

//Constructor with processes lists already order by arrival time.
CPU::CPU(list<Process> * processes){
    this->clock = 0;
    this->curr = 0;
    this->activeQueue = new list<Process>;
    this->expiredQueue = new list<Process>;
    this->finishedQueue = new list<Process>;
    this->ioQueue = new list<Process>;
    this->startQueue = processes;
}

CPU::~CPU(){
    delete this->activeQueue;
    delete this->expiredQueue;
    delete this->finishedQueue;
    delete this->ioQueue;
    delete this->startQueue;
}
//Ticking clock.
int CPU::tick(){
    //cout << clock << endl;
    return this->clock++;
}

//Checking if the CPU is empty.
int CPU::isEmpty(){
    if(this->curr == 0){
        return 0;
    } else {
        return -1;
    }
}

//Submit those process which arrivaltime >= clock.
int CPU::submit(){
    while(this->startQueue->size() > 0){
        Process * p = &this->startQueue->front();
        if(p->hasArrived(clock) == 0){
            p->calcTimeSlice();
            this->activeQueue->push_back(*p);
            //[clock] <pid> Enters ready queue (Priority: _, TimeSlice: _)
            cout << "[" << this->clock << "]";
            cout << "<" << p->getPid() << ">";
            cout << " Enters ready queue ";
            cout << "(Priority: " << p->getPriority();
            cout << ", TimeSlice: " << p->getTimeSlice() << ")" << endl;
            this->startQueue->pop_front();
        } else {
            return -1;
        }
    }
    return 0;
}

int CPU::executeIO(){
    //This can cause you troubles.
    list<Process> * aux = new list<Process>;
    
    while(this->ioQueue->size() > 0){
        
        Process * p = new Process(this->ioQueue->front());
        if(p->executeIO() == 0){
            if(p->isExhausted() == 0){
                p->calcPriority();
                p->calcTimeSlice();
                this->expiredQueue->push_back(*p);
                //[clock] <pid> Finishes IO and moves to the Expired Queue (Priority : _, TimeSlice : _)
                cout << "[" << this->clock << "]";
                cout << "<" << p->getPid() << ">";
                cout << " Finishes IO and moves and moves to the Expired Queue ";
                cout << "(Priority: " << p->getPriority();
                cout << ", TimeSlice: " << p->getTimeSlice() << ")" << endl;
                
            } else {
                this->activeQueue->push_back(*p);
                //[clock] <pid> Finishes IO and moves to the Ready Queue
                cout << "[" << this->clock << "]";
                cout << "<" << p->getPid() << ">";
                cout << " Finishes IO and moves to the Ready Queue" << endl;
                
            }
        } else {
            aux->push_back(*p);
        }
        
        this->ioQueue->pop_front();
    }
    
    this->ioQueue = aux;
    
    return 0;
}

//Calling "scheduler" to execute a process.
int CPU::sched(){
//    
//    //Checking if some process arrived at the Ready Queue.
//    if(this->activeQueue->size() == 0 && this->ioQueue == 0 && this->curr == 0)
//        return 0;
    
    //Sort the active queue by priority
    if(this->activeQueue->size() > 0){
        this->activeQueue->sort(comparePriority);
    
        //Checking if the CPU is idle.
        if(this->isEmpty() == 0){
            this->curr = new Process(this->activeQueue->front());
            //[clock] <pid> Enters the CPU
            cout << "[" << this->clock << "]";
            cout << "<" << this->curr->getPid() << ">";
            cout << " Enters the CPU" << endl;
            
            this->activeQueue->pop_front();
            
        }
    
        //Checking if some process with lower priority has arrived at the active queue.
        if(this->activeQueue->front().getPriority() < this->curr->getPriority()){
            this->activeQueue->push_back(*this->curr);
            this->curr = new Process(this->activeQueue->front());
            //[clock] <pid> Preempts Process pid
            cout << "[" << this->clock << "]";
            cout << "<" << this->curr->getPid() << ">";
            cout << " Preempts Process " << this->activeQueue->back().getPid() << endl;
            
            this->activeQueue->pop_front();
        }
    }
    

    if(this->curr != 0){
        //Execute the current process.
        this->curr->execute(clock);
    
        //Check if the CPU Burst of the current process is exhausted.
        if(this->curr->cpuFinished() == 0){
        //Check if the current process is finished.
            if(this->curr->isFinished() == 0){
                this->curr->end(clock);
                
                //[clock] <pid> Finishes and moves to the Finished Queue.
                cout << "[" << this->clock << "]";
                cout << "<" << this->curr->getPid() << ">";
                cout << " Finishes and moves to the Finished Queue" << endl;
                
                this->finishedQueue->push_back(*this->curr);
                this->curr = 0;
            }
            
            else {
                //[clock] <pid> Moves to the IO Queue
                cout << "[" << this->clock << "]";
                cout << "<" << this->curr->getPid() << ">";
                cout << " Moves to the IO Queue" << endl;
                this->ioQueue->push_back(*this->curr);
                this->curr = 0;
            }
        }
        
        else if(this->curr->isExhausted() == 0){
            this->curr->calcPriority();
            this->curr->calcTimeSlice();
            this->expiredQueue->push_back(*this->curr);
            //[clock] <pid> Finishes its time slice and moves to the Expired Queue ( Priority : _, Timeslice : _)
            cout << "[" << this->clock << "]";
            cout << "<" << this->curr->getPid() << ">";
            cout << " Finishes its time slice and moves to the Expired Queue ";
            cout << "(Priority: " << this->curr->getPriority();
            cout << ", TimeSlice: " << this->curr->getTimeSlice() << ")" << endl;
            this->curr = 0;
        }
    }
    
    //Perform IO.
    if(this->ioQueue->size() > 0){
        //cout << "executing IO" << endl;
        this->executeIO();
    }
    
    return 0;
}

int CPU::isDone(){
//    cout << this->curr << endl;
//    cout << this->startQueue->size() << endl;
//    cout << this->activeQueue->size() << endl;
//    cout << this->expiredQueue->size() << endl;
//    cout << this->ioQueue->size() << endl;
    if(this->curr == 0 && this->startQueue->size() == 0 && this->activeQueue->size() == 0
       && this->expiredQueue->size() == 0 && this->ioQueue->size() == 0){
        return 0;
    } else if(this->curr == 0 && this->activeQueue->size() == 0 && this->expiredQueue->size() > 0){
        //cout << "aqui"<< endl;
        list<Process> * aux = this->activeQueue;
        this->activeQueue = this->expiredQueue;
        this->expiredQueue = aux;
        //[ clock ] ∗∗∗ Queue Swap
        cout << "[" << this->clock << "]";
        cout << " ∗∗∗ Queue Swap" << endl;
    }
    return -1;
}

int CPU::report(){
    
    int nprocess = 0;
    float avgTatime = 0;
    float avgCputime = 0;
    float avgWt = 0;
    float avgCpuUt = 0;
    
    cout << "##### REPORT #####" << fixed << endl;
    while(this->finishedQueue->size() > 0){
        Process * p = &this->finishedQueue->front();
        nprocess++;
        cout << "Process <" << p->getPid() << ">" << endl;
        int tatime = p->getEndTime() - p->getArrivalTime();
        avgTatime += tatime;
        cout << "Turnaround Time: " << tatime << endl;
        
        int cputime = p->getCpuTime();
        avgCputime += cputime;
        cout << "Total CPU Time: " << cputime << endl;
        
        int wt = tatime - cputime - p->getIOTime();
        avgWt += wt;
        cout << "Waiting Time: " << wt << endl;
        
        float cut = float(cputime)/float(tatime);
        cut = cut*100;
        
        avgCpuUt+=cut;
        cout << "CPU utilization: " << setprecision(1) << cut << endl;
        finishedQueue->pop_front();
    }
    cout << "#################" << endl;
    avgTatime = avgTatime/nprocess;
    cout << "Average Turnaround Time: " << setprecision(3) << avgTatime << endl;
    avgWt = avgWt/nprocess;
    cout << "Average Waiting Time: " << setprecision(3) << avgWt << endl;
    avgCpuUt = avgCpuUt/nprocess;
    cout << "Average CPU Utilization: " << setprecision(3) << avgCpuUt << endl;
    cout << "#################" << endl;
    
    return 0;
}
