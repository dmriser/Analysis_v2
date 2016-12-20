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
  void add_cut(DataEventCut *this_cut){ cuts.push_back(this_cut); }
  void set_name(std::string n){ selection_name = n;}
  void enable_all() { for(int i=0; i<cuts.size(); i++) { cuts[i]->enable();  } }
  void disable_all(){ for(int i=0; i<cuts.size(); i++) { cuts[i]->disable(); } }
  void disable_by_name(std::string target){ for(int i=0; i<cuts.size(); i++) { if(cuts[i]->name() == target) { cuts[i]->disable(); break; }}}
  void enable_by_name(std::string target) { for(int i=0; i<cuts.size(); i++) { if(cuts[i]->name() == target) { cuts[i]->enable(); break; }}}
  void disable_by_regex(std::string);
  void enable_by_regex(std::string);
  void summarize();
  void optimize();

  DataEventCut *getCut(std::string cutName); 
  
  virtual bool passes(h22Event, int); /** I am making this virtual in the event that we need to change something before asking if the events pass (vertex correction ect.) */ 
  virtual bool passesFast(h22Event, int); /** I am making this virtual in the event that we need to change something before asking if the events pass (vertex correction ect.) */ 
  std::map<std::string, double> cut_pass_fraction(); 
  
};


#endif
