#include <iostream>
#include <vector>
using namespace std;

#include "DBins.h"
#include "TRandom3.h"

int main()
{

  cout << " Bin Example! " << endl; 

  TRandom3 * rand = new TRandom3(); 

  // Testing out new constructor 
  DLineBins xBins(20, 0, 0.6); 

  // Do a little test fill 
  for (int i=0; i<10000; i++){
    xBins.Fill( rand->Uniform(1) );
  }

  // Old Bins 
  xBins.Print(); 
  
  // Do an asymmetric rebinning of the xBins into 2 big bins. 
  vector<double> xLimits;
  xLimits.push_back(0.1);
  xLimits.push_back(0.2);
  xLimits.push_back(0.4);
  xLimits.push_back(0.6); 
  int n_x_rebins = xLimits.size()-1; 

  // Get the new bins
  DLineBins newXBins = xBins.Rebin(n_x_rebins, xLimits); 
  newXBins.Print(); 

  // Showing how to initialize empty bins with the structure of the newXBins
  DLineBins emptyNewXBins(n_x_rebins, xLimits);
  emptyNewXBins.Print(); 

  cout.width(12); cout << " bin "; 
  cout.width(12); cout << " width "; 
  cout.width(12); cout << " min ";
  cout.width(12); cout << " max "<< endl;
  
  for (int ibin=0; ibin<emptyNewXBins.GetNumber(); ibin++) {
    cout.width(12); cout << ibin; 
    cout.width(12); cout << emptyNewXBins.GetBin(ibin).GetWidth();
    cout.width(12); cout << emptyNewXBins.GetBin(ibin).GetMin();
    cout.width(12); cout << emptyNewXBins.GetBin(ibin).GetMax() << endl;
  }
  
    // Try to break the machine by doing another rebin
  vector<double> newXLimits;
  newXLimits.push_back(0.1);
  newXLimits.push_back(0.55);
  newXLimits.push_back(0.6); 
  int n_new_x_rebins = newXLimits.size()-1; 

  DLineBins newerXBins = newXBins.Rebin(n_new_x_rebins, newXLimits); 
  newerXBins.Print();
  
  return 0;
}
