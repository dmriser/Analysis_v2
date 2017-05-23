
#include <iostream>

#include "common/Histograms.h"
#include "common/Plotter.h"

#include "CommonTools.h"

int main(int nargs, char *args[]){
  
  if (nargs < 2){
    std::cout << " Please pass in the root file which contains asymmetry histograms. " << std::endl; 
    return 0;
  }
  
  std::string inputFile(args[1]); 

  Histos kph("test", Meson::kKaonPositive);
  kph.Load(inputFile, 2);

  Histos kmh("test", Meson::kKaonNegative);
  kmh.Load(inputFile, 2);

  Histos pph("test", Meson::kPionPositive);
  pph.Load(inputFile, 2);

  Histos pmh("test", Meson::kPionNegative);
  pmh.Load(inputFile, 2);

  Fits kpf("test", Meson::kKaonPositive);
  kpf.Load(inputFile, 1);

  Fits kmf("test", Meson::kKaonNegative);
  kmf.Load(inputFile, 1);

  Fits ppf("test", Meson::kPionPositive);
  ppf.Load(inputFile, 1);

  Fits pmf("test", Meson::kPionNegative);
  pmf.Load(inputFile, 1);

  Plotter pkp(&kph, &kpf, Meson::kKaonPositive);
  pkp.PlotGridZX();  
  pkp.PlotGridZPt();  

  Plotter pkm(&kmh, &kmf, Meson::kKaonNegative);
  pkm.PlotGridZX();  
  pkm.PlotGridZPt();  

  Plotter ppp(&pph, &ppf, Meson::kPionPositive);
  ppp.PlotGridZX();  
  ppp.PlotGridZPt();  

  Plotter ppm(&pmh, &pmf, Meson::kPionNegative);
  ppm.PlotGridZX();  
  ppm.PlotGridZPt();  

  IntegratedHistos kp_int(inputFile, Meson::kKaonPositive); 
  IntegratedHistos km_int(inputFile, Meson::kKaonNegative); 
  IntegratedHistos pp_int(inputFile, Meson::kPionPositive); 
  IntegratedHistos pm_int(inputFile, Meson::kPionNegative); 

  IntegratedPlotter plot_int;
  plot_int.Add(&kp_int, Meson::kKaonPositive); 
  plot_int.Add(&km_int, Meson::kKaonNegative); 
  plot_int.Add(&pp_int, Meson::kPionPositive); 
  plot_int.Add(&pm_int, Meson::kPionNegative); 

  plot_int.PlotX(); 
  plot_int.PlotZ(); 
  plot_int.PlotPt(); 
  plot_int.PlotXByZ(Meson::kPionPositive);
  plot_int.PlotXByZ(Meson::kKaonPositive);
  plot_int.PlotSummary(); 

  return 0;
}
