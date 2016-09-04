#include <iostream>
#include <vector> 
#include <fstream>
#include <cstdlib>
using namespace std; 

#include "DBins.h"
#include "DISPackage.h"
#include "Dumper.h"
#include "h22Option.h"

int main(int argc, char * argv[]){
  
  // Binning Scheme 
  DBins xBins(40, 0.05, 0.65);
  DBins qqBins(10, 1.0, 5.0); 

  DISHistograms histos;
  histos.output_name = "dis_devel";
  histos.set_bins(xBins, qqBins); 
  histos.init();
  histos.load();
  histos.draw();
  
  return 0;
}
