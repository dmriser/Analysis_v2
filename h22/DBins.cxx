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
  cout << " DBins are now outdated, please switch to DLineBins. " << endl; 
}

DBins::~DBins()
{
  // Total Destruction
}

DBins::DBins(int bn, double bmin, double bmax)
{
  cout << " DBins are now outdated, please switch to DLineBins. " << endl;
    
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

DBin DBin::operator+(DBin& bin)
{
  // I am not building in protection for adding bins that are non-sequential. 
  DBin result;

  // Do sizing, min, max, width 
  result.SetMin(this->GetMin());
  result.SetMax(bin.GetMax());
  result.Refresh(); 

  // Do Fills and Set Errors  
  result.SetFills(this->GetFills() + bin.GetFills());
  result.CalcStatError(); 

  // How do we handle the scaling correctly, this is a weighted average for temp. usage. 
  double scale = (this->GetContent()+bin.GetContent())/(this->GetFills()+bin.GetFills());
  result.SetScale( scale ); 

  return result; 
}

DBin DBin::operator+=(DBin& bin)
{

  // Do sizing, min, max, width 
  this->SetMin(this->GetMin());
  this->SetMax(bin.GetMax());
  this->Refresh(); 

  // Do Fills and Set Errors  
  this->SetFills(this->GetFills() + bin.GetFills());
  this->CalcStatError(); 

  // How do we handle the scaling correctly, this is a weighted average for temp. usage. 
  double scale = (this->GetContent()+bin.GetContent())/(this->GetFills()+bin.GetFills());
  this->SetScale( scale ); 

  return *this; 
}

DBin DBin::operator/(DBin& bin)
{
  // This is on the todo list. Going to set errors = (a/b)*sqrt((a+b)/(a*b))
  // Not sure if this is ok. 
  DBin result;
  return result; 
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

DLineBins::DLineBins(int numBins, double binMin, double binMax)
{
  // Generating symmetric bins based on incoming conditions, this method
  // replaces the old class DBins.
  if (numBins < 0 || binMax < binMin) { cout << " Bad Parameters Set to DLineBins::DLineBins(int numBins, double binMin, double binMax)... Exiting Const. "; return; }
  
  double binWidth = double( (binMax-binMin)/numBins ); 

  for (int ibin=0; ibin<numBins; ibin++) {
    // Setting the min, max, and width
    DBin aBin;
    aBin.SetMin(binMin + ibin*binWidth);
    aBin.SetMax(binMin + (ibin+1)*binWidth);
    aBin.Refresh();

    // Adding the bin to our Line 
    AddBin(aBin);
  }
  
}

DLineBins::DLineBins(int numBins, vector<double> limits)
{
  // Generates bins based on your input limits.
  if (numBins+1 != limits.size()){ cout << " Aborting DLineBins for bin number / limit mismatch! " << endl; return; }

  // Add the bins. 
  for (int ibin=0; ibin<numBins; ibin++){
    DBin this_bin;
    this_bin.SetMin(limits[ibin]);
    this_bin.SetMax(limits[ibin+1]);
    AddBin(this_bin);
  }
  
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

DLineBins DLineBins::Rebin(int numBins, vector<double> limits)
{
  DLineBins rebins;

  cout << " Starting DLineBins::Rebin()... Number = " << numBins << " Old number = " << bins.size() << endl; 
  
  if (numBins+1 != limits.size()) { cout << " Error: DLineBins::Rebin() detects mismatching number of bins and limit size (numBins+1)... " << endl; return rebins; }
  else if (limits.front() < bins.front().GetMin()) { cout << " Error: DLineBins::Rebin() detects lower limit smaller than original... " << endl; return rebins; }
  else if (limits.back()  > bins.back() .GetMax()) { cout << " Error: DLineBins::Rebin() detects upper limit bigger than original... " << endl; return rebins; }

  // Bin counting variable.  
  int rebin_index = 0; 

  // Setup first bin to add to new DLineBins 
  DBin this_bin;
  this_bin.SetMin( limits[0] );
  this_bin.SetMax( limits[1]  );
  this_bin.Refresh();

  // Find the first old bin that we start with
  int first_bin = 0; 
  if (limits[0] != bins.front().GetMin()) { 
    for (int b=0; b<bins.size(); b++){

      if (limits[0] == bins[b].GetMin()){
	first_bin = b;
	break; 
      } 
      
      if (bins[b].GetMin() > limits[0]){
	first_bin = b;
	break; 
      } 

    } 
  }
  
  cout << " Starting at old bin " << first_bin << endl; 
  
  // Loop over the remaining bins and stop when we pass the end of the new bin limits. 
  for (int b=first_bin; b<bins.size(); b++){

    // Stop we reached the end of the line. 
    if (bins[b].GetMax() > limits.back()){ rebins.AddBin(this_bin); break; } 

    // We finished filling the new bin, move to next.
    if (bins[b].GetMin() > limits[rebin_index+1]){
      cout << " Finishing new bin " << rebin_index << endl; 
      rebins.AddBin(this_bin);
      rebin_index++;
       
      this_bin.Clear();
      this_bin.SetMin( limits[rebin_index] );
      this_bin.SetMax( limits[rebin_index+1] );
      this_bin.Refresh(); 
    }

    // Old bin is within the current new bin, add old bin contents to it. 
    this_bin += bins[b]; 
    cout << " Summing on old bin " << b << " into new bin " << rebin_index << " with old limits " << bins[b].GetMin() << " to " << bins[b].GetMax() << " new " << limits[rebin_index] << " to " << limits[rebin_index+1] << endl; 
    
    // We are on the last bin, push it back 
    if (b+1 == bins.size()){ rebins.AddBin(this_bin); } 
  } 
  
  return rebins; 
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
