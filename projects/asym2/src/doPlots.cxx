#include <iostream>

#include "common/Config.h"
#include "common/ConfigLoader.h"
#include "common/ConfigLoader.cxx"
#include "common/PhiHistograms.h"
#include "common/Plotting.h"
#include "common/KinematicHistograms.h"
#include "common/MissingMassHistograms.h"

int main(int argc, char *argv[]){

  if(argc < 2){
    std::cerr << "Pass in configuration files." << std::endl; 
    exit(0);
  }


  std::vector<Config> configs; 
  for(int iarg=1; iarg<argc; iarg++){
    std::string jsonFilename(argv[iarg]);
    configs.push_back(ConfigLoader::getConfiguration(jsonFilename)); 
  }

 for(Config c : configs){

   PhiHistos *plus  = new PhiHistos(); 
   PhiHistos *minus = new PhiHistos(); 
   
   plus ->Load(c.axes, Form("%s/%s/histos.root", c.analysisHome.c_str(), c.name.c_str()), "countsPlus"); 
   minus->Load(c.axes, Form("%s/%s/histos.root", c.analysisHome.c_str(), c.name.c_str()), "countsMinus"); 
   
   PhiHistos *asym = new PhiHistos();
   asym->Load(c.axes, Form("%s/%s/fits.root", c.analysisHome.c_str(), c.name.c_str()), "asym");
   
   PhiHistos *err = new PhiHistos(); 
   err->Load(c.axes, Form("%s/%s/fits.root", c.analysisHome.c_str(), c.name.c_str()), "fit_confidenceInterval");
   
   KinematicHistos *kin = new KinematicHistos(); 
   kin->Load(c.axes, Form("%s/%s/fits.root", c.analysisHome.c_str(), c.name.c_str()), "integratedAsymmetry"); 
   
   MissingMassHistos *mass = new MissingMassHistos(); 
   mass->Load(c.axes, Form("%s/%s/histos.root", c.analysisHome.c_str(), c.name.c_str()), "PassKinematic"); 
   
   Plotting::Setup();
   Plotting::PlotTwoPhiHistos(asym, err,
			      "A_{LU}^{sin(#phi)}",
			      "#phi_{h}",
			      "A_{LU}^{sin(#phi)}",
			      "histpe",
			      "e3same",
			      styles::marker_black, 
			      styles::filled_blue,
			      Form("%s/%s/plots/asymmetry/phi.pdf",c.analysisHome.c_str(),c.name.c_str())); 
   
   Plotting::PlotTwoPhiHistos(plus, minus,
			      "Phi-Dist.",
			      "#phi_{h}",
			      "Counts",
			      "hist",
			      "histsame",
			      styles::filled_red, 
			      styles::filled_blue,
			      Form("%s/%s/plots/phi/counts.pdf",c.analysisHome.c_str(),c.name.c_str())); 
   
   
   Plotting::PlotOneKinematicHisto(kin,
				   "A_{LU}^{sin(#phi)}",
				   "A_{LU}^{sin(#phi)}",
				   "histpe",
				   styles::marker_black,
				   Form("%s/%s/plots/asymmetry/integrated.pdf",c.analysisHome.c_str(),c.name.c_str())); 
   
   Plotting::PlotOneMissingMassHisto(mass,
				     "Pass MesonID",
				     "hist",
				     styles::filled_red,
				     Form("%s/%s/plots/missingMass/passKinematics.pdf",c.analysisHome.c_str(),c.name.c_str())); 
   
 }
 
 return 0;
}
