
#include <iostream>

#include "common/Histograms.h"
#include "common/NewPlotter.h"

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

  Plotter pkm(&kmh, &kmf, Meson::kKaonNegative);
  pkm.PlotGridZX();  

  Plotter ppp(&pph, &ppf, Meson::kPionPositive);
  ppp.PlotGridZX();  

  Plotter ppm(&pmh, &pmf, Meson::kPionNegative);
  ppm.PlotGridZX();  

  return 0;
}
