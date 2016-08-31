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

  // Binning Scheme 
  vector<double> limits;
  limits.push_back( 1.0 );
  limits.push_back( 1.3 );
  limits.push_back( 5.0 );
  
  DLineBins xBins(5,0.1,0.6);
  DLineBins qqBins(limits.size()-1,limits);

  double e_beam = 5.498; 
  
  for (int i=0; i<xBins.GetNumber(); i++) {
    for (int j=0; j<qqBins.GetNumber(); j++) {
      double e_prime = e_beam - qqBins.GetBin(j).GetCenter()/(2*0.938*xBins.GetBin(i).GetCenter());
      double theta   = 2*(180/3.14159265)*asin(qqBins.GetBin(j).GetCenter()/(2*sqrt(e_prime*e_beam)));
      cout.width(12); cout << xBins.GetBin(i).GetCenter();
      cout.width(12); cout << qqBins.GetBin(j).GetCenter();
      cout.width(12); cout << e_prime;
      cout.width(12); cout << theta;
      cout.width(12); cout << model.GetXS(1,1,e_beam,e_prime,theta) << endl;
    }
  }
  
  return 0; 
}
