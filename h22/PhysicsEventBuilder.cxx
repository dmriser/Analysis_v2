
#include <cmath>

#include "CommonTools.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"

#ifndef physics_event_builder_cxx
#define physics_event_builder_cxx

PhysicsEventBuilder::PhysicsEventBuilder(){
  //  by default building for my experiment
  physicsEvent.beamElectron   = TLorentzVector(0, 0, 5.498, 5.498);
  physicsEvent.targetParticle = TLorentzVector(0, 0,0,proton_mass);
  physicsEvent.beamEnergy     = physicsEvent.beamElectron.E();
  physicsEvent.targetMass     = physicsEvent.targetParticle.E(); 
}

PhysicsEventBuilder::PhysicsEventBuilder(TLorentzVector beam, TLorentzVector target){
  physicsEvent.beamElectron   = beam;
  physicsEvent.targetParticle = target;
  physicsEvent.beamEnergy     = physicsEvent.beamElectron.E();
  physicsEvent.targetMass     = physicsEvent.targetParticle.E(); 
}

PhysicsEventBuilder::~PhysicsEventBuilder(){
}

PhysicsEvent &PhysicsEventBuilder::getPhysicsEvent(TLorentzVector &electron){
  physicsEvent.ClearKinematics(); 
  setVectors(electron);
  setKinematics();
  return physicsEvent; 
}

PhysicsEvent &PhysicsEventBuilder::getPhysicsEvent(TLorentzVector &electron, TLorentzVector &particle){
  physicsEvent.particle1 = particle; 
  physicsEvent.ClearKinematics(); 
  setVectors(electron);
  physicsEvent.finalState -= particle;
  setKinematics();

  // Adapted from DisFunctions.C
  // S. Pisanos
  //  physicsEvent.pT = particle.Vect().Perp((physicsEvent.virtualPhoton.Vect()));  

  physicsEvent.z  = particle.T()/physicsEvent.virtualPhoton.T();

  TVector3 boost = -(physicsEvent.virtualPhoton + physicsEvent.targetParticle).BoostVector();
  Double_t c0, c1, c2, c3;
  TVector3 v0, v1;

  // you must clone before boosting otherwise you destroy the lab
  // frame vectors and you will get confused in your orig. code.
  TLorentzVector photon(physicsEvent.virtualPhoton);
  TLorentzVector electronClone(electron); 
  TLorentzVector particleClone(particle); 
  photon  .Boost(boost);
  electronClone.Boost(boost);
  particleClone.Boost(boost);

  v0 = photon.Vect().Cross(electronClone.Vect());
  v1 = photon.Vect().Cross(particleClone.Vect());
  c0 = v0.Dot(particleClone.Vect());
  c1 = v0.Dot(v1);
  c2 = v0.Mag();
  c3 = v1.Mag();
  physicsEvent.phiHadron = to_degrees*(c0/TMath::Abs(c0)) * TMath::ACos(c1 / (c2*c3) );
  physicsEvent.pT        = particleClone.Perp();
  physicsEvent.thetaHadron = particleClone.Theta()*to_degrees; 

  return physicsEvent; 
}

PhysicsEvent &PhysicsEventBuilder::getPhysicsEvent(TLorentzVector &electron, TLorentzVector &particle1, TLorentzVector &particle2){
  physicsEvent.particle1 = particle1; 
  physicsEvent.particle2 = particle2; 
  physicsEvent.ClearKinematics(); 
  setVectors(electron);
  physicsEvent.finalState -= (particle1+particle2);
  setKinematics();
  return physicsEvent; 
}

void PhysicsEventBuilder::setVectors(TLorentzVector &electron){
  // Set 4-Vectors
  physicsEvent.detectedElectron = electron; 
  physicsEvent.finalEnergy      = physicsEvent.detectedElectron.E(); 
  physicsEvent.virtualPhoton    = physicsEvent.beamElectron - physicsEvent.detectedElectron;
  physicsEvent.finalState       = physicsEvent.targetParticle + physicsEvent.virtualPhoton;
}

void PhysicsEventBuilder::setKinematics(){
  // Get the numbers 
  physicsEvent.nu  = physicsEvent.beamEnergy - physicsEvent.finalEnergy;
  physicsEvent.qq  = -1*physicsEvent.virtualPhoton.Mag2();
  physicsEvent.x   = physicsEvent.qq/(2*physicsEvent.targetParticle.E()*physicsEvent.virtualPhoton.E()); 
  physicsEvent.w   = sqrt(physicsEvent.targetParticle.Mag2()+ (physicsEvent.qq/physicsEvent.x)*(1-physicsEvent.x));  
  physicsEvent.mm2 = physicsEvent.finalState.Mag2();
  physicsEvent.y   = physicsEvent.nu/physicsEvent.beamEnergy;
}

#endif
