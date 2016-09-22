
// c++ includes 
#include <cmath>
#include <iostream>
#include <vector>

// my includes 
#include "BaseDISHistograms.h"
#include "CommonTools.h"
#include "MCLoader.h"
#include "h22Reader.h"
#include "HistogramLoader.h"
#include "NathanArchive.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"
#include "PhysicsEventSelector.h"

// root 
#include "TLorentzVector.h"

#ifndef mc_loader_cxx
#define mc_loader_cxx

MCLoader::MCLoader(PhysicsEventSelector *eventCriteria, std::string outputFile, std::string saveOpts) : HistogramLoader(eventCriteria, outputFile, saveOpts){

}

MCLoader::~MCLoader(){
  // Anything to do here? 
}

void MCLoader::Initialize(){
  recEvents.Init("recEvents","Reconstructed MC Hits");
  genEvents.Init("genEvents","Generated MC Events");
  recAndGenEvents.Init("recAndGenEvents","Rec/Gen Same Bin Events");
}

// This is the core routine which conditionally fills histograms. 
void MCLoader::ProcessEvent(){

  // Deal with the generated first.
  TLorentzVector genElectron = event.gen_particle(11);   
  PhysicsEvent genPhysicsEvent = builder.getPhysicsEvent(genElectron); 
  int mcsect = floor(to_degrees*genElectron.Phi()/60)+1;
  if (mcsect > 0) { genEvents.Fill(genPhysicsEvent, mcsect); }
  
  eID.set_info(runno(),GSIM);
  int e_index = eID.get_electron(event);
  if (e_index > -123){
    TLorentzVector recElectron(event.cx[e_index]*event.p[e_index],
			       event.cy[e_index]*event.p[e_index],
			       event.cz[e_index]*event.p[e_index],
			       event.p[e_index]);
    
    int sector = event.dc_sect[e_index]; 
    PhysicsEvent recPhysicsEvent = builder.getPhysicsEvent(recElectron);
    
    if (eventSelector->passes(recPhysicsEvent) && sector > 0) {
      recEvents.Fill(recPhysicsEvent, sector);
    }
  } 
}

void MCLoader::Save(){
  // You may want to recreate the file, but dont overwrite it by
  // doing "RECREATE" on the second 2 saves. 
  recEvents.Save(outputFilenameWithExtension, saveOption);
  genEvents.Save(outputFilenameWithExtension, "UPDATE");   
  recAndGenEvents.Save(outputFilenameWithExtension, "UPDATE");
}

#endif
