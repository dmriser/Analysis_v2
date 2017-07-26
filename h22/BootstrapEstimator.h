#ifndef BOOTSTRAP_H
#define BOOTSTRAP_H

#include <vector>

#include "TF1.h"
#include "TH1.h"

class BootstrapEstimator {

 public:
  BootstrapEstimator(){
    BootstrapEstimator(100);
  }

 BootstrapEstimator(int numberReplicas) : fNumberReplicas(numberReplicas) {

  }

  ~BootstrapEstimator();

 protected:
  int fNumberReplicas; 
  
  std::vector<float> fMean, fVariance; 
  std::vector< std::vector<TH1F*> > fReplicas; 
  

};

#endif
