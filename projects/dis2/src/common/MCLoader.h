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
#include "PhysicsEventBuilder.h"
#include "PhysicsEventSelector.h"
#include "CommonTools.h"
#include "PhysicsEvent.h"

// root 
#include "TLorentzVector.h"
#include "TH2.h"

class MCLoader : public HistogramLoader{
 public:
  MCLoader(PhysicsEventSelector *eventCriteria, std::string outputFile, std::string saveOpts, std::string monteCarloType);
  ~MCLoader();

 protected:
  BaseDISHistograms recEvents;
  BaseDISHistograms genEvents;
  BaseDISHistograms recAndGenEvents;
  NathanEIDWrapper eID; 
  std::string mcType; 
  
 public:
  void Initialize();
  void ProcessEvent();
  void Save();
  void Rebin(int xFactor, int yFactor);

};

MCLoader::MCLoader(PhysicsEventSelector *eventCriteria, std::string outputFile, std::string saveOpts, std::string monteCarloType) : HistogramLoader(eventCriteria, outputFile, saveOpts){
  mcType = monteCarloType; 
}

MCLoader::~MCLoader(){
  // Anything to do here? 
}

void MCLoader::Initialize(){
  recEvents.Init(Form("recEvents%s",mcType.c_str()),"Reconstructed MC Hits");
  genEvents.Init(Form("genEvents%s",mcType.c_str()),"Generated MC Events");
  recAndGenEvents.Init(Form("recAndGenEvents%s",mcType.c_str()),"Rec/Gen Same Bin Events");
  //  recAndGenEvents.Rebin2D(2,2);
}

// This is the core routine which conditionally fills histograms. 
void MCLoader::ProcessEvent(){

  // Deal with the generated first.
  TLorentzVector genElectron = event.gen_particle(11);   
  PhysicsEvent genPhysicsEvent = builder.getPhysicsEvent(genElectron); 

  // Maybe here we should check that the track was reconstructed 
  // if (event.gpart > 0), but that would probably skew acceptance 
  // by stopping all particles which went through holes ect. so it 
  // doesn't seem like what we want to do. 
  int mcSector = event.mcSectorByPID(11);
  if (mcSector > -1 && mcSector < 7) { genEvents.Fill(genPhysicsEvent, mcSector); }

  // Dealing directly with histograms in base histograms here
  // is not ideal, think of a better way. 
  int genxByQQBin = recAndGenEvents.xByQQ[0]->FindBin(genPhysicsEvent.x, genPhysicsEvent.qq);
  int genwByQQBin = recAndGenEvents.wByQQ[0]->FindBin(genPhysicsEvent.w, genPhysicsEvent.qq);
  
  eID.set_info(runno(),GSIM);
  int e_index = eID.get_electron(event);
  if (e_index > -123){
    TLorentzVector recElectron(event.cx[e_index]*event.p[e_index],
			       event.cy[e_index]*event.p[e_index],
			       event.cz[e_index]*event.p[e_index],
			       event.p[e_index]);
    
    int sector = event.dc_sect[e_index]; 
    PhysicsEvent recPhysicsEvent = builder.getPhysicsEvent(recElectron);

    // Again doing this to fill coincident events recAndGen needed to calculate purity and
    // stability. 
    int recxByQQBin = recAndGenEvents.xByQQ[0]->FindBin(recPhysicsEvent.x, recPhysicsEvent.qq);
    int recwByQQBin = recAndGenEvents.wByQQ[0]->FindBin(recPhysicsEvent.w, recPhysicsEvent.qq);

    if (recxByQQBin == genxByQQBin){ recAndGenEvents.FillxByQQ(genPhysicsEvent.x, genPhysicsEvent.qq, sector); }
    if (recwByQQBin == genwByQQBin){ recAndGenEvents.FillwByQQ(genPhysicsEvent.w, genPhysicsEvent.qq, sector); } 
    
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

void MCLoader::Rebin(int xFactor, int yFactor){
  recEvents.Rebin2D(xFactor, yFactor);
  genEvents.Rebin2D(xFactor, yFactor);
  recAndGenEvents.Rebin2D(xFactor, yFactor);
}

#endif
