//////////////////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  August 14, 2016 

  DataEventSelector.cxx 

*/
//////////////////////////////////////////////////////

#ifndef dselection_cxx
#define dselection_cxx

// c++ includes
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
using namespace std; 

// my includes
#include "DataEventCut.h"
#include "h22Event.h"
#include "DataEventSelector.h"

DataEventSelector::DataEventSelector(){
  // Setup 
}

DataEventSelector::~DataEventSelector(){
  // Destroy
  for (vector<DataEventCut*>::iterator it=cuts.begin(); it!=cuts.end(); it++ ) delete *it; 
}

bool DataEventSelector::passes(h22Event event, int index){
  // Loop over the cuts we have and make sure all pass.
  bool status = true;
  
  vector<DataEventCut*>::iterator it;
  for (it = cuts.begin(); it!=cuts.end(); it++){
    if ( (*it)->is_on() && (*it)->applies(event, index) && !(*it)->passes(event, index)) { status = false; }
  }

  return status;
}

DataEventCut * DataEventSelector::getCut(string cutName){
  vector<DataEventCut*>::iterator it;
  for (it = cuts.begin(); it!=cuts.end(); it++){
    if ((*it)->name() == cutName) { return (*it); }
  }

  cout << "[DataEventSelector::getCut] Couldn't find " << cutName << endl;
  return new DataEventCut();
}


void DataEventSelector::optimize(){
  std::sort(cuts.begin(), cuts.end());
}

bool DataEventSelector::passesFast(h22Event event, int index){
  // Loop over the cuts we have and make sure all pass.
  bool status = true;
  
  vector<DataEventCut*>::iterator it;
  for (it = cuts.begin(); it!=cuts.end(); it++){
    if ( (*it)->is_on() && (*it)->applies(event, index) && !(*it)->passes(event, index)) { status = false; return status; }
  }

  return status;
}

map<string, double> DataEventSelector::cut_pass_fraction(){

  map<string, double> results;
  for (int icut=0; icut<cuts.size(); icut++)
    {
      results[cuts[icut]->name()] = (double) cuts[icut]->number_pass()/(cuts[icut]->number_fail()+cuts[icut]->number_pass());
    }
  
  return results; 
}

void DataEventSelector::enable_by_regex(string regex)
{
  
  for (int icut=0; icut<cuts.size(); icut++)
    {
      if (cuts[icut]->name().find(regex) != std::string::npos) { cuts[icut]->enable(); }  
    }
}

void DataEventSelector::disable_by_regex(string regex)
{
  
  for (int icut=0; icut<cuts.size(); icut++)
    {
      if (cuts[icut]->name().find(regex) != std::string::npos) { cuts[icut]->disable(); }  
    }
}

void DataEventSelector::summarize()
{

  cout << "<--------------------- DSelector Summary ------------------------>" << endl; 
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
  cout << "<---------------------------------------------------------------->" << endl; 
}

#endif
