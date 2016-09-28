#ifndef AppendInformationToh22_h
#define AppendInformationToh22_h

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using std::vector;
using std::string;

 // Put your includes here 
#include "DBins.h" 
#include "DCut.h" 
#include "DEvent.h" 
#include "DSelection.h" 
#include "h22Event.h" 
#include "h22Reader.h" 

struct TrueInformation{
  int pid;
  double px, py, pz; 
};

typedef std::vector<TrueInformation> GeneratedEvents; 

class AppendInformationToh22 : public h22Reader {
    public:
  AppendInformationToh22();
  ~AppendInformationToh22();
  
  GeneratedEvents LoadEventFile(std::string inputFile);
  void Loop();
  void SaveNewFile(std::string outputFile);
};

inline void split(const string &s, char delim, vector<string> &elems) {
  stringstream ss;
  ss.str(s);
  string item;
  while (getline(ss, item, delim)) {
    elems.push_back(item);
  }
}

inline vector<string> split(const string &s, char delim) {
  vector<string> elems;
  split(s, delim, elems);
  return elems;
}

#endif

