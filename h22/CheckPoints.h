#ifndef checkpoints_h 
#define checkpoints_h 

// standard
#include <iostream>
#include <map>

// root 
#include "TFile.h"
#include "TH1.h"

struct CheckPoint{
  std::string name; 
  int         counts; 
};

class CheckPoints {

 public:
  CheckPoints(); 
  ~CheckPoints(); 

  void CheckIn();
  void AddCheckPoint(std::string name);
  void PrintSummary();
  void Save(TFile *out); 
  void Reset(); 

protected:
  int                        fCurrentAxis; 
  int                        fNumberOfAxes; 
  std::map<int, CheckPoint>  fCheckPoints; 
  

};

#endif 
