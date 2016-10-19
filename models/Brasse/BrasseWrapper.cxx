#ifndef brasse_wrapper_cxx
#define brasse_wrapper_cxx

#include "BrasseWrapper.h"

extern"C"{
  double brasse_(double *beamEnergy, double *qq, double *w);
}

double BrasseInclusiveModel::getValue(double beamEnergy, double qq, double w){
  double value = 0.0; 
  value = brasse_(&beamEnergy, &qq, &w);
  return value; 
}

#endif 
