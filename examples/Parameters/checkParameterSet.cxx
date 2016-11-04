#include <iostream>
using namespace std; 

#include "ParameterSet.h"
#include "TRandom3.h"

int main(){

  TRandom3 rand; 

  ParameterSet trialSet; 
  trialSet.setName("trialParameters"); 
  trialSet.setDescription("Trial instantiation of the ParameterSet class");

  int numberOfParams = 20; 
  for (int p=0; p< numberOfParams; p++){
    trialSet.addValue(rand.Gaus(1.0, 1.0));
    trialSet.addError(rand.Gaus(0.1, 0.1));
  }

  for (int p=0; p< numberOfParams; p++){
    cout.width(16); cout << trialSet.getValue(p); 
    cout.width(16); cout << trialSet.getError(p) << endl;
  }

  return 0; 
}
