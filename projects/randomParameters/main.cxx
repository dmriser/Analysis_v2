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

  const int NFILES = 400; 

  Parameters minPars, maxPars; 
  minPars    .loadParameters("pars/data_loose.pars"); 
  maxPars    .loadParameters("pars/data_tight.pars"); 

  std::map<int, std::string> parsToVary; 
  parsToVary[0]  = "EL_VZ_MIN"; 
  parsToVary[1]  = "EL_VZ_MAX"; 
  parsToVary[2]  = "EL_ECU_MAX"; 
  parsToVary[3]  = "EL_ECU_MIN"; 
  parsToVary[4]  = "EL_ECV_MAX"; 
  parsToVary[5]  = "EL_ECW_MAX"; 
  parsToVary[6]  = "EL_CC_FIDA"; 
  parsToVary[7]  = "EL_CC_FIDC"; 
  parsToVary[8]  = "EL_DCR1_FIDA"; 
  parsToVary[9]  = "EL_DCR1_FIDH"; 
  parsToVary[10] = "EL_DCR3_FIDA"; 
  parsToVary[11] = "EL_DCR3_FIDH"; 
  parsToVary[12] = "EL_EC_NSIGMA"; 
  parsToVary[13] = "EL_CCT_NSIGMA"; 

  TRandom3 rand(seed); 

  for (int i=0; i<NFILES; i++){  
      Parameters nominalPars; 
      nominalPars.loadParameters("pars/data.pars"); 

    for (std::pair<int, std::string> parSet : parsToVary){
      double randomPar = getRandomParameter(minPars, maxPars, parSet.second, 0, rand); 
      nominalPars.getParameter(parSet.second).setValue(0, randomPar); 
    }

      double randomPar = getRandomParameter(minPars, maxPars, "KP_TOFMASS_MIN", 0, rand); 
      for (int par=0; par<6; par++){
	nominalPars.getParameter("KP_TOFMASS_MIN").setValue(par, randomPar); 
	nominalPars.getParameter("PIP_TOFMASS_MAX").setValue(par, randomPar); 
      }

      nominalPars.saveParameters(Form("random_pars/random_var%d.pars", i)); 
  }

  return 0; 
}
