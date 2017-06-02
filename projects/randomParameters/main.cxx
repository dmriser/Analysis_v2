#include <iostream>
#include <vector>
#include <map>
using std::cout;
using std::endl;

// MyLibs
#include "Parameters.h"

#include "TRandom3.h"

inline double mean(std::vector<double> x){
  double tot = 0.0; 
  for (int i=0; i<x.size(); i++) tot += x[i]; 
  return tot/x.size();
}

inline double getRandomParameter(Parameters& min, Parameters& max, std::string name, int value, TRandom3& rand){
  return rand.Uniform()*(max.getParameter(name).getValue(value)-min.getParameter(name).getValue(value)) + min.getParameter(name).getValue(value); 
}

int main(int argc, char *argv[]){

  int seed = 12345; 
  if (argc > 1){
    seed = atoi(argv[1]); 
  }

  Parameters minPars, maxPars; 
  minPars    .loadParameters("pars/data_loose.pars"); 
  maxPars    .loadParameters("pars/data_tight.pars"); 

  std::map<int, std::string> parsToVary; 
  parsToVary[0] = "KP_TOFMASS_MIN"; 

  TRandom3 rand(seed); 

  for (auto& parSet : parsToVary){
    for (int i=0; i<10; i++){  
      Parameters nominalPars; 
      nominalPars.loadParameters("pars/data.pars"); 
 
      double randomPar = getRandomParameter(minPars, maxPars, parSet.second, 0, rand); 
      nominalPars.getParameter(parSet.second).setValue(0, randomPar); 
      nominalPars.getParameter(parSet.second).setValue(1, randomPar); 
      nominalPars.getParameter(parSet.second).setValue(2, randomPar); 
      nominalPars.getParameter(parSet.second).setValue(3, randomPar); 
      nominalPars.getParameter(parSet.second).setValue(4, randomPar); 
      nominalPars.getParameter(parSet.second).setValue(5, randomPar); 

      nominalPars.getParameter("PIP_TOFMASS_MAX").setValue(0, randomPar); 
      nominalPars.getParameter("PIP_TOFMASS_MAX").setValue(1, randomPar); 
      nominalPars.getParameter("PIP_TOFMASS_MAX").setValue(2, randomPar); 
      nominalPars.getParameter("PIP_TOFMASS_MAX").setValue(3, randomPar); 
      nominalPars.getParameter("PIP_TOFMASS_MAX").setValue(4, randomPar); 
      nominalPars.getParameter("PIP_TOFMASS_MAX").setValue(5, randomPar); 
      nominalPars.saveParameters(Form("random_pars/random_par%d_var%d.pars",parSet.first,i)); 
    }
  }

  return 0; 
}
