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

class DSelection
{
 public:
  DSelection();
  ~DSelection();

 private:
  std::string selection_name; 
  std::vector<DCut*> cuts;

 public:
  void add_cut(DCut *this_cut){ cuts.push_back(this_cut); }
  void set_name(std::string n){ selection_name = n;}
  void enable_all() { for(int i=0; i<cuts.size(); i++) { cuts[i]->enable();  } }
  void disable_all(){ for(int i=0; i<cuts.size(); i++) { cuts[i]->disable(); } }
  void summarize();
  
  bool passes(h22Event, int); //! Passed down to the DCuts 
  std::map<std::string, double> cut_pass_fraction(); 
  
};


#endif
