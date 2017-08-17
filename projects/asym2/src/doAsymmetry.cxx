#include <iostream>
#include <vector>

#include "common/Config.h"
#include "common/ConfigLoader.h"
#include "common/ConfigLoader.cxx"
#include "common/PhiHistograms.h"
#include "common/PhiFits.h"
#include "common/Fitter.h"
#include "common/KinematicHistograms.h"

// temp 
#include "TH1.h"

int main(int argc, char *argv[]){

  if (argc < 3){
    exit(0);
  }

  PhiHistos *plus  = new PhiHistos(); 
  PhiHistos *minus = new PhiHistos(); 

  std::string file(argv[1]);

  Config c = ConfigLoader::getConfiguration(argv[2]);

  plus ->Load(c.axes, file, "countsPlus");
  minus->Load(c.axes, file, "countsMinus");

  for(std::pair<int, std::vector<TH1F*>> p : plus->histos){
    for(TH1F *h : p.second){
      std::cout << "Loaded histos " << h->GetEntries() << std::endl;     
    }
  }

  PhiHistos *asym = new PhiHistos();
  asym->CreateAsymmetry(plus, minus);

  Fitter *fitter = new Fitter(); 
  //  std::pair<PhiFits, PhiHistos> fitPair = fitter->Fit(asym,"basicFit","[0]*sin((3.14159/180.0)*x)/(1 + [1]*cos((3.14159/180.0)*x) + [2]*cos(2*(3.14159/180.0)*x))","WLER");
  std::pair<PhiFits, PhiHistos> fitPair = fitter->Fit(asym,"basicFit","[0]*sin((3.14159/180.0)*x)","WLER");

  KinematicHistos *kin = new KinematicHistos(); 
  kin->Init(c.axes, "integratedAsymmetry"); 
  kin->BuildFromFits(&fitPair.first);

  TFile *outFile = new TFile("/u/home/dmriser/Analysis_v2/projects/asym2/out/asym.root","recreate");
  asym->Save(outFile);
  fitPair.first.Save(outFile);
  fitPair.second.Save(outFile);
  kin->Save(outFile);
  outFile->Close();

  return 0; 
}
