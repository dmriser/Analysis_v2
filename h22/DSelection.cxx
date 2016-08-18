//////////////////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  August 14, 2016 

  DSelection.cxx 

*/
//////////////////////////////////////////////////////

#ifndef dselection_cxx
#define dselection_cxx

// c++ includes
#include <iostream>
#include <map>
#include <vector>
using namespace std; 

// my includes
#include "DCut.h"
#include "DEvent.h"
#include "DSelection.h"


DSelection::DSelection()
{
  // Setup 
}

DSelection::~DSelection()
{
  // Destroy
  for (vector<DCut*>::iterator it=cuts.begin(); it!=cuts.end(); it++ ) delete *it; 
}

bool DSelection::passes(DEvent event, int index)
{
  // Loop over the cuts we have and make sure all pass.
  bool status = true;
  
  vector<DCut*>::iterator it;
  for (it = cuts.begin(); it!=cuts.end(); it++)
    {
      if ( (*it)->is_on() && (*it)->applies(event, index) && !(*it)->passes(event, index)) { status = false; }
    }

  return status;
}

map<string, double> DSelection::cut_pass_fraction()
{

  map<string, double> results;
  for (int icut=0; icut<cuts.size(); icut++)
    {
      results[cuts[icut]->name()] = (double) cuts[icut]->number_pass()/(cuts[icut]->number_fail()+cuts[icut]->number_pass());
    }
  
  return results; 
}
  
void DSelection::summarize()
{

  cout << "<--------------------- DSelector Summary ------------------------->" << endl; 
  cout.width(20); cout << "Cut Name";
  cout.width(8);  cout << "Pass"; 
  cout.width(8);  cout << "Total" << endl;
  

  for (int icut=0; icut<cuts.size(); icut++)
    {
      cout.width(20); cout << cuts[icut]->name();
      cout.width(8);  cout << cuts[icut]->number_pass();
      cout.width(8);  cout << cuts[icut]->number_fail() + cuts[icut]->number_pass() << endl;
    }
  cout << "<---------------------------------------------------------------->" << endl; 
}

#endif
