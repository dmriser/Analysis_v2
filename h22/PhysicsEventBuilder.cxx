
#include <cmath>

#include "CommonTools.h"
#include "TLorentzVector.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"

#ifndef physics_event_builder_cxx
#define physics_event_builder_cxx

PhysicsEventBuilder::PhysicsEventBuilder(){
  //  by default building for my experiment
  physicsEvent.beamElectron = TLorentzVector(0, 0, 5.498, 5.498);
  physicsEvent.targetParticle = TLorentzVector(0, 0,0,proton_mass);
  physicsEvent.beamEnergy = physicsEvent.beamElectron.E();
  physicsEvent.targetMass = physicsEvent.targetParticle.E(); 
}

PhysicsEventBuilder::PhysicsEventBuilder(TLorentzVector beam, TLorentzVector target){
  physicsEvent.beamElectron = beam;
  physicsEvent.targetParticle = target;
  physicsEvent.beamEnergy = physicsEvent.beamElectron.E();
  physicsEvent.targetMass = physicsEvent.targetParticle.E(); 
}

PhysicsEventBuilder::~PhysicsEventBuilder(){
}

PhysicsEvent PhysicsEventBuilder::getPhysicsEvent(TLorentzVector electron){
  physicsEvent.ClearKinematics(); 
  setVectors(electron);
  setKinematics();
  return physicsEvent; 
}

void PhysicsEventBuilder::setVectors(TLorentzVector electron){
  // Set 4-Vectors
  physicsEvent.detectedElectron = electron; 
  physicsEvent.finalEnergy = physicsEvent.detectedElectron.E(); 
  physicsEvent.virtualPhoton = physicsEvent.beamElectron - physicsEvent.detectedElectron;
  physicsEvent.finalState = physicsEvent.targetParticle + physicsEvent.virtualPhoton;
}

void PhysicsEventBuilder::setKinematics(){
  // Get the numbers 
  physicsEvent.nu = physicsEvent.beamEnergy - physicsEvent.finalEnergy;
  physicsEvent.qq = -1*physicsEvent.virtualPhoton.Mag2();
  physicsEvent.x = physicsEvent.qq/(2*physicsEvent.targetParticle.E()*physicsEvent.virtualPhoton.E()); 
  physicsEvent.mm2 = physicsEvent.targetParticle.Mag2()+ (physicsEvent.qq/physicsEvent.x)*(1-physicsEvent.x);  
  physicsEvent.w = sqrt(physicsEvent.mm2);
  physicsEvent.y = physicsEvent.nu/physicsEvent.beamEnergy;
}

#endif
