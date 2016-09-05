#include <iostream>
#include <vector> 
#include <fstream>
#include <cstdlib>
using namespace std; 

#include "DBins.h"
#include "DISPackage.h"
#include "Dumper.h"
#include "h22Option.h"

int PrintUsage(); 

int main(int argc, char * argv[]){

  if (argc < 2) {
    return PrintUsage();
  }
  
  // Binning Scheme for 1-D Histograms derived from 2-D Histograms 
  DBins xBins(40, 0.05, 0.65);
  DBins qqBins(10, 1.0, 5.0); 

  DISManager manager(argv[1],false); 
  manager.set_bins(xBins, qqBins); 
  manager.init();

  // We need to program so routines in DISPackage.cxx before this will work
  manager.do_xs();
  //  manager.histos.draw();
  
  return 0;
}

int PrintUsage(){
  cout << " --> Fail: Pass in the name of the root file which contains histograms!"  << endl; 
  return 0; 
}
