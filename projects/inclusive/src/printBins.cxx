#include "common/Bins.h"

#include <iostream>
using std::cout; 
using std::endl;

int main(int argc, char *argv[]){

  Bins bins; 

  cout << "Number of bins: " << bins.WBINS << endl; 
  cout << "Number of bins: " << bins.Q2BINS << endl; 
  cout << "Number of bins: " << bins.PHIBINS << endl; 

  for(int bin=0; bin<=bins.WBINS; bin++){
    cout.width(12); 
    cout << bins.wBins[bin] << endl; 
  }
  for(int bin=0; bin<=bins.Q2BINS; bin++){
    cout.width(12); 
    cout << bins.qqBins[bin] << endl; 
  }
  for(int bin=0; bin<=bins.PHIBINS; bin++){
    cout.width(12); 
    cout << bins.phiBins[bin] << endl; 
  }

  return 0;
} 
