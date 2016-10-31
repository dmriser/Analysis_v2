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
#include "LundReader.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"

// CERN Root Libs
#include "TLorentzVector.h"

class RadiativeCorrectionLoader : public LundReader{

 public:
  RadiativeCorrectionLoader(string histogramName, string histogramTitle, string outputName, string saveOpt);
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
    int sector = 1+floor((180.0 + electron.Phi()*to_degrees)/60.0);
    if (sector > 0 && sector < 7) { simEvents.Fill(physicsEvent, sector); }

    cout.width(12); cout << electron.X();
    cout.width(12); cout << electron.Y();
    cout.width(12); cout << electron.Z();
    cout.width(12); cout << electron.Theta() * to_degrees;
    cout.width(12); cout << electron.Phi() * to_degrees;
    cout.width(12); cout << physicsEvent.x; 
    cout.width(12); cout << physicsEvent.w; 
    cout.width(12); cout << physicsEvent.qq << endl; 
  }

}

void RadiativeCorrectionLoader::Save(){
  simEvents.Save(outputFilenameWithExtension, saveOption);
}

#endif 
