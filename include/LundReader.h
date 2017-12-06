#ifndef lund_reader_h
#define lund_reader_h

#include <iostream>
#include <fstream>
#include <vector>

#include "LundEvent.h"

class LundReader{
 public:
  LundReader();
  ~LundReader();
  
  LundReader(std::string inputFilename);

  // Member Variables   
 public:
  int currentEvent; 
  std::vector<LundEvent> events; 
  
  // Member Functions 
 public:
  void LoadEvents(std::string inputFilename);
  void AddEvent(LundEvent thisEvent){ events.push_back(thisEvent); }
  int GetNumberOfEvents(){ return events.size(); }
  LundEvent GetEvent(){ return events[currentEvent]; }
  LundEvent GetEvent(int eventNumber){return events[eventNumber];}
  bool HasEvent(){ if (currentEvent < (GetNumberOfEvents()-1)){ currentEvent++; return true; } return false; }

 private:
  bool isHeader(std::string lineOfInterest);
  bool isParticle(std::string lineOfInterest);
  std::vector<std::string> parseLine(std::string line);

};

#endif 
