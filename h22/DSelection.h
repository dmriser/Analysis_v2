//////////////////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  August 14, 2016 

  DSelection.h -> Defines class structure for the 
  selection utility which combines DCut objects to 
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
#include "DCut.h"
#include "h22Event.h"

class DSelection
{
 public:
  DSelection();
  ~DSelection();

  std::string selection_name; 
  std::vector<DCut*> cuts;

 public:
  void add_cut(DCut *this_cut){ cuts.push_back(this_cut); }
  void set_name(std::string n){ selection_name = n;}
  void enable_all() { for(int i=0; i<cuts.size(); i++) { cuts[i]->enable();  } }
  void disable_all(){ for(int i=0; i<cuts.size(); i++) { cuts[i]->disable(); } }
  void disable_by_name(std::string target){ for(int i=0; i<cuts.size(); i++) { if(cuts[i]->name() == target) { cuts[i]->disable(); break; }}}
  void enable_by_name(std::string target) { for(int i=0; i<cuts.size(); i++) { if(cuts[i]->name() == target) { cuts[i]->enable(); break; }}}
  void disable_by_regex(std::string);
  void enable_by_regex(std::string);
  void summarize();
  
  virtual bool passes(h22Event, int); /** I am making this virtual in the event that we need to change something before asking if the events pass (vertex correction ect.) */ 
  std::map<std::string, double> cut_pass_fraction(); 
  
};


#endif
