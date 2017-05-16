
#include <iostream>

#include "common/Histos.h"
#include "common/Plotter.h"

#include "CommonTools.h"

int main(int nargs, char *args[]){

  
  if (nargs < 2){
    std::cout << " Please pass in the root file which contains asymmetry histograms. " << std::endl; 
    return 0;
  }
  
  
  std::string inputFile(args[1]); 

  AsymmetryHistograms h("test"); 
  h.Load(inputFile, 2);

  Plotter p(&h); 
  p.PlotGridXZKp();   
  p.PlotGridXZKm();   
  p.PlotGridPtZKp();   
  p.PlotGridPtZKm();   
  p.PlotX(); 
  p.PlotZ(); 
  p.PlotPt(); 

  return 0;
}
