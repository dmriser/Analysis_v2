// c++ stuff 
#include <iostream>
using std::cout;
using std::endl;

// model
#include "F1F209Wrapper.hh"

// my stuff
#include "DBins.h"

int main(){

  F1F209Wrapper model;

  // Binning Scheme 
  DLineBins energyBins(5, 1, 4);
  DLineBins thetaBins(5, 10, 60); 

  cout.width(12); cout << " E' Energy ";
  cout.width(12); cout << " Theta ";
  cout.width(12); cout << " XS " << endl; 
  
  for (int i=0; i<energyBins.GetNumber(); i++) {
    for (int j=0; j<thetaBins.GetNumber(); j++) {
      cout.width(12); cout << energyBins.GetBin(i).GetCenter();
      cout.width(12); cout << thetaBins.GetBin(j).GetCenter();
      cout.width(12); cout << model.GetXS(1,1,5.498,energyBins.GetBin(i).GetCenter(),thetaBins.GetBin(j).GetCenter()) << endl;
    }
  }
  
  return 0; 
}
