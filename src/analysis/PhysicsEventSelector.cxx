//////////////////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  August 14, 2016 

  DSelection.cxx 

*/
//////////////////////////////////////////////////////

#ifndef physics_event_selector_cxx
#define physics_event_selector_cxx

// c++ includes
#include <iostream>
#include <map>
#include <vector>
using namespace std; 

// my includes
#include "PhysicsEventCut.h"
#include "PhysicsEvent.h"
#include "PhysicsEventSelector.h"

PhysicsEventSelector::PhysicsEventSelector(){
  // Setup 
}

PhysicsEventSelector::~PhysicsEventSelector(){
  for (vector<PhysicsEventCut*>::iterator it=cuts.begin(); it!=cuts.end(); it++ ) delete *it; 
}

bool PhysicsEventSelector::passes(PhysicsEvent &event){
  // Loop over the cuts we have and make sure all pass.
  bool status = true;
  
  vector<PhysicsEventCut*>::iterator it;
  for (it = cuts.begin(); it!=cuts.end(); it++)
    {
      if ( (*it)->is_on() && (*it)->applies(event) && !(*it)->passes(event)) { status = false; }
    }

  return status;
}

map<string, double> PhysicsEventSelector::cut_pass_fraction()
{

  map<string, double> results;
  for (int icut=0; icut<cuts.size(); icut++)
    {
      results[cuts[icut]->name()] = (double) cuts[icut]->number_pass()/(cuts[icut]->number_fail()+cuts[icut]->number_pass());
    }
  
  return results; 
}

void PhysicsEventSelector::enable_by_regex(string regex)
{
  
  for (int icut=0; icut<cuts.size(); icut++)
    {
      if (cuts[icut]->name().find(regex) != std::string::npos) { cuts[icut]->enable(); }  
    }
}

void PhysicsEventSelector::disable_by_regex(string regex)
{
  
  for (int icut=0; icut<cuts.size(); icut++)
    {
      if (cuts[icut]->name().find(regex) != std::string::npos) { cuts[icut]->disable(); }  
    }
}

void PhysicsEventSelector::summarize()
{

  cout << "<--------------------- PhysicsEventSelector Summary ------------------------>" << endl; 
  cout.width(20); cout << "Cut Name";
  cout.width(12);  cout << "Pass";
  cout.width(12);  cout << "Frac"; 
  cout.width(12);  cout << "Total" << endl;
  

  for (int icut=0; icut<cuts.size(); icut++)
    {
      if ( cuts[icut]->is_on() ) {
	cout.width(20); cout << cuts[icut]->name();
	cout.width(12);  cout << cuts[icut]->number_pass();
	cout.width(12);  cout << (float) cuts[icut]->number_pass()/cuts[icut]->number_total();
	cout.width(12);  cout << cuts[icut]->number_total() << endl;
      }

      else
	{
	  cout.width(20); cout << cuts[icut]->name();
	  cout.width(12); cout << " Disabled " << endl;
	}

    }
  cout << "<--------------------------------------------------------------------------->" << endl; 
}

#endif
