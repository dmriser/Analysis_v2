#ifndef data_loader_h
#define data_loader_h

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
#include "Parameters.h"
#include "ParticleFilter.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"
#include "PhysicsEventSelector.h"

// root 
#include "TFile.h"
#include "TLorentzVector.h"
#include "THnSparse.h"

class DataLoader : public HistogramLoader{
 public:
  DataLoader(PhysicsEventSelector *eventCriteria, MomCorr_e1f * momCorr, Parameters *pars,std::string outputFile, std::string saveOpts);
  ~DataLoader();

 protected:
  const static int dimension     = 5;
  const static int numberSectors = 6;
  
  THnSparseI *dataEvents; 
  int bins[dimension];
  double min[dimension];
  double max[dimension];

  NathanEIDWrapper eID; 
  ParticleFilter  *filter;
  MomCorr_e1f     *momCorrector; 
  string           outputFilename; 

 protected:
  void Initialize();
  void ProcessEvent();
  void Save();

};

DataLoader::DataLoader(PhysicsEventSelector *eventCriteria, MomCorr_e1f *momCorr, Parameters *pars, std::string outputFile, std::string saveOpts) : HistogramLoader(eventCriteria, outputFile, saveOpts){
  momCorrector = momCorr; 
  outputFilename = outputFile;
  // 5-Dimensional Binning 
  // 0 - Sector
  // 1 - Momentum 
  // 2 - Theta 
  // 3 - Relative Phi
  // 4 - W 
  bins[0] = numberSectors;
  bins[1] = 100;
  bins[2] = 100;
  bins[3] = 100;
  bins[4] = 100;

  min[0] = 1.00;     max[0] = 7.0; 
  min[1] = 2.05;     max[1] = 5.0; 
  min[2] = 22.0;     max[2] = 48.0; 
  min[3] = -12.0;    max[3] = 12.0; 
  min[4] =  0.70;    max[4] = 1.20; 

  filter = new ParticleFilter(pars);
  //  filter->getSelector(11)->enable_by_regex("NPhe");

  dataEvents = new THnSparseI("dataEvents","dataEvents",dimension,bins,min,max);
}

DataLoader::~DataLoader(){
  filter->getSelector(11)->summarize(); 
}

void DataLoader::Initialize(){

}

void DataLoader::ProcessEvent(){

  vector<int> electrons = filter->getVectorOfParticleIndices(event, 11);
  if ( !electrons.empty() ){
    TLorentzVector electron   = event.getTLorentzVector(electrons[0],     11);
    electron                  = momCorrector->PcorN(electron, -1, 11); 
    int sector                = event.dc_sect[electrons[0]]; 
    PhysicsEvent physicsEvent = builder.getPhysicsEvent(electron);
    
    if (eventSelector->passes(physicsEvent) && sector > 0) {
      double dataPoint[dimension] = {(double) sector, electron.E(),electron.Theta()*to_degrees,event.rphi(electrons[0]),physicsEvent.w};
      dataEvents->Fill(dataPoint);
    }
  } 
}

void DataLoader::Save(){
  TFile *outputFile = new TFile(outputFilename.c_str(), "RECREATE");
  dataEvents->Write(); 
  outputFile->Write();
  outputFile->Close();
}

#endif
