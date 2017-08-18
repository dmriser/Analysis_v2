#include <iostream>

#include "common/Config.h"
#include "common/ConfigLoader.h"
#include "common/ConfigLoader.cxx"
#include "common/PhiHistograms.h"
#include "common/Plotting.h"

int main(int argc, char *argv[]){

  Config c = ConfigLoader::getConfiguration("/volatile/clas12/dmriser/analysis/asymmetry/pi_n_basic/config.json");


  PhiHistos *asym = new PhiHistos();
  asym->Load(c.axes, Form("%s/%s/fits.root", c.analysisHome.c_str(), c.name.c_str()), "asym");

  PhiHistos *err = new PhiHistos(); 
  err->Load(c.axes, Form("%s/%s/fits.root", c.analysisHome.c_str(), c.name.c_str()), "fit_confidenceInterval");

  Plotting::Setup();
  Plotting::PlotOnePhiHisto(asym,
			    "A_{LU}^{sin(#phi)}",
			    "#phi_{h}",
			    "A_{LU}^{sin(#phi)}",
			    "hist",
			    1,
			    Form("%s/%s/plots/asymmetry/test.pdf",c.analysisHome.c_str(),c.name.c_str())); 

  Plotting::PlotTwoPhiHistos(asym, err,
			    "A_{LU}^{sin(#phi)}",
			    "#phi_{h}",
			    "A_{LU}^{sin(#phi)}",
			    "histpe",
			    "e3same",
			     0, 1,
			    Form("%s/%s/plots/asymmetry/test_err.pdf",c.analysisHome.c_str(),c.name.c_str())); 

  return 0;
}
