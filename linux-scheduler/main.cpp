//
//  main.cpp
//  linux-scheduler
//
//  Created by Cristopher Freitas on 9/30/15.
//  Copyright © 2015 Cristopher Freitas. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <list>

#include "process.hpp"
#include "cpu.hpp"

using namespace std;

//Comparing arrival time.
bool compareAt(Process& first, Process& second)
{
    if(first.getArrivalTime() < second.getArrivalTime())
        return true;
    else return false;
    
}

list<Process> * readInput(){
    
    //ifstream f = ifstream("/Users/cristopher/Documents/linux-scheduler/linux-scheduler/input.txt");
    //Buffering input.
    string output;
    int c = 0;
    int pid = 0;
    int cpubursts = 0;
    list<int> * cpuList = new list<int>;
    int iobursts = 0;
    list<int> * ioList = new list<int>;
    int nice = 0;
    int arrival = 0;
    
    list<Process> * processes = new list<Process>;
    
    while (true) {
        
        cin >> output;
        //Checking the end of file signal.
        if(!output.compare("***"))
            break;
        
        //Checking the input positions.
        if(c == 0){
            nice = stoi(output);
            c++;
        } else if(c == 1){
            arrival = stoi(output);
            c++;
        } else if(c == 2){
            //cout << "n cpu burst: " << output << endl;
            cpubursts = stoi(output);
            iobursts = cpubursts - 1;
            c++;
        } else {
            if(cpubursts > 0){
                //cout << "cpu burst #" << cpubursts << ":" << output << endl;
                cpubursts--;
                cpuList->push_back(stoi(output));
                if(iobursts == 0){
                    c = 0;
//                    cout << "PROCESS ####" << endl;
//                    cout << "pid value: " << pid << endl;
//                    cout << "nice value:" << nice << endl;
//                    cout << "arrival time:" << arrival << endl;
                    Process * p = new Process(pid, nice, arrival, cpuList, ioList);
                    processes->push_back(*p);
                    cpuList = new list<int>;
                    ioList = new list<int>;
                    pid++;
                }
            } else if(iobursts > 0){
                //cout << "io burst" << iobursts << ":" << output << endl;
                iobursts--;
                ioList->push_back(stoi(output));
                if(iobursts == 0){
                    c = 0;
//                    cout << "PROCESS ####" << endl;
//                    cout << "pid value: " << pid << endl;
//                    cout << "nice value:" << nice << endl;
//                    cout << "arrival time:" << arrival << endl;
                    Process * p = new Process(pid, nice, arrival, cpuList, ioList);
                    processes->push_back(*p);
                    cpuList = new list<int>;
                    ioList = new list<int>;
                    pid++;
                }
            }
        }
    }
    
    return processes;
    
}

//The program will receive the filename on the execution.
int main(int argc, const char * argv[]) {
    
    //Reading processes input file and putting into a list.
    list<Process> * processes = readInput();
    
    //Sorting processes list according to arrival time.
    processes->sort(compareAt);
    
    //Printing the processes list after sorted.
//    while(processes->size() > 0){
//        Process p = processes->front();
//        cout << ' ' << p.toString() << endl;
//        processes->pop_front();
//    }
    
    //CPU initialized.
    CPU * cpu = new CPU(processes);
    //cout << "CPU initialized" << endl;

    while(true){
        cpu->submit();
        //cout << "Submit processes" << endl;
        cpu->sched();
        if(cpu->isDone() == 0)
            break;
        cpu->tick();
    }
    
    //Printing report.
    cpu->report();
    
    processes = 0;
    delete processes;
    cpu = 0;
    delete cpu;
    
    return 0;
}
