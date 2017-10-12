#ifndef physics_event_h
#define physics_event_h

#include "TLorentzVector.h"

class PhysicsEvent {
 public:
  PhysicsEvent(); 
  ~PhysicsEvent(); 
  
  double x, y, z, t;
  double beamEnergy, finalEnergy;
  double nu, qq, w, mm2;
  double phiHadron, thetaHadron, pT; 
  double targetMass; 
  double eta; 

  /* Add anything general about the event here, no sector, no paddle number just physics, 
   ex: angle between particle 1 and 2. */
  
  TLorentzVector beamElectron, detectedElectron, targetParticle, virtualPhoton, finalState, particle1, particle2;   
  void ClearKinematics(); 
  
};

#endif
