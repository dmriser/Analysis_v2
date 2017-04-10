//////////////////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  August 14, 2016 

  DataEventSelector.h -> Defines class structure for the 
  selection utility which combines DataEventCut objects to 
  say yes or no. 

*/
//////////////////////////////////////////////////////

#ifndef dselection_h
#define dselection_h

// c++ includes
#include <iostream>
#include <map>
#include <vector>

// my includes
#include "DataEventCut.h"
#include "h22Event.h"

class DataEventSelector
{
 public:
  DataEventSelector();
  ~DataEventSelector();

  std::string selection_name; 
  std::vector<DataEventCut*> cuts;

 public:
  void AddCut(DataEventCut *this_cut){ cuts.push_back(this_cut); }
  void SetName(std::string n){ selection_name = n;}
  void EnableAll() { for(int i=0; i<cuts.size(); i++) { cuts[i]->Enable();  } }
  void DisableAll(){ for(int i=0; i<cuts.size(); i++) { cuts[i]->Disable(); } }
  void DisableByName(std::string target){ for(int i=0; i<cuts.size(); i++) { if(cuts[i]->GetName() == target) { cuts[i]->Disable(); break; }}}
  void EnableByName(std::string target) { for(int i=0; i<cuts.size(); i++) { if(cuts[i]->GetName() == target) { cuts[i]->Enable(); break; }}}
  void DisableByRegex(std::string);
  void EnableByRegex(std::string);
  void PrintSummary();
  void Optimize();

  DataEventCut *GetCut(std::string cutName);
  
  virtual bool IsPassed(h22Event, int); /** I am making this virtual in the event that we need to change something before asking if the events pass (vertex correction ect.) */
  virtual bool IsPassedFast(h22Event, int); /** I am making this virtual in the event that we need to change something before asking if the events pass (vertex correction ect.) */
  std::map<std::string, double> cut_pass_fraction(); 
  
};


#endif
