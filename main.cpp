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

float calculate_waits() {
    float sum = 0;
    for (map<string, Process> iterator itr = processes.begin(); itr != processes.end(); ++itr)
    {
        sum += itr->second.total_wait;
    }
    return sum / n;
}



float calculate_turnaround() {
    
}

void FCFS() {
    int term_cnt = 0;
    int current_process_burst_time = 0;
    int context_switch_timer = t_cs / 2;
    Process* running;


    printf("time %dms: Simulator started for FCFS %s\n", t, (printQueue()).c_str());
    fflush (stdout);


    while (term_cnt < n) {

        // Find any processes that arrive at current time t and push process into queue
        map<string, Process>::iterator itr = processes.begin();
        while (itr->second.arrival_time <= t &&  itr != processes.end()) {
            if (itr->second.arrival_time == t) {
                readyQueue.push_back(itr->second);
                itr->second.ready_start = t;
                printf("time %dms: Process %s arrived and added to ready queue %s\n", t, itr->second.p_name.c_str(), (printQueue()).c_str());
                fflush (stdout);
            }
            itr++;
        }


        // Handle blocking processes
        for (map<string, Process>::iterator itr2 = processes.begin(); itr2 != processes.end(); ++itr2) {
            /*
            if (t == 605) {
                cout << printQueue() << " " << itr2->second.end_blocking_time << endl;
            }
            if (t == 606) {
                cout << printQueue() << " " << itr2->second.end_blocking_time << endl;
            }
            if (t == 607) {
                cout << printQueue() << " " << itr2->second.end_blocking_time << endl;
            }
            */
            if (t == itr2->second.end_blocking_time) {
                itr2->second.end_blocking_time = -1;
                readyQueue.push_back(itr2->second);
                itr2->second.ready_start = t;
                printf("time %dms: Process %s completed I/O; added to ready queue %s\n", t, itr2->second.p_name.c_str(), printQueue().c_str());
                fflush(stdout);
                if (running == NULL) {
                    context_switch_timer = t_cs / 2;
                }
            }
            //cout << "time is: " << t << " " << itr2->second.p_name << " " << printQueue() << endl;
        }


        // Take care of ready processes and currently running process
        if (current_process_burst_time == 0 && context_switch_timer > 0) {
            // In a context switch out of the CPU, decrement timer
            context_switch_timer--;
        } else if (current_process_burst_time == 0 && context_switch_timer == 0){
            // Switch to new process
            if (!readyQueue.empty()) {

                running = &readyQueue.front();
                readyQueue.pop_front();
                processes.find(running->p_name)->second.total_wait += t - processes.find(running->p_name)->second.ready_start;

                printf("time %dms: Process %s started using the CPU %s\n", t, running->p_name.c_str(), printQueue().c_str());
                fflush(stdout);

                current_process_burst_time = running->burst_time;
                context_switch_timer = t_cs -1;
            }
        } else {
            current_process_burst_time--;
            if (current_process_burst_time == 0 ) {
                processes.find(running->p_name)->second.endBurst(t, t_cs);
                if (processes.find(running->p_name)->second.num_burst == 0) {
                    printf("time %dms: Process %s terminated %s\n", t, running->p_name.c_str(), printQueue().c_str());
                    fflush(stdout);
                    term_cnt++;
                    if (term_cnt == n) {
                        t += (t_cs / 2) - 1;
                    }
                    
                } else {
                    if (processes.find(running->p_name)->second.num_burst == 1) {
                        printf("time %dms: Process %s completed a CPU burst; %d burst to go %s\n", t, running->p_name.c_str(), processes.find(running->p_name)->second.num_burst,  printQueue().c_str());
                        fflush(stdout);
                    } else {
                        printf("time %dms: Process %s completed a CPU burst; %d bursts to go %s\n", t, running->p_name.c_str(), processes.find(running->p_name)->second.num_burst,  printQueue().c_str());
                        fflush(stdout);
                    }
                    printf("time %dms: Process %s switching out of CPU; will block on I/O until time %dms %s\n", t, running->p_name.c_str(), t + running->IOtime + (t_cs / 2), printQueue().c_str());
                    fflush(stdout);

                }
                running = NULL;
            }
        }


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
