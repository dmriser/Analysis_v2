#ifndef TestPhysicsEventBuilder_cxx
#define TestPhysicsEventBuilder_cxx

#include "TestPhysicsEventBuilder.h"

#include <iostream>
using namespace std; 

 // Put your includes here 
#include "CommonTools.h"
#include "DBins.h" 
#include "h22Event.h" 
#include "h22Reader.h"
#include "PhysicsEvent.h"
#include "PhysicsEventCut.h"
#include "PhysicsEventSelector.h"

// Root
#include "TLorentzVector.h"

 // Class Constructor/Destructor 
TestPhysicsEventBuilder::TestPhysicsEventBuilder(){ 
  beam = TLorentzVector(0, 0, 5.498, 5.498);
  target = TLorentzVector(0,0,0,proton_mass);  
}

TestPhysicsEventBuilder::~TestPhysicsEventBuilder(){ 

}

void TestPhysicsEventBuilder::Loop(){

  PhysicsEventBuilder eventBuilder = PhysicsEventBuilder(beam, target); 
  PhysicsEventBuilder noArgsEventBuilder; 
  
  // -------------- Elastic event criteria ----------------
  PhysicsEventCut_MissingMass * missingMassCut = new PhysicsEventCut_MissingMass();
  missingMassCut->set_min(0.8);
  missingMassCut->set_max(1.1);

  PhysicsEventSelector elasticSelector; 
  elasticSelector.add_cut( missingMassCut ); 
  elasticSelector.enable_all(); 

  // -------------- Inclusive event criteria ----------------
  PhysicsEventCut_w * wCut = new PhysicsEventCut_w();
  wCut->set_min(2.05);
  wCut->set_max(100.0);

  PhysicsEventCut_qq * qqCut = new PhysicsEventCut_qq();
  qqCut->set_min(1.00);
  qqCut->set_max(100.0);

  PhysicsEventCut_y * yCut = new PhysicsEventCut_y();
  yCut->set_min(-0.01);
  yCut->set_max(0.9);

  PhysicsEventSelector inclusiveSelector; 
  inclusiveSelector.add_cut( wCut );
  inclusiveSelector.add_cut( qqCut );
  inclusiveSelector.add_cut( yCut ); 
  inclusiveSelector.enable_all(); 

 // Event loop below. 
  for(int ievent=0; ievent<GetEntries(); ievent++){
    GetEntry(ievent); 
    
    for (int ipart = 0; ipart < event.gpart; ipart++){
      
      // Using user_ana pid to find electrons
      if (event.id[ipart] == 11) {
	TLorentzVector detectedElectron(event.p[ipart]*event.cx[ipart],
					event.p[ipart]*event.cy[ipart],
					event.p[ipart]*event.cz[ipart],
					event.p[ipart]);
	
	PhysicsEvent thisEvent = eventBuilder.getPhysicsEvent(detectedElectron);
	PhysicsEvent thisNoArgsEvent = noArgsEventBuilder.getPhysicsEvent(detectedElectron); 

	cout << thisNoArgsEvent.w << " " << thisEvent.w << endl; 
	
	elasticSelector.passes(thisEvent);
	inclusiveSelector.passes(thisEvent);
	
      }
    } 
  } 

  elasticSelector  .summarize();
  inclusiveSelector.summarize(); 
  
}
#endif
