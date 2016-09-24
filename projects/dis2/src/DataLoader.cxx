
// c++ includes 
#include <iostream>
#include <vector>

// my includes 
#include "BaseDISHistograms.h"
#include "DataLoader.h"
#include "h22Reader.h"
#include "HistogramLoader.h"
#include "NathanArchive.h"
#include "MomCorr.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"
#include "PhysicsEventSelector.h"

// root 
#include "TLorentzVector.h"

#ifndef data_loader_cxx
#define data_loader_cxx

DataLoader::DataLoader(PhysicsEventSelector *eventCriteria, MomCorr_e1f * momCorr, std::string outputFile, std::string saveOpts) : HistogramLoader(eventCriteria, outputFile, saveOpts){
  momCorrector = momCorr; 
}

DataLoader::~DataLoader(){
  // Anything to do here? 
}

void DataLoader::Initialize(){
  dataEvents.Init("dataEvents","Data Hits");
}

// This is the core routine which conditionally fills histograms. 
void DataLoader::ProcessEvent(){

  eID.set_info(runno(),GSIM);
  int e_index = eID.get_electron(event);
  if (e_index > -123){
    TLorentzVector electron(event.cx[e_index]*event.p[e_index],
			    event.cy[e_index]*event.p[e_index],
			    event.cz[e_index]*event.p[e_index],
			    event.p[e_index]);

    electron = momCorrector->PcorN(electron, -1, 11); 
    // TODO: Add momentum corrections here.  Think about setting path to momemtum correction file, or relying on external environment to provide us with
    // a working momentum correction utility (probably the better solution).
    
    int sector = event.dc_sect[e_index]; 
    PhysicsEvent physicsEvent = builder.getPhysicsEvent(electron);
    
    if (eventSelector->passes(physicsEvent) && sector > 0) {
      dataEvents.Fill(physicsEvent, sector); 
    }
  } 
}

void DataLoader::Save(){
  dataEvents.Save(outputFilenameWithExtension, saveOption);
}

#endif
