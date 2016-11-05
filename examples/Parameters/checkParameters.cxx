#include <iostream>
using namespace std; 

#include "Parameters.h"
#include "ParameterSet.h"
#include "TRandom3.h"

int main(){

  TRandom3 rand; 

  Parameters pars; 

  ParameterSet trialSet;
  trialSet.setName("trialSet");

  ParameterSet trialSet2;
  trialSet2.setName("trialSet2");

  int numberOfParams = 6; 
  for (int p=0; p< numberOfParams; p++){
    trialSet.addValue(rand.Gaus(1.0, 1.0));
    trialSet.addError(rand.Gaus(0.1, 0.1));
    trialSet2.addValue(rand.Gaus(1.0, 1.0));
    trialSet2.addError(rand.Gaus(0.1, 0.1));
  }

  pars.addParameterSet(trialSet); 
  pars.addParameterSet(trialSet2); 
  pars.setNumberOfFiles(12); 
  pars.saveParameters("pars.dat");

  return 0; 
}
