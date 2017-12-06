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

#include "TH2.h"

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
  return (kWidth/2 + ibin*kWidth + kMin);
  
}

double DBins::bin_min(int ibin){
  if ( ibin > kNumber || ibin < 0 ) { cout << "Error trying to get bin center of bin outside range! " << endl; return 0.0; }

  // Bins are all the same size 
  return (ibin*kWidth + kMin);  
}

double DBins::bin_max(int ibin){
  if ( ibin > kNumber || ibin < 0 ) { cout << "Error trying to get bin center of bin outside range! " << endl; return 0.0; }

  // Bins are all the same size 
  return ((ibin+1)*kWidth + kMin);  
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
  stat_rebin_threshold = 0.1; 
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

DLineBins::DLineBins(int numBins, const std::vector<double>& limits)
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

DLineBins::DLineBins(const std::vector<double>& limits){
  DLineBins(limits.size()-1, limits);
}

int DLineBins::FindBin(double val){

  for (int i=0; i<bins.size(); i++){
      if ( val >= bins[i].GetMin() && val <= bins[i].GetMax() ) return i; 
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
  
  //  cout << " Starting at old bin " << first_bin << endl; 
  
  // Loop over the remaining bins and stop when we pass the end of the new bin limits. 
  for (int b=first_bin; b<bins.size(); b++){

    // Stop we reached the end of the line. 
    if (bins[b].GetMax() > limits.back()){ rebins.AddBin(this_bin); break; } 

    // We finished filling the new bin, move to next.
    if (bins[b].GetMin() > limits[rebin_index+1]){
      //  cout << " Finishing new bin " << rebin_index << endl; 
      rebins.AddBin(this_bin);
      rebin_index++;
       
      this_bin.Clear();
      this_bin.SetMin( limits[rebin_index] );
      this_bin.SetMax( limits[rebin_index+1] );
      this_bin.Refresh(); 
    }

    // Old bin is within the current new bin, add old bin contents to it. 
    this_bin += bins[b]; 
    //    cout << " Summing on old bin " << b << " into new bin " << rebin_index << " with old limits " << bins[b].GetMin() << " to " << bins[b].GetMax() << " new " << limits[rebin_index] << " to " << limits[rebin_index+1] << endl; 
    
    // We are on the last bin, push it back 
    if (b+1 == bins.size()){ rebins.AddBin(this_bin); } 
  } 
  
  return rebins; 
}

DLineBins DLineBins::StatisticalRebin(int numBins)
{
  DLineBins results; 

  if (numBins > bins.size()) { cout << " Error in DLineBins::StatisticalRebin()... target number exceeds bins.size() " << endl; return results; }
  if (bins.size()/numBins < 5) { cout << " Warning: DLineBins::StatisticalRebin()... Outcome unlikely good, increase ratio of old bins to new bins. " << endl; } 
  
  // Totaling bin content in all bins. 
  long int total = 0;
  for (int ibin=0; ibin<bins.size(); ibin++){ total += bins[ibin].GetFills(); } 

  // Setting target number of entries in each bin.
  int target        = (int) total/numBins; 
  int running_total = 0; 
  int rebin_index   = 0; 
  
  // Start looping on bins and creating new.
  DBin this_bin;
  this_bin.SetMin(bins.front().GetMin());

  //  cout << " Starting Statistical Rebin, target is " << target << endl; 
  
  for (int ibin=0; ibin<bins.size(); ibin++) {
    // We passed the optimal point. 
    if (running_total > target) {
      //      cout << " Writing into rebin " << rebin_index << " for passing it with " << running_total << " entries on bin " << ibin << endl; 
      this_bin.SetMax( bins[ibin].GetMax() );
      this_bin.Refresh();
      this_bin.SetFills( running_total );
      results.AddBin(this_bin);
      running_total = 0;
      rebin_index++;
      this_bin.Clear();
      this_bin.SetMin( bins[ibin].GetMax() );
    }
    
    // We are within threshold, add the new bin. 
    if  ( (double) ((double)(target-running_total)/(double)(target)) < stat_rebin_threshold ) {
      //      cout << " Writing into rebin " << rebin_index << " for coming in threshold with " << running_total << " entries on bin " << ibin << endl; 
      this_bin.SetMax( bins[ibin].GetMax() );
      this_bin.Refresh();
      this_bin.SetFills( running_total );
      results.AddBin(this_bin);
      running_total = 0;
      rebin_index++;
      this_bin.Clear();
      this_bin.SetMin( bins[ibin].GetMax() );
    }
    
    
    // If we are on our last bin, just add the remainder.
    if (rebin_index == numBins-1) {
      for (int jbin=ibin; jbin<bins.size(); jbin++){
	running_total += bins[jbin].GetFills();
      }
      this_bin.SetMax( bins.back().GetMax() );
      this_bin.Refresh();
      this_bin.SetFills( running_total ); 
      results.AddBin(this_bin);
      break; 
    }
    
    // Add the bin contents to the running total 
    running_total += bins[ibin].GetFills();
  }
  
  return results; 
}

vector<double> DLineBins::GetLimits(){
  vector<double> limits;

  for (int ibin=0; ibin<GetNumber(); ibin++) {
    limits.push_back( GetBin(ibin).GetMin() );
  }

  limits.push_back( bins.back().GetMax() );
  return limits;
}

DLineBins DLineBins::operator+(DLineBins& lineBins){
  DLineBins results;

  if (this->GetNumber() != lineBins.GetNumber()) { cout << " Error: DLineBins operator+()... Cannot add DLineBins with different lenghts! " << endl; return results; } 

  // Go adding bin by bin. This is naive, we should actually check the bin
  // boundaries and then search for the correct mapping between our 2 bins. 
  // This method is fine as long as every line bin is identical before rebinning.
  // The "every line bin same" is exactly what we have for a TH2 histogram. 
  // I may revisit this later, if my salary goes up. 
  for (int ibin=0; ibin<this->GetNumber(); ibin++){
    DBin bin_a; bin_a = lineBins.GetBin(ibin);
    DBin bin_b; bin_b = this->GetBin(ibin);
    DBin this_bin = bin_a + bin_b;
    results.AddBin( this_bin ); 
  } 
  
  return results; 
}


DLineBins DLineBins::operator+=(DLineBins& lineBins){

  DLineBins results; 
  if (this->GetNumber() != lineBins.GetNumber()) { cout << " Error: DLineBins operator+()... Cannot add DLineBins with different lenghts! " << endl; return results; } 

  // Go adding bin by bin. This is naive, we should actually check the bin
  // boundaries and then search for the correct mapping between our 2 bins. 
  // This method is fine as long as every line bin is identical before rebinning.
  // The "every line bin same" is exactly what we have for a TH2 histogram. 
  // I may revisit this later, if my salary goes up. 
  for (int ibin=0; ibin<this->GetNumber(); ibin++){
    DBin bin_a = this->GetBin(ibin);
    DBin bin_b = lineBins.GetBin(ibin); 
    DBin this_bin = bin_a + bin_b; 
    this->AddBin( this_bin ); 
  } 
  
  return *this; 
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

DPlaneBins::DPlaneBins(TH2D * sourceHistogram){

  int numberXBins = sourceHistogram->GetXaxis()->GetNbins();
  double xMin = sourceHistogram->GetXaxis()->GetBinLowEdge(1);
  double xMax = sourceHistogram->GetXaxis()->GetBinUpEdge(numberXBins);

  int numberYBins = sourceHistogram->GetYaxis()->GetNbins();
  double yMin = sourceHistogram->GetYaxis()->GetBinLowEdge(1);
  double yMax = sourceHistogram->GetYaxis()->GetBinUpEdge(numberYBins);

  SetEdgeBins(DLineBins(numberYBins, yMin, yMax)); 

  DLineBins xBins(numberXBins, xMin, xMax);
  
  for (int yBin = 0; yBin < numberYBins; yBin++) {
    DLineBins theseBins; 

    for (int xBin = 0; xBin < numberXBins; xBin++) {
      int fills = (int) sourceHistogram->GetBinContent(xBin+1, yBin+1); 
      DBin thisBin;
      thisBin.SetMin(xBins.GetBin(xBin).GetMin());
      thisBin.SetMax(xBins.GetBin(xBin).GetMax());
      thisBin.SetFills( fills );
      theseBins.AddBin(thisBin);
    }
    AddLineBins( theseBins );
  }

  // Still need to set edge bin content in order for rebins to work. 
  for (int ibin=0; ibin<edge_bins.GetNumber(); ibin++){
    int fills = 0;
    for (int jbin=0; jbin<bins[ibin].GetNumber(); jbin++){
      fills += bins[ibin].GetBin(jbin).GetFills(); 
    }
    edge_bins.GetBin(ibin).SetFills( fills );
  }

}

void DPlaneBins::AddLineBins(DLineBins inBins)
{
  if (bins.size() == edge_bins.GetNumber()){ cout << " Cannot add other LineBins, edge bins are at limit. " << endl; return; } 
  bins.push_back(inBins); 
}

void DPlaneBins::Fill(double x, double y)
{
  int edge_index = edge_bins.FindBin(x); 
  if (edge_index > -1) { edge_bins.Fill(x); }
  
  if (edge_index > -1 && edge_index < edge_bins.GetNumber())
    {
      bins[edge_index].Fill(y);
    }
}

void DPlaneBins::Print()
{

  for (int i=0; i<bins.size(); i++)
    {
      bins[i].Print(); 
    }
  
}

DPlaneBins DPlaneBins::StatisticalRebin(int numEdgeBins, int numBinsPerLine){
  DPlaneBins result;

  if (numEdgeBins > edge_bins.GetNumber() ) { cout << " Error: DPlaneBins::StatisticalRebin() Number of final edge bins less than original number. " << endl; return result; }

  for (int b=0; b<bins.size(); b++) 
    if (numBinsPerLine > bins[b].GetNumber() ) { cout << " Error: DPlaneBins::StatisticalRebin() Number of final line bins less than original number. " << endl; return result; }

  // First do the edge bins
  DLineBins edge_result = edge_bins.StatisticalRebin( numEdgeBins ); 
  result.SetEdgeBins( edge_result );

  // Now we need to start adding line bins together and rebinning them.
  // edge_result now has new kMin and kMax parameters for each bin, we
  // can use them to combine our lines. 
  int edge_bin_index  = 0; 
  DLineBins this_line = bins[0]; 
  this_line.Clear();
  
  for (int ibin=0; ibin<edge_bins.GetNumber(); ibin++) {

    // Stop adding bins together. 
    cout << " bin " << ibin << " edge bin " << edge_bin_index << endl;
    if (edge_bins.GetBin(ibin).GetMax() > edge_result.GetBin(edge_bin_index).GetMax()) {
      edge_bin_index++; 
      DLineBins this_new_line = this_line.StatisticalRebin(numBinsPerLine);
      result.AddLineBins( this_new_line );
      this_line.Clear();
    }

    // We're at the last, just add remaning lines. 
    if (edge_bin_index == numEdgeBins-1) {
      for (int jbin=ibin; jbin<edge_bins.GetNumber(); jbin++) {
	this_line = this_line + bins[jbin];
      }
      result.AddLineBins(this_line.StatisticalRebin(numBinsPerLine)); 
      break; 
    }

    //  Add lines to the current total
    this_line = this_line+bins[ibin];
    
  }
  
  return result; 
}

TH2D * DPlaneBins::toTH2D(){

  DLineBins xBins = bins[0];
  DLineBins yBins = edge_bins;
  
  TH2D * thisHisto = new TH2D("thisHisto","",xBins.GetNumber(),xBins.GetMin(),xBins.GetMax(),yBins.GetNumber(),yBins.GetMin(),yBins.GetMax()); 

  for (int yBin = 0; yBin < yBins.GetNumber(); yBin++){
    for (int xBin = 0; xBin < xBins.GetNumber(); xBin++){
      double binContent = bins[yBin].GetBin(xBin).GetFills(); 
      thisHisto->SetBinContent(xBin+1, yBin+1, binContent); 
    }
  }

  return thisHisto; 
}

#endif
