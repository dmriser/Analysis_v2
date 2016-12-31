#ifndef data_loader_h
#define data_loader_h

// c++ includes 
#include <iostream>
#include <vector>
using namespace std;

// my includes 
#include "BaseDISHistograms.h"
#include "h22Reader.h"
#include "HistogramLoader.h"
#include "NathanArchive.h"
#include "MomCorr.h"
#include "Parameters.h"
#include "ParticleFilter.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"
#include "PhysicsEventSelector.h"

// root 
#include "TLorentzVector.h"

class DataLoader : public HistogramLoader{
 public:
  DataLoader(PhysicsEventSelector *eventCriteria, MomCorr_e1f * momCorr, Parameters *pars, std::string outputFile, std::string saveOpts);
  ~DataLoader();

 protected:
  BaseDISHistograms dataEvents;
  NathanEIDWrapper eID; 
  MomCorr_e1f *momCorrector; 
  ParticleFilter *filter; 

 protected:
  void Initialize();
  void ProcessEvent();
  void Save();

};

DataLoader::DataLoader(PhysicsEventSelector *eventCriteria, MomCorr_e1f * momCorr, Parameters *pars, std::string outputFile, std::string saveOpts) : HistogramLoader(eventCriteria, outputFile, saveOpts){
  momCorrector = momCorr; 
  filter       = new ParticleFilter(pars);
}

DataLoader::~DataLoader(){
  // Anything to do here? 
}

void DataLoader::Initialize(){
  dataEvents.Init("dataEvents","Data Hits");

  
  //  cout << "[DataLoader] Warning: Running with electron ID strictness alterations. " << endl;
  //  eID.cc_fid_strict = 2;
  //  eID.dc_r1_strict  = 2;
  //  eID.dc_r3_strict  = 2;

  filter->getSelector(11)->disable_by_name("DC Region 1 Fid Cut");
  filter->getSelector(11)->disable_by_name("DC Region 3 Fid Cut");
  filter->getSelector(11)->disable_by_name("EC-U Cut");
  filter->getSelector(11)->disable_by_name("EC-V Cut");
  filter->getSelector(11)->disable_by_name("EC-W Cut");

}

// This is the core routine which conditionally fills histograms. 
void DataLoader::ProcessEvent(){

  //  eID.set_info(runno(),GSIM);
  //  int e_index = eID.get_electron(event);

  filter->set_info(runno(), GSIM);
  int e_index = filter->getByPID(event, 11);
  if (e_index > -123){
    TLorentzVector electron(event.cx[e_index]*event.p[e_index],
			    event.cy[e_index]*event.p[e_index],
			    event.cz[e_index]*event.p[e_index],
			    event.p[e_index]);

    electron = momCorrector->PcorN(electron, -1, 11); 

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
