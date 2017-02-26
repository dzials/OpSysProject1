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
      if(!(line[0] == '#')&&(!line[0] == ' ')){ //Check for bad lines
        if(isalnum(line[0])){
          string n = line.substr(0,1);
          int arrive, burst, nburst, IO;
          char *cstrLine = &line[0u];
          int breakNum = 0;
          for(unsigned int i = 0; i < line.size(); ++i){
            char* temp;
            if(line[i] == '|') //Check if current character is a | to see what data is being read
              breakNum++;
            else if(breakNum == 1){ //Extract Arrival time
              temp = strtok(cstrLine,"|");
              arrive = atoi(temp);
              breakNum++;
            }
            else if(breakNum == 2){ //Extract CPU Burst Time
              temp = strtok(cstrLine,"|");
              burst = atoi(temp);
              breakNum++;
            }
            else if(breakNum == 3){ //Extract Num Bursts
              temp = strtok(cstrLine,"|");
              nburst = atoi(temp);
              breakNum++;
            }
            else if(breakNum == 4){ //Extract IO time
              temp = strtok(cstrLine,"|");
              IO = atoi(temp);
              breakNum++;
            }
          }
          Process* temp = new Process(n,arrive,burst,nburst,IO);
          processes.insert(pair<string,Process>(n,*temp));
        }
      }
    }
    input.close();
    return processes;
  }
};
