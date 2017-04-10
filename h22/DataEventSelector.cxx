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
#include "CommonTools.h"
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

bool DataEventSelector::IsPassed(h22Event event, int index){
  // Loop over the cuts we have and make sure all pass.
  bool status = true;
  
  vector<DataEventCut*>::iterator it;
  for (it = cuts.begin(); it!=cuts.end(); it++){
    if ( (*it)->IsOn() && (*it)->CanBeApplied(event, index) && !(*it)->IsPassed(event, index)) { status = false; }
  }

  return status;
}

DataEventCut * DataEventSelector::GetCut(string cutName){
  vector<DataEventCut*>::iterator it;
  for (it = cuts.begin(); it!=cuts.end(); it++){
    if ((*it)->GetName() == cutName) { return (*it); }
  }

  cout << "[DataEventSelector::getCut] Couldn't find " << cutName << endl;
  return new DataEventCut();
}


void DataEventSelector::Optimize(){
  std::sort(cuts.begin(), cuts.end());
}

bool DataEventSelector::IsPassedFast(h22Event event, int index){
  // Loop over the cuts we have and make sure all pass.
  bool status = true;
  
  vector<DataEventCut*>::iterator it;
  for (it = cuts.begin(); it!=cuts.end(); it++){
    if ( (*it)->IsOn() && (*it)->CanBeApplied(event, index) && !(*it)->IsPassed(event, index)) { status = false; return status; }
  }

  return status;
}

map<string, double> DataEventSelector::cut_pass_fraction(){

  map<string, double> results;
  for (int icut=0; icut<cuts.size(); icut++)
    {
      results[cuts[icut]->GetName()] = (double) cuts[icut]->NumberPass()/(cuts[icut]->NumberFail()+cuts[icut]->NumberPass());
    }
  
  return results; 
}

void DataEventSelector::EnableByRegex(string regex)
{
  
  for (int icut=0; icut<cuts.size(); icut++)
    {
      if (cuts[icut]->GetName().find(regex) != std::string::npos) { cuts[icut]->Enable(); }
    }
}

void DataEventSelector::DisableByRegex(string regex)
{
  
  for (int icut=0; icut<cuts.size(); icut++)
    {
      if (cuts[icut]->GetName().find(regex) != std::string::npos) { cuts[icut]->Disable(); }
    }
}

void DataEventSelector::PrintSummary()
{

  cout << "<---------------------  Selector Summary ------------------------>" << endl; 
  cout.width(20); cout << "Cut GetName";
  cout.width(12); cout << "Pass";
  cout.width(12); cout << "Frac"; 
  cout.width(12); cout << "Total" << endl;
  
  for (int icut=0; icut<cuts.size(); icut++){
      if ( cuts[icut]->IsOn() ) {
    cout.width(20); cout << cuts[icut]->GetName();
    cout.width(12); cout << cuts[icut]->NumberPass();
    cout.width(12); cout << stringify( (float) cuts[icut]->GetPassFraction());
    cout.width(12); cout << cuts[icut]->NumberTotal() << endl;
      }

      // the cut was not used 
      else{
      cout.width(20); cout << cuts[icut]->GetName();
      cout.width(12); cout << " Disabled " << endl;
	}

    }
  cout << "<---------------------------------------------------------------->" << endl; 
}

#endif
