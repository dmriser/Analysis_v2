#ifndef lund_event_h 
#define lund_event_h 

#include <iostream>
#include <vector>

class LundHeader{

 public:
  LundHeader();
  ~LundHeader();

  inline LundHeader(int numberParticles, double beamPolar){
    numberOfParticles = numberParticles; beamPolarization = beamPolar;
  }

 public:
  int numberOfParticles, numberOfTargetNucleons, numberOfTargetProtons;
  double beamPolarization, targetPolarization, x, y, w, qq, nu;

  void SetHeader(int numberParticles, double beamPolar);
  void SetKinematics(double thisX, double thisY, double thisW, double thisQQ, double thisNu){
    x = thisX; y = thisY; w = thisW; qq = thisQQ; nu = thisNu; 
  }

  std::string toString();
  void Print();
};

class LundParticle{
 public:
  LundParticle();
  ~LundParticle();

  inline LundParticle(int thisType, int thisPID, double thisPx, double thisPy, double thisPz,
	       double thisVx, double thisVy, double thisVz){
    type = thisType;  PID = thisPID; px = thisPx;
    py = thisPy;  pz = thisPz;  vx = thisVx;
    vy = thisVy;  vz = thisVz;
  }

 public:
  int index, charge, type, PID, parentIndex, daughterIndex; 
  double px, py, pz, vx, vy, vz, energy, mass;

  void SetParticle(int thisType, int thisPID, double thisPx, double thisPy, double thisPz,
		   double thisVx, double thisVy, double thisVz){
    type = thisType;  PID = thisPID; px = thisPx;
    py = thisPy;  pz = thisPz;  vx = thisVx;
    vy = thisVy;  vz = thisVz;
  }

  std::string toString();
  void Print();
};

class LundEvent{
 public:
  LundEvent();
  ~LundEvent();
  
 public:
  LundHeader eventHeader; 
  std::vector<LundParticle> eventParticles; 

  void PrintEvent();
  void SetHeader(LundHeader thisHeader);
  void AddParticle(LundParticle thisParticle){ eventParticles.push_back(thisParticle); }
};

#endif
