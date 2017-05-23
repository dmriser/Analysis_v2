#include "common/Bins.h"
#include "common/Constants.h"
#include "common/Histograms.h"
#include "common/PIDHistograms.h"
#include "common/IntegratedHistograms.h"
#include "common/Plotter.h"
#include "common/Types.h"


int main(){

  Histos test_histos("test_histos", Meson::kPionPositive); 

  Fits f("test",Meson::kPionPositive); 
  f.Load("out/refactor_fill.root",1); 

  IntegratedHistos int_histos(&f, Meson::kPionPositive);
  int_histos.Save("out/test_int_histos.root","recreate"); 

  return 0; 
}
