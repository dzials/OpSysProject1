#include <string>
using namespace std;

class Process {
public:
  string p_name;
  int arrival_time;
  int burst_time;
  int num_burst;
  int IOtime;
  int end_blocking_time;

  Process(string n, int arrive, int burst, int nburst, int IO);

  void endBurst(int t, int t_cs);
};
