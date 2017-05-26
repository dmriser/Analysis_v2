
#include <iostream>

#include "common/Fits.h"
#include "common/Fitter.h"
#include "common/Histograms.h"
#include "common/PIDHistograms.h"
#include "common/IntegratedHistograms.h"
#include "common/SignalBackground.h"
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
  kp_hist.Save(inputFile, "update", 2); 

  Histos km_hist("test", Meson::kKaonNegative); 
  km_hist.Load(inputFile, 1);
  km_hist.CalculateAsymmetry(); 
  km_hist.Save(inputFile, "update", 2); 

  Histos pp_hist("test", Meson::kPionPositive); 
  pp_hist.Load(inputFile, 1);
  pp_hist.CalculateAsymmetry(); 
  pp_hist.Save(inputFile, "update", 2); 

  Histos pm_hist("test", Meson::kPionNegative); 
  pm_hist.Load(inputFile, 1);
  pm_hist.CalculateAsymmetry(); 
  pm_hist.Save(inputFile, "update", 2); 

  Fits kp_fit(&kp_hist, "test", Meson::kKaonPositive); 
  Fits km_fit(&km_hist, "test", Meson::kKaonNegative); 
  Fits pp_fit(&pp_hist, "test", Meson::kPionPositive); 
  Fits pm_fit(&pm_hist, "test", Meson::kPionNegative); 

  // Uses just the A sin(phi) 
  BasicFitter fitter; 

  // Uses A sin(phi)/(1 + B cos(phi) + C cos(2phi))
  //  BasicAllMomentFitter fitter; 
  fitter.Fit(&kp_hist, &kp_fit); 
  fitter.Fit(&km_hist, &km_fit); 
  fitter.Fit(&pp_hist, &pp_fit); 
  fitter.Fit(&pm_hist, &pm_fit); 

  kp_fit.Save(inputFile, "update", 1); 
  km_fit.Save(inputFile, "update", 1); 
  pp_fit.Save(inputFile, "update", 1); 
  pm_fit.Save(inputFile, "update", 1);   

  IntegratedHistos kp_integ(&kp_fit, "base", Meson::kKaonPositive);
  IntegratedHistos km_integ(&km_fit, "base", Meson::kKaonNegative);
  IntegratedHistos pp_integ(&pp_fit, "base", Meson::kPionPositive);
  IntegratedHistos pm_integ(&pm_fit, "base", Meson::kPionNegative);

  kp_integ.Save(inputFile, "update"); 
  km_integ.Save(inputFile, "update"); 
  pp_integ.Save(inputFile, "update"); 
  pm_integ.Save(inputFile, "update"); 

  PidHistos pp_pid("test", Meson::kPionPositive);
  pp_pid.Load(inputFile);

  SignalBackgroundFitter sb_fitter(&pp_pid, "test", Meson::kPionPositive); 
  sb_fitter.Fit(0.0, 0.4, 0.7); 
  sb_fitter.CorrectAsymmetry(&pp_fit, &kp_fit); 

  IntegratedHistos *pp_corr_int = sb_fitter.GetPionHistos(); 
  IntegratedHistos *kp_corr_int = sb_fitter.GetKaonHistos(); 
  
  pp_corr_int->Save(inputFile, "update"); 
  kp_corr_int->Save(inputFile, "update"); 

  return 0;
}
