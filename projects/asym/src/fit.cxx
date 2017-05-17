
#include <iostream>

#include "common/Fits.h"
#include "common/Histograms.h"
#include "common/Types.h"

#include "CommonTools.h"

int main(int nargs, char *args[]){

  
  if (nargs < 2){
    std::cout << " Please pass in the root file which contains asymmetry histograms. " << std::endl; 
    return 0;
  }
  
  std::string inputFile(args[1]); 

  Histos kp_hist("test", Meson::kKaonPositive); 
  kp_hist.Load(inputFile, 1);
  kp_hist.CalculateAsymmetry(); 

  Histos km_hist("test", Meson::kKaonNegative); 
  km_hist.Load(inputFile, 1);
  km_hist.CalculateAsymmetry(); 

  Histos pp_hist("test", Meson::kPionPositive); 
  pp_hist.Load(inputFile, 1);
  pp_hist.CalculateAsymmetry(); 

  Histos pm_hist("test", Meson::kPionNegative); 
  pm_hist.Load(inputFile, 1);
  pm_hist.CalculateAsymmetry(); 

  Fits kp_fit(&kp_hist, "test", Meson::kKaonPositive); 
  kp_fit.Fit(); 
  kp_fit.Save(inputFile, "update", 1); 

  Fits km_fit(&km_hist, "test", Meson::kKaonNegative); 
  km_fit.Fit(); 
  km_fit.Save(inputFile, "update", 1); 

  Fits pp_fit(&pp_hist, "test", Meson::kPionPositive); 
  pp_fit.Fit(); 
  pp_fit.Save(inputFile, "update", 1); 

  Fits pm_fit(&pm_hist, "test", Meson::kPionNegative); 
  pm_fit.Fit(); 
  pm_fit.Save(inputFile, "update", 1); 
   
  return 0;
}
