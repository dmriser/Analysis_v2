#ifndef lund_event_cxx
#define lund_event_cxx 

#include "LundEvent.h"
#include <iostream>
using namespace std;

#include "TString.h"

// -------------------------------------------------------------------------
// LundHeader
LundHeader::LundHeader(){
  // Init header variables to 0
  numberOfParticles = 0;
  numberOfTargetNucleons = 0;
  numberOfTargetProtons = 0;
  targetPolarization = 0.0; 
  beamPolarization = 0.0; 
  x = 0.0; y = 0.0; w = 0.0;
  qq = 0.0; nu = 0.0;
}

LundHeader::~LundHeader(){
}

void LundHeader::SetHeader(int numberParticles, double beamPolar){
  numberOfParticles = numberParticles; 
  beamPolarization = beamPolar; 
}

string LundHeader::toString(){
  return Form("%d    %d    %d    %f    %f    %f    %f    %f    %f    %f",numberOfParticles,numberOfTargetNucleons,
	      numberOfTargetProtons,targetPolarization,beamPolarization,x,y,w,qq,nu);
}

// -------------------------------------------------------------------------
// LundParticle
LundParticle::LundParticle(){

}

LundParticle::~LundParticle(){
}


string LundParticle::toString(){

}

// -------------------------------------------------------------------------
// LundEvent
LundEvent::LundEvent(){

}

LundEvent::~LundEvent(){

}
  
void LundEvent::PrintEvent(){

}

#endif
