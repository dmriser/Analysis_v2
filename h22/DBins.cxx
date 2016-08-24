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
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

// my includes
#include "DBins.h"

DBins::DBins()
{
  // Creation Story here.
}

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

int DBins::find_bin(double value)
{

  int bin = floor((value-kMin)/kWidth);

  if (bin > -1 && bin < kNumber) return bin;
  else return -123;
}

void DBins::operator=(const DBins * newBins)
{
  kNumber = newBins->kNumber;
  kMin = newBins->kMin;
  kMax = newBins->kMax;
  refresh();
}

void DBins::operator=(DBins newBins)
{
  kNumber = newBins.kNumber;
  kMin = newBins.kMin;
  kMax = newBins.kMax;
  refresh();
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

double DBins::bin_center(int ibin)
{
  if ( ibin > kNumber || ibin < 0 ) { cout << "Error trying to get bin center of bin outside range! " << endl; return 0.0; }

  // Bins are all the same size 
  return kWidth/2 + kBins[ibin];
  
}

#endif
