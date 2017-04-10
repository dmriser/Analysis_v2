
#ifndef mc_loader_h
#define mc_loader_h

// c++ includes 
#include <iostream>
#include <vector>
#include <cmath>
using namespace std; 

// my includes 
#include "BaseDISHistograms.h"
#include "h22Reader.h"
#include "HistogramLoader.h"
#include "NathanArchive.h"
#include "Parameters.h"
#include "ParticleFilter.h"
#include "PhysicsEventBuilder.h"
#include "PhysicsEventSelector.h"
#include "CommonTools.h"
#include "PhysicsEvent.h"

// root 
#include "TLorentzVector.h"
#include "TH2.h"

class MCLoader : public HistogramLoader{
 public:
  MCLoader(PhysicsEventSelector *eventCriteria, Parameters *pars, std::string outputFile, std::string saveOpts, std::string monteCarloType);
  ~MCLoader();

 protected:
  BaseDISHistograms recEvents;
  BaseDISHistograms genEvents;
  BaseDISHistograms recAndGenEvents;
  ParticleFilter    *filter; 
  std::string       mcType; 
  
 public:
  void Initialize();
  void ProcessEvent();
  void Save();
  bool EventHasGeneratedElectron();
};

MCLoader::MCLoader(PhysicsEventSelector *eventCriteria, Parameters *pars, std::string outputFile, std::string saveOpts, std::string monteCarloType) : HistogramLoader(eventCriteria, outputFile, saveOpts){
  mcType = monteCarloType; 
  filter = new ParticleFilter(pars);
}

MCLoader::~MCLoader(){
}

void MCLoader::Initialize(){
  recEvents.Init(Form("recEvents%s",mcType.c_str()),"Reconstructed MC Hits");
  genEvents.Init(Form("genEvents%s",mcType.c_str()),"Generated MC Events");
  recAndGenEvents.Init(Form("recAndGenEvents%s",mcType.c_str()),"Rec/Gen Same Bin Events");
}

void MCLoader::ProcessEvent(){

  if (EventHasGeneratedElectron()){
    TLorentzVector genElectron   = event.GetGeneratedParticle(11);   
    PhysicsEvent genPhysicsEvent = builder.getPhysicsEvent(genElectron); 
    int mcSector                 = event.GetMCSectorByPID(11);
    
    if (mcSector > -1 && mcSector < 7) { 
      genEvents.Fill(genPhysicsEvent, mcSector); 
    }
  }

  int e_index = filter->getByPID(event, 11);
  if (e_index > -123){

    TLorentzVector recElectron   = event.GetTLorentzVector(e_index, 11);
    int sector                   = event.dc_sect[e_index]; 
    PhysicsEvent recPhysicsEvent = builder.getPhysicsEvent(recElectron);

    if (eventSelector->passes(recPhysicsEvent) && sector > 0) {
      recEvents.Fill(recPhysicsEvent, sector);
    }
  } 
}

bool MCLoader::EventHasGeneratedElectron(){
  for(int ipart=0; ipart<event.gpart; ipart++){
    if (event.q[ipart] == -1 && event.mcid[ipart] == 11){
      return true; 
    }
  }
  return false; 
}

void MCLoader::Save(){
  // You may want to recreate the file, but dont overwrite it by
  // doing "RECREATE" on the second 2 saves. 
  recEvents      .Save(outputFilenameWithExtension, saveOption);
  genEvents      .Save(outputFilenameWithExtension, "UPDATE");   
  recAndGenEvents.Save(outputFilenameWithExtension, "UPDATE");
}

#endif
