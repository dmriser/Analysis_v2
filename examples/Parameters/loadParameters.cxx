#include <iostream>
using namespace std; 

#include "Parameters.h"
#include "ParameterSet.h"

int main(){

  Parameters pars; 
  pars.loadParameters("pars.dat");
  pars.saveParameters("loadedAndSavedPars.dat");

  return 0;
}
