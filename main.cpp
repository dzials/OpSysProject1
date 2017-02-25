#include <iostream>
#include "Parse.h"
using namespace std;

/*
  Contributors: Stephen Dzialo, Yarden Ne'eman, Tyler Peláez
*/
map<string,Process> processes;
int n,m, t_cs, t_slice;

int main(int argc, char const *argv[]) {
  if(argc == 1){
    cerr << "ERROR: Invalid arguments" << endl;
    cerr << "USAGE: ./a.out <input-file> <stats-output-file>" << endl;
    return 0;
  }
  processes = Parse::read((string)argv[1]);
  return 0;
}
