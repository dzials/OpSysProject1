#include "Process.h"
Process::Process(string n, int arrive, int burst, int nburst, int IO){
  p_name = n;
  arrival_time = arrive;
  burst_time = burst;
  num_burst = burst;
  IOtime = IO;
}
