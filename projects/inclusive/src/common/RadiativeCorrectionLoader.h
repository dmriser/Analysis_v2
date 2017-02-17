#ifndef rad_corr_h 
#define rad_corr_h 

// C++ libs 
#include <iostream>
#include <cmath>
using namespace std; 

// My Libs 
#include "BaseDISHistograms.h"
#include "CommonTools.h"
#include "KeppelRadReader.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"

// CERN Root Libs
#include "TLorentzVector.h"
#include "TRandom3.h"
#include "TMath.h"

class RadiativeCorrectionLoader : public KeppelRadReader {

 public:
  RadiativeCorrectionLoader(string histogramName, string histogramTitle, string outputName, string saveOpt);
  ~RadiativeCorrectionLoader();

 protected:
  BaseDISHistograms   simEvents; 
  PhysicsEventBuilder builder; 
  TRandom3            rand;
  string histoName, histoTitle, outputFilenameWithExtension, saveOption; 

  bool SectorIsValid(int sector);

 public:
  void Initialize();
  void ProcessEvent();
  void Save();
};

RadiativeCorrectionLoader::RadiativeCorrectionLoader(string histogramName, string histogramTitle, string outputName, string saveOpt){
  histoName                   = histogramName; 
  histoTitle                  = histogramTitle; 
  outputFilenameWithExtension = outputName; 
  saveOption                  = saveOpt;
}

RadiativeCorrectionLoader::~RadiativeCorrectionLoader(){
}

void RadiativeCorrectionLoader::Initialize(){
  simEvents.Init(histoName, histoTitle);
}

void RadiativeCorrectionLoader::ProcessEvent(){
  // I don't think electron phi is provided for us
  // so randomize it between -180.0 & 180.0.
  double phi = rand.Uniform(360.0)-180.0;
  double px  = ep*TMath::Sin(thete*to_radians)*TMath::Cos(phi*to_radians); 
  double py  = ep*TMath::Sin(thete*to_radians)*TMath::Sin(phi*to_radians); 
  double pz  = ep*TMath::Cos(thete*to_radians); 

  TLorentzVector electron(px, py, pz, ep); 
  PhysicsEvent physicsEvent = builder.getPhysicsEvent(electron);
  int sector                = 1+floor((180.0 + electron.Phi()*to_degrees)/60.0);

  if (SectorIsValid(sector)) { 
    simEvents.Fill(physicsEvent, sector); 
  }
}

bool RadiativeCorrectionLoader::SectorIsValid(int sector){
  return (sector > 0 && sector < 7);
}

void RadiativeCorrectionLoader::Save(){
  simEvents.Save(outputFilenameWithExtension, saveOption);
}

#endif 
