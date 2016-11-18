#ifndef mc_loader_h
#define mc_loader_h

// c++ includes 
#include <iostream>
#include <vector>
using namespace std;

// my includes 
//#include "BaseHistograms.h"
#include "CommonTools.h"
#include "h22Reader.h"
#include "HistogramLoader.h"
#include "NathanArchive.h"
#include "MomCorr.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"
#include "PhysicsEventSelector.h"

// root 
#include "TFile.h"
#include "TLorentzVector.h"
#include "THnSparse.h"

class MCLoader : public HistogramLoader{
 public:
  MCLoader(PhysicsEventSelector *eventCriteria, std::string outputFile, std::string saveOpts);
  ~MCLoader();

 protected:
  //  BaseHistograms mcEvents;
  const static int dimension = 4;
  
  THnSparseI *recEvents; 
  THnSparseI *genEvents; 

  int bins[dimension];
  double min[dimension];
  double max[dimension];

  NathanEIDWrapper eID; 
  string outputFilename; 

 protected:
  void Initialize();
  void ProcessEvent();
  void Save();

};

MCLoader::MCLoader(PhysicsEventSelector *eventCriteria, std::string outputFile, std::string saveOpts) : HistogramLoader(eventCriteria, outputFile, saveOpts){
  outputFilename = outputFile;
  // 4-Dimensional Binning 
  // 0 - Sector 
  // 1 - Momentum 
  // 2 - Theta 
  // 3 - Relative Phi
  bins[0] = 6;
  bins[1] = 100;
  bins[2] = 100;
  bins[3] = 100;

  min[0] = 1.0;      max[0] = 7.0; 
  min[1] = 2.05;     max[1] = 5.0; 
  min[2] = 18.0;     max[2] = 50.0; 
  min[3] = -20.0;    max[3] = 20.0; 

  recEvents = new THnSparseI("recEvents","recEvents",dimension,bins,min,max);
  genEvents = new THnSparseI("genEvents","genEvents",dimension,bins,min,max);
}

MCLoader::~MCLoader(){
  // Anything to do here? 
}

void MCLoader::Initialize(){

}

// This is the core routine which conditionally fills histograms. 
void MCLoader::ProcessEvent(){

  TLorentzVector genElectron = event.gen_particle(11);
  PhysicsEvent genPhysicsEvent = builder.getPhysicsEvent(genElectron);

  int mcSector = event.mcSectorByPID(11);
  if (mcSector > -1 && mcSector < 7) { 
    double genPoint[dimension] = {(double)mcSector,genElectron.E(),genElectron.Theta()*to_degrees,genElectron.Phi()*to_degrees};
    genEvents->Fill(genPoint);
  }

  eID.set_info(runno(),GSIM);
  int e_index = eID.get_electron(event);
  if (e_index > -123){
    TLorentzVector electron(event.cx[e_index]*event.p[e_index],
			    event.cy[e_index]*event.p[e_index],
			    event.cz[e_index]*event.p[e_index],
			    event.p[e_index]);

    int sector = event.dc_sect[e_index]; 
    PhysicsEvent physicsEvent = builder.getPhysicsEvent(electron);
    
    if (eventSelector->passes(physicsEvent) && sector > 0) {
      double mcPoint[dimension] = {(double)sector,electron.E(),electron.Theta()*to_degrees,electron.Phi()*to_degrees};
      recEvents->Fill(mcPoint);
    }
  } 
}

void MCLoader::Save(){
  TFile *outputFile = new TFile(outputFilename.c_str(), "UPDATE");
  recEvents->Write();
  genEvents->Write();
  outputFile->Write();
  outputFile->Close();
}

#endif
