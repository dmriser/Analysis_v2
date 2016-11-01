#ifndef BOSTED_ELASTIC_WRAPPER_CXX
#define BOSTED_ELASTIC_WRAPPER_CXX

#include "BostedElasticWrapper.h"

extern"C"{
  float elas_(float *beamEnergy, float *theta);
  float elasrad_(float *beamEnergy, float *theta, float *targetRadiationLengths, float *wCut);
}

float BostedElasticWrapper::getRadiatedValue(float beamEnergy, float theta, float targetRadiationLengths, float wCut){
  return elasrad_(&beamEnergy, &theta, &targetRadiationLengths, &wCut);; 
}

float BostedElasticWrapper::getValue(float beamEnergy, float theta){
  return elas_(&beamEnergy, &theta); 
}

#endif
