#ifndef TestPhysicsEventBuilder_h
#define TestPhysicsEventBuilder_h


 // Put your includes here 
#include "DBins.h" 
#include "DCut.h" 
#include "DEvent.h" 
#include "DSelection.h" 
#include "h22Event.h" 
#include "h22Reader.h" 
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"

// root headers
#include "TLorentzVector.h"

class TestPhysicsEventBuilder : public h22Reader {
 public:
  TestPhysicsEventBuilder();
  ~TestPhysicsEventBuilder();
  
  TLorentzVector beam, target; 
  
  void Loop();
};
#endif
