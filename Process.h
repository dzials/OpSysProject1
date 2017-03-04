#include <string>
using namespace std;

class Process {
public:
  string p_name;
  int arrival_time;
  int burst_time;
  int remaining;
  int num_burst;
  int total_bursts;
  int IOtime;
  int ready_start;
  int total_wait;
  int preemption_cnt;
  int end_blocking_time;
  int start_burst;
  int turnaround;

  Process();
  Process(string n, int arrive, int burst, int nburst, int IO);
  void copy_process(const Process &p);
  void endBurst(int t, int t_cs);
  void reset();
};
