#ifndef keppel_inclusive_wrapper_cxx
#define keppel_inclusive_wrapper_cxx
 
#include "KeppelInclusiveWrapper.h"

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


#endif
