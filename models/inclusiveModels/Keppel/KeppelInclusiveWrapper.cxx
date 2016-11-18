#ifndef keppel_inclusive_wrapper_cxx
#define keppel_inclusive_wrapper_cxx
 
#include "KeppelInclusiveWrapper.h"
#include <cmath>

extern"C"{
  float tkeppel_(float *e,float *q2,float *w1);
}

KeppelInclusiveWrapper::KeppelInclusiveWrapper(){

}

KeppelInclusiveWrapper::~KeppelInclusiveWrapper(){

}

float KeppelInclusiveWrapper::GetXS(float beamEnergy, float qq, float w){
  return tkeppel_(&beamEnergy, &qq, &w);
}

float KeppelInclusiveWrapper::GetXSByX(float beamEnergy, float qq, float x){

  float w = sqrt(pow(0.938,2)-qq+qq/x); 
  float xs = tkeppel_(&beamEnergy, &qq, &w);
  //  float jacobian = (qq/(w*x))*(0.5-1/x); 
  float jacobian = qq/(2*w*x*x); 
  return xs*jacobian;
}


#endif
