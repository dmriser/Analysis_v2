#include "common/Bins.h"
#include "common/BinFactory.h"
#include "common/Constants.h"
#include "common/Histograms.h"
#include "common/Types.h"

#include "TFile.h"


int main(){

  Histos test_histos("test_histos", Meson::kPionPositive); 
  //  test_histos.Save("out/test_histos.root","recreate",1); 

  return 0; 
}
