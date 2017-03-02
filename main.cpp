#include <iostream>
#include <deque>
#include <fstream>
#include <iomanip>
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
//Number of context switches
int cs = 0;

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
    cs = 0;
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
    float bursts = 0;
    for (map<string, Process>::iterator itr = processes.begin(); itr != processes.end(); ++itr)
    {
        sum += (itr->second.total_wait);
        bursts += itr->second.total_bursts;
    }
    return sum / bursts;
}

float calculate_avg_CPU() {
    float sum = 0;
    float bursts = 0;
    for (map<string, Process>::iterator itr = processes.begin(); itr != processes.end(); ++itr)
    {
        sum += ((itr->second.burst_time) * itr->second.total_bursts);
        bursts += (itr->second.total_bursts);
    }
    return sum / bursts;
}

float calculate_turnaround() {

    return calculate_waits() + calculate_avg_CPU() + (t_cs/2);
}

int calculate_pre() {
    int total = 0;
    for (map<string, Process>::iterator itr = processes.begin(); itr != processes.end(); ++itr)
    {
        total += (itr->second.preemption_cnt);
    }
    return total;
}

void print_statistics(const char* filename, int x) {
    ofstream output;
    output.open(filename);
    if(x == 1)
        output << "Algorithm FCFS" << endl;
    else if(x == 2)
        output << "Algorithm SRT" << endl;
    else
        output << "Algorithm RR" << endl;

    output << "-- average CPU burst time: " << setprecision(2) << fixed << calculate_avg_CPU() << " ms" << endl;
    output << "-- average wait time: " << setprecision(2) << fixed << calculate_waits() << " ms" << endl;
    output << "-- average CPU burst time: " << setprecision(2) << fixed <<  calculate_turnaround() << " ms" << endl;
    output << "-- total number of context siwtches: " << cs << endl;
    output << "-- total number of preemptions: " << calculate_pre() << endl;
    output.close();
}
void FCFS() {
    int term_cnt = 0;
    int current_process_burst_time = 0;
    int context_switch_timer = t_cs / 2;
    Process* running = NULL;


    printf("time %dms: Simulator started for FCFS %s\n", t, (printQueue()).c_str());
    fflush (stdout);


    while (term_cnt < n) {

        // Find any processes that arrive at current time t and push process into queue
        map<string, Process>::iterator itr = processes.begin();
        while (itr->second.arrival_time <= t &&  itr != processes.end()) {
            if (itr->second.arrival_time == t) {
                itr->second.ready_start = t + (t_cs/2);
                readyQueue.push_back(itr->second);
                printf("time %dms: Process %s arrived and added to ready queue %s\n", t, itr->second.p_name.c_str(), (printQueue()).c_str());
                fflush (stdout);
            }
            itr++;
        }


        // Handle blocking processes
        for (map<string, Process>::iterator itr2 = processes.begin(); itr2 != processes.end(); ++itr2) {
            if (t == itr2->second.end_blocking_time) {
                itr2->second.end_blocking_time = -1;
                readyQueue.push_back(itr2->second);
                itr2->second.ready_start = t + (t_cs/2);
                printf("time %dms: Process %s completed I/O; added to ready queue %s\n", t, itr2->second.p_name.c_str(), printQueue().c_str());
                fflush(stdout);
                if (running == NULL) {
                    context_switch_timer = t_cs / 2;
                }
            }
        }


        // Take care of ready processes and currently running process
        if (current_process_burst_time == 0 && context_switch_timer > 0) {
            // In a context switch out of the CPU, decrement timer
            context_switch_timer--;
        } else if (current_process_burst_time == 0 && context_switch_timer == 0){
            // Switch to new process
            if (!readyQueue.empty()) {
                cs++;
                running = &readyQueue.front();
                readyQueue.pop_front();
                processes.find(running->p_name)->second.total_wait += (t - (processes.find(running->p_name)->second.ready_start));

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
    print_statistics(argv[2], 1);

    return 0;
}
