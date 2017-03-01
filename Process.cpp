#include "Process.h"
#include <iostream>
Process::Process(string n, int arrive, int burst, int nburst, int IO){
  p_name = n;
  arrival_time = arrive;
  burst_time = burst;
  num_burst = nburst;
  total_bursts = nburst;
  IOtime = IO;
  end_blocking_time = -1;
  total_wait = 0;
  ready_start = 0;
  preemption_cnt = 0;
}

void Process::endBurst(int t, int t_cs) {
	num_burst--;
	end_blocking_time = t + IOtime + (t_cs / 2);
	if (num_burst == 0) {
		end_blocking_time = -1;
	}
	//std::cout << "enburstcall " << t  << " " << end_blocking_time << std::endl;
}
