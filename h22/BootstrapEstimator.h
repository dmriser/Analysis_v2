#ifndef BOOTSTRAP_H
#define BOOTSTRAP_H

#include <vector>

#include "TF1.h"
#include "TH1.h"
#include "TRandom3.h"

class BootstrapEstimator {

 public:
  BootstrapEstimator(){
    BootstrapEstimator(100);
  }

 BootstrapEstimator(int numberReplicas) : fNumberReplicas(numberReplicas) {
    fRandom = new TRandom3(); 

    fFitFunction = "gaus";
  }

  ~BootstrapEstimator();

  void Fit(TH1F *sourceHisto);
  void SetFitFunction(const std::string &fitFunction){
    fFitFunction = fitFunction; 
  } 

 protected:
  int fNumberReplicas; 
  
  std::vector< std::vector<float> > fMean; 
  std::vector< std::vector<float> > fVariance; 
  std::vector<TH1F*> fReplicas; 
  std::vector<TF1*>  fFits; 
  
  std::string fFitFunction; 
  TRandom3 *fRandom; 

};

#endif
