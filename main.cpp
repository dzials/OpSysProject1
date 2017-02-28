#include <iostream>
#include <deque>
#include "Parse.h"

using namespace std;

/*
  Contributors: Stephen Dzialo, Yarden Ne'eman, Tyler Peláez
*/

// GLOBAL VARIABLES
// 
// map of processes
map<string,Process> processes;


// Queue of Processes in ready state
deque<Process> readyQueue;


// Number of processes
int n = 0; 
// Number of processors (cores)
int m = 1; 
// Time for context switch
int t_cs = 6; 
// Round-robin time slice
int t_slice = 94; 
// Time counter
int t = 0;

//For debugging Parser.h
void print_processes_map() {
    map<string, Process>::iterator itr;
    for(itr = processes.begin(); itr != processes.end(); ++itr) {
        cout << itr->first << " " << itr->second.arrival_time << " " << itr->second.burst_time << " " << itr->second.num_burst << " "  << itr->second.IOtime<< endl;
    }
}

void reset() {
    readyQueue = deque<Process>();
    t = 0;
}

string printQueue() {
    string build = "[Q";
    if (readyQueue.size() == 0) {
       
        return build += " <empty>]";
    }
    for (deque<Process>::iterator itr = readyQueue.begin(); itr != readyQueue.end(); itr++) {
        build += " " + itr->p_name;
    }
    return build + "]";
}

void FCFS() {
    int term_cnt = 0;
    int current_process_burst_time = 0;
    int context_switch_timer = t_cs / 2;
    Process* running;


    printf("time %dms: Simulator started for FCFS %s\n", t, (printQueue()).c_str());
    fflush (stdout);


    while (term_cnt < processes.size()) {

        // Find any processes that arrive at current time t and push process into queue
        map<string, Process>::iterator itr = processes.begin();
        while (itr->second.arrival_time <= t &&  itr != processes.end()) {
            if (itr->second.arrival_time == t) {
                readyQueue.push_back(itr->second);
                printf("time %dms: Process %s arrived and added to ready queue %s\n", t, itr->second.p_name.c_str(), (printQueue()).c_str());
                fflush (stdout);
            }
            itr++;
        }


        // Take care of ready processes and currently running process
        if (current_process_burst_time == 0 && context_switch_timer > 0) {
            if (context_switch_timer == t_cs) {
                running->endBurst();
                printf("time %dms: Process %s completed a CPU burst; %d bursts to go %s\n", t, running->p_name.c_str(), running->num_burst,  printQueue().c_str());
                if (running->num_burst == 0) {
                    printf("time %dms: Process %s terminated %s\n", t, running->p_name.c_str(), printQueue().c_str());
                    term_cnt++;
                } else {
                    printf("time %dms: Process %s switching out of CPU; will block on I/O until time %dms %s\n", t, running->p_name.c_str(), t + running->IOtime + (t_cs / 2), printQueue().c_str());

                }
                running = NULL;
            }
            // In a context switch out of the CPU, decrement timer
            context_switch_timer--;
        } else if (current_process_burst_time == 0 && context_switch_timer == 0){
            // Switch to new process
            running = &readyQueue.front();
            readyQueue.pop_front();

            printf("time %dms: Process %s started using the CPU %s\n", t, running->p_name.c_str(), printQueue().c_str());

            current_process_burst_time = running->burst_time;
            context_switch_timer = t_cs;
        } else {
            current_process_burst_time--;
        }

        // Handle blocking processes

        #if 0
            Penis18==========D~~~~dock(Penis2);
        #endif


        t++;
    }
    printf("time %dms: Simulator ended for FCFS\n", t);
    fflush (stdout);

}


int main(int argc, char const *argv[]) {
    if(argc == 1){
        cerr << "ERROR: Invalid arguments" << endl;
        cerr << "USAGE: ./a.out <input-file> <stats-output-file>" << endl;
        return 0;
    }
    processes = Parse::read((string)argv[1]);
    n = processes.size();

    reset();
    FCFS();

    return 0;
}
