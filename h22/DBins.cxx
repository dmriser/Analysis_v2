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

////////////////////////////////////////////////////////////////////////
/*

  DBinsAsymmetric 

*/
////////////////////////////////////////////////////////////////////////

DBinsAsymmetric::DBinsAsymmetric()
{
  // Starting up 
}

DBinsAsymmetric::~DBinsAsymmetric()
{
  // Destroy 
}

int DBinsAsymmetric::find_bin(double value)
{
  
  for (int ibin=0; ibin<bins.size(); ibin++)
    {
      if (value > bins[ibin] && value < bins[ibin+1]) return ibin;  
    }

  return -123;
}

double DBinsAsymmetric::width(int b)
{
  if (b > bins.size()) {
    cout << " Warning: Trying to access bins that exceed length of DBinsAsymmetric::number()! " << endl;
    return -123;
  }

  return (bins[b+1]-bins[b]);
}

double DBinsAsymmetric::center(int b)
{
  if (b > bins.size()) {
    cout << " Warning: Trying to access bins that exceed length of DBinsAsymmetric::number()! " << endl;
    return -123;
  }

  return bins[b]+(bins[b+1]-bins[b])/2;
}

double DBinsAsymmetric::min(int b)
{
  if (b > bins.size()) {
    cout << " Warning: Trying to access bins that exceed length of DBinsAsymmetric::number()! " << endl;
    return -123;
  }

  return (bins[b]);
}

double DBinsAsymmetric::max(int b)
{
  if (b > bins.size()) {
    cout << " Warning: Trying to access bins that exceed length of DBinsAsymmetric::number()! " << endl;
    return -123;
  }

  return (bins[b+1]);
}

////////////////////////////////////////////////////////////////////////////////////////////////
/*

  DBin

*/
////////////////////////////////////////////////////////////////////////////////////////////////

DBin::DBin()
{
  Clear(); 
}

DBin::~DBin()
{

}

void DBin::Clear()
{
  kWidth = 0;
  kFills = 0;  
  kMin = 0;
  kMax = 0;
  kSysError = 0;
  kStatError = 0;
  kScale = 1.00; 
}

void DBin::Refresh()
{
  kWidth = kMax-kMin;
}

void DBin::CalcStatError()
{
  kStatError = sqrt(kFills);
}

void DBin::Print()
{
  cout.width(8); cout << kFills; 
}
////////////////////////////////////////////////////////////////////////////////
/*

  DLineBins 

*/
////////////////////////////////////////////////////////////////////////////////

DLineBins::DLineBins()
{
  kNumber = 0; 
}

DLineBins::~DLineBins()
{

}

int DLineBins::FindBin(double val)
{
  for (int i=0; i<bins.size(); i++)
    {
      if ( val > bins[i].GetMin() && val < bins[i].GetMax() ) return i; 
    }

  return -123;   
}

void DLineBins::Fill(double val)
{
  
    for (int i=0; i<bins.size(); ++i) {
      if ( val > bins[i].GetMin() && val < bins[i].GetMax() ) { bins[i].Fill(); return; }
    }

    if (val < bins[0].GetMin())       { kUnderflow++; return; }
    if (val > bins[kNumber].GetMax()) { kOverflow++;  return; }

}  

void DLineBins::Print()
{
  vector<DBin>::iterator it; 
  for (it = bins.begin(); it != bins.end(); ++it)
    {
      (*it).Print(); 
    }
  cout << endl; 
  
}

////////////////////////////////////////////////////////////////////////////////////
/*

  DPlaneBins 

*/
////////////////////////////////////////////////////////////////////////////////////

DPlaneBins::DPlaneBins()
{

}

DPlaneBins::~DPlaneBins()
{

}

void DPlaneBins::AddLineBins(DLineBins inBins)
{
  if (bins.size() == edge_bins.GetNumber()){ cout << " Cannot add other LineBins, edge bins are at limit. " << endl; return; } 
  bins.push_back(inBins); 
}

void DPlaneBins::Fill(double x, double y)
{
  int edge_index = edge_bins.FindBin(x); 

  if (edge_index > -1 && edge_index < edge_bins.GetNumber())
    {
      bins[edge_index].Fill(y);
    }
}

void DPlaneBins::Print()
{

  for (int i=0; i<edge_bins.GetNumber(); i++)
    {
      bins[i].Print(); 
    }
  
}

#endif
