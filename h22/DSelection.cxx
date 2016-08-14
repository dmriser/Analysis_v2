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
#include "DSelection.h"


DSelection::DSelection()
{
  // Setup 
}

DSelection::~DSelection()
{
  // Destroy
}

bool DSelection::passes(h22Event event, int index)
{
  // Loop over the cuts we have and make sure all pass.
  bool status = true;
  for (int icut=0; icut<cuts.size(); icut++) { if ( cuts[icut].is_on() && !cuts[icut].passes(event, index) ) { status = false; } }
  return status;
}

map<string, double> DSelection::cut_pass_fraction()
{

  map<string, double> results;
  for (int icut=0; icut<cuts.size(); icut++)
    {
      results[cuts[icut].name()] = (double) cuts[icut].number_pass()/cuts[icut].number_fail();
    }
  
  return results; 
}
  


#endif
