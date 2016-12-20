
#include "TLorentzVector.h"
#include "PhysicsEvent.h"

#ifndef physics_event_cxx
#define physics_event_cxx

PhysicsEvent::PhysicsEvent(){

  x = 0.0;
  y = 0.0;
  z = 0.0;
  beamEnergy = 0.0; 
  finalEnergy = 0.0;
  nu = 0.0; 
  qq = 0.0; 
  w = 0.0; 
  mm2 = 0.0; 
  phiHadron = 0.0;
  pT = 0.0;
  targetMass = 0.0; 
  
}

PhysicsEvent::~PhysicsEvent(){


} 
  

void PhysicsEvent::ClearKinematics(){ 

  x = 0.0;
  y = 0.0;
  z = 0.0;
  finalEnergy = 0.0;
  nu = 0.0; 
  qq = 0.0; 
  w = 0.0; 
  mm2 = 0.0; 
  phiHadron = 0.0;
  pT = 0.0;
  
}

#endif
