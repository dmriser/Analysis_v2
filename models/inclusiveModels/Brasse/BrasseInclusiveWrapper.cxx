#ifndef brasse_inclusive_wrapper_cxx
#define brasse_inclusive_wrapper_cxx
 
#include "BrasseInclusiveWrapper.h"
#include <cmath>

extern"C"{
  float brasse_sub_(float *e,float *q2,float *w1, float *dsigma);
}

BrasseInclusiveWrapper::BrasseInclusiveWrapper(){

}

BrasseInclusiveWrapper::~BrasseInclusiveWrapper(){

}

float BrasseInclusiveWrapper::GetXS(float beamEnergy, float qq, float w){
  float result = 0.0; 
  brasse_sub_(&beamEnergy, &qq, &w, &result);
  return result; 
}

float BrasseInclusiveWrapper::GetXSByX(float beamEnergy, float qq, float x){

  float w = sqrt(pow(0.938,2)-qq+qq/x); 
  float result = 0.0; 
  brasse_sub_(&beamEnergy, &qq, &w, &result);
  return result*(-0.5*(qq/(pow(x,2)*w)));
}


#endif
