#ifndef lund_event_cxx
#define lund_event_cxx 

#include "LundEvent.h"
#include <iostream>
using namespace std;

#include "TLorentzVector.h"
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
  string message = "unsupported";
  return message;
}

void LundHeader::Print(){
  cout.width(4); cout << numberOfParticles; 
  cout.width(4); cout << numberOfTargetNucleons; 
  cout.width(4); cout << numberOfTargetProtons; 
  cout.width(12); cout << beamPolarization; 
  cout.width(12); cout << targetPolarization; 
  cout.width(12); cout << x; 
  cout.width(12); cout << y; 
  cout.width(12); cout << w; 
  cout.width(12); cout << qq; 
  cout.width(12); cout << nu << endl; 
}

// -------------------------------------------------------------------------
// LundParticle
LundParticle::LundParticle(){
  
}

LundParticle::~LundParticle(){
}


string LundParticle::toString(){
  string message = "feature not supported yet";
  return message;
}

void LundParticle::Print(){
  cout.width(4); cout << index;
  cout.width(4); cout << charge;
  cout.width(4); cout << type;
  cout.width(6); cout << PID;
  cout.width(4); cout << parentIndex;
  cout.width(4); cout << daughterIndex;
  cout.width(12); cout << px; 
  cout.width(12); cout << py; 
  cout.width(12); cout << pz; 
  cout.width(12); cout << energy; 
  cout.width(12); cout << mass; 
  cout.width(12); cout << vx; 
  cout.width(12); cout << vy; 
  cout.width(12); cout << vz << endl; 
}

TLorentzVector LundParticle::GetTLorentzVector(){
  double p2 = pow(px,2) + pow(py,2) + pow(pz,2);
  return TLorentzVector(px, py, pz, sqrt(p2 + pow(mass, 2)));
}

// -------------------------------------------------------------------------
// LundEvent
LundEvent::LundEvent(){

}

LundEvent::~LundEvent(){

}
  
void LundEvent::PrintEvent(){
  
  eventHeader.Print();

  for(int particle=0; particle<eventHeader.numberOfParticles; particle++){
    eventParticles[particle].Print();
  }

}

void LundEvent::SetHeader(LundHeader thisHeader){

  eventHeader.numberOfParticles = thisHeader.numberOfParticles; 
  eventHeader.numberOfTargetNucleons = thisHeader.numberOfTargetNucleons; 
  eventHeader.numberOfTargetProtons = thisHeader.numberOfTargetProtons; 
  eventHeader.beamPolarization = thisHeader.beamPolarization; 
  eventHeader.targetPolarization = thisHeader.targetPolarization; 
  eventHeader.x = thisHeader.x; 
  eventHeader.y = thisHeader.y; 
  eventHeader.w = thisHeader.w; 
  eventHeader.qq = thisHeader.qq; 
  eventHeader.nu = thisHeader.nu;

}

LundParticle LundEvent::GetParticle(int particle){

  LundParticle nullParticle;

  if (particle < eventParticles.size()){
    return eventParticles[particle];
  }

  cout << "[LundEvent::GetParticle] Index requested outside of range of particles in event! " << endl;
  return nullParticle; 
}

#endif
