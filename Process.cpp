#include "Process.h"
#include <iostream>

Process::Process() {
    p_name = "";
    arrival_time = 0;
    burst_time = 0;
    remaining = 0;
    num_burst = 0;
    total_bursts = 0;
    IOtime = 0;
    end_blocking_time = -1;
    total_wait = 0;
    ready_start = 0;
    preemption_cnt = 0;
}
Process::Process(string n, int arrive, int burst, int nburst, int IO){
  p_name = n;
  arrival_time = arrive;
  burst_time = burst;
  remaining = burst;
  num_burst = nburst;
  total_bursts = nburst;
  IOtime = IO;
  end_blocking_time = -1;
  total_wait = 0;
  ready_start = 0;
  preemption_cnt = 0;
}

void Process::copy_process(const Process &p) {
    p_name = p.p_name;
    arrival_time = p.arrival_time;
    burst_time = p.burst_time;
    remaining = p.remaining;
    num_burst = p.num_burst;
    total_bursts = p.total_bursts;
    IOtime = p.IOtime;
    end_blocking_time = p.end_blocking_time;
    total_wait = p.total_wait;
    ready_start = p.ready_start;
    preemption_cnt = p.preemption_cnt;
}

void Process::endBurst(int t, int t_cs) {
	num_burst--;
	end_blocking_time = t + IOtime + (t_cs / 2);
	if (num_burst == 0) {
		end_blocking_time = -1;
	}
    remaining = burst_time;
	//std::cout << "enburstcall " << t  << " " << end_blocking_time << std::endl;
}
