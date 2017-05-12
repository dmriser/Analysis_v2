
#include <iostream>

#include "common/Histos.h"

#include "CommonTools.h"

int main(int nargs, char *args[]){

  
  if (nargs < 2){
    std::cout << " Please pass in the root file which contains asymmetry histograms. " << std::endl; 
    return 0;
  }
  
  
  std::string inputFile(args[1]); 

  AsymmetryHistograms h("test"); 
  h.Load(inputFile, 1);
  h.Fit();
  h.Save(inputFile, "update", 2);
  

  return 0;
}
