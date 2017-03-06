/*
  Contributors: Stephen Dzialo (dzials), Yarden Ne'eman (neemay), Tyler Peláez (pelaet)
*/

#include <string>
#include <cstring>
#include <map>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <utility>
#include "Process.h"

class Parse{
public:
  static map<string,Process> read(string file){
    map<string,Process> processes;
    string line;
    ifstream input;
  	input.open(file.c_str());
    while(!(input.eof())){ //Loop through each line until end of file
      getline(input,line);
      int ch = (int)(line[0]);
      if(isupper(ch)){ //Check for bad lines
        string n = line.substr(0,1);
        int arrive = 0, burst = 0, nburst = 0, IO = 0;
        char *cstrLine = &line[0];
        int breakNum = 0;
        char* value = strtok(cstrLine, "|");
        while(value != NULL) {
            if(breakNum == 0) {
                value = strtok(NULL, "|");
            }
            else if(breakNum == 1) {
                arrive = atoi(value);
                value = strtok(NULL, "|");
            }
            else if(breakNum == 2) {
                burst = atoi(value);
                value = strtok(NULL, "|");
            }
            else if(breakNum == 3) {
                nburst = atoi(value);
                value = strtok(NULL, "|");
            }
            else {
                IO = atoi(value);
                value = strtok(NULL, "|");
            }
            breakNum++;
        }
        Process* temp = new Process(n,arrive,burst,nburst,IO);
        processes.insert(pair<string,Process>(n,*temp));
      }
    }
    input.close();
    return processes;
  }
};
