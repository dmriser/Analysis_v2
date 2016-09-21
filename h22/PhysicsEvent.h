
#include "TLorentzVector.h"

#ifndef physics_event_h
#define physics_event_h

class PhysicsEvent{

 public:
  PhysicsEvent(); 
  ~PhysicsEvent(); 
  
  double x, y, z;
  double beamEnergy, finalEnergy;
  double nu, qq, w, mm2;
  double phiHadron, pTransverse; 
  double targetMass; 

  /* Add anything general about the event here, no sector, no paddle number just physics, 
   ex: angle between particle 1 and 2. */
  
  TLorentzVector beamElectron, detectedElectron, targetParticle, virtualPhoton, finalState;   
  void ClearKinematics(); 
  
};

#endif
