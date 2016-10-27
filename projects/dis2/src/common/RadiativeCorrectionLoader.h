#ifndef rad_corr_h 
#define rad_corr_h 

// C++ libs 
#include <iostream>
#include <cmath>
using namespace std; 

// My Libs 
#include "BaseDISHistograms.h"
#include "CommonTools.h"
#include "LundEvent.h"
#include "LundParticle.h"
#include "LundReader.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"

// CERN Root Libs
#include "TLorentzVector.h"

class RadiativeCorrectionLoader : public LundReader{

 public:
  RadiativeCorrectionLoader(string inputName, string histogramName, string histogramTitle);
  ~RadiativeCorrectionLoader();

 protected:
  BaseDISHistograms simEvents; 
  PhysicsEventBuilder builder; 
  string histoName, histoTitle, outputFilenameWithExtension, saveOption; 

 public:
  void Initialize();
  void Loop();
  void Save();
  void Execute();

};

RadiativeCorrectionLoader::RadiativeCorrectionLoader(string histogramName, string histogramTitle, string outputName, string saveOpt){
  histoName = histogramName; 
  histoTitle = histogramTitle; 
  outputFilenameWithExtension = outputName; 
  saveOption = saveOpt;
}

RadiativeCorrectionLoader::~RadiativeCorrectionLoader(){
  // Anything to do here? 
}

void RadiativeCorrectionLoader::Execute(){
  Initialize();
  Loop();
  Save();
}

void RadiativeCorrectionLoader::Initialize(){
  simEvents.Init(histoName, histoTitle);
}

void RadiativeCorrectionLoader::Loop(){

  // No complicated conditional fill conditions, because 
  // we expect generated events only. 
  while(HasEvent()){
    TLorentzVector electron = GetEvent().GetParticle(0).GetTLorentzVector(); 
    PhysicsEvent physicsEvent = builder.getPhysicsEvent(electron);
    int sector = 1+floor(electron.Phi()*to_degrees/60.0);
    simEvents->Fill(physicsEvent, sector);
  }

}

void RadiativeCorrectionLoader::Save(){
  simEvents.Save(outputFilenameWithExtension, saveOption);
}

#endif 
