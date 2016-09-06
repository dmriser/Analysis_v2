// c++ stuff 
#include <iostream>
#include <cmath>
#include <vector>
using std::cout;
using std::endl;
using std::vector;

// model
#include "F1F209Wrapper.hh"

// my stuff
#include "DBins.h"

int main(){

  F1F209Wrapper model;

  // Constants for XS 
  int A = 27, Z = 13;
  double beam_energy = 6.4; // GeV 
  
  DLineBins thetaBins(10, 5, 35);
  DLineBins energyBins(10,0.5,5.9);

  // Looping on bins 
  for (int thetaBin = 0; thetaBin < thetaBins.GetNumber(); thetaBin++) {
    for (int energyBin = 0; energyBin < energyBins.GetNumber(); energyBin++) {
      cout.width(12); cout << thetaBins .GetBin(thetaBin) .GetCenter();
      cout.width(12); cout << energyBins.GetBin(energyBin).GetCenter();
      cout.width(12); cout << model.GetXSByAngle(Z, A, beam_energy, energyBins.GetBin(energyBin).GetCenter(), thetaBins.GetBin(thetaBin).GetCenter()) << endl;
    }
  }
  
  return 0; 
}
