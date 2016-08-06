////////////////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  August 5, 2016 

  DBins.h -> Shamelessly using my name for classes now. 

*/
////////////////////////////////////////////////////

#ifndef dbins_cxx
#define dbins_cxx

// c++ includes
#include <iostream>
#include <vector>
using namespace std;

// my includes
#include "DBins.h"

DBins::~DBins()
{
  // Total Destruction
}

DBins::DBins(int bn, double bmin, double bmax)
{
  kNumber = bn;
  kMin = bmin;
  kMax = bmax;
  kWidth = (kMax-kMin)/(kNumber-1);

  for (int b=0; b<kNumber; b++) kBins.push_back( kMin + b*kWidth );
}

void DBins::refresh()
{
  kWidth = (kMax-kMin)/(kNumber-1);
  for (int b=0; b<kNumber; b++) kBins.push_back( kMin + b*kWidth );
}

void DBins::set_number(int n)
{
  kNumber = n;
}

void DBins::set_min(double m)
{
  kMin = m;
}

void DBins::set_max(double m)
{
  kMax = m;
}

#endif
