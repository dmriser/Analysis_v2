//////////////////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  September 21, 2016

*/
//////////////////////////////////////////////////////

#ifndef physics_event_selector_h
#define physics_event_selector_h

// c++ includes
#include <iostream>
#include <map>
#include <vector>

// my includes
#include "PhysicsEvent.h"
#include "PhysicsEventCut.h"

class PhysicsEventSelector
{
 public:
  PhysicsEventSelector();
  ~PhysicsEventSelector();

  std::string selection_name; 
  std::vector<PhysicsEventCut*> cuts;

 public:
  void add_cut(PhysicsEventCut *this_cut){ cuts.push_back(this_cut); }
  void set_name(std::string n){ selection_name = n;}
  void enable_all() { for(int i=0; i<cuts.size(); i++) { cuts[i]->enable();  } }
  void disable_all(){ for(int i=0; i<cuts.size(); i++) { cuts[i]->disable(); } }
  void disable_by_name(std::string target){ for(int i=0; i<cuts.size(); i++) { if(cuts[i]->name() == target) { cuts[i]->disable(); break; }}}
  void enable_by_name(std::string target) { for(int i=0; i<cuts.size(); i++) { if(cuts[i]->name() == target) { cuts[i]->enable(); break; }}}
  void disable_by_regex(std::string);
  void enable_by_regex(std::string);
  void summarize();
  
  virtual bool passes(PhysicsEvent); /** I am making this virtual in the event that we need to change something before asking if the events pass (vertex correction ect.) */ 
  std::map<std::string, double> cut_pass_fraction(); 
  
};


#endif
