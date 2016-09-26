
// c++ includes 
#include <iostream>
#include <vector>

// my includes 
#include "BaseDISHistograms.h"
#include "h22Reader.h"
#include "HistogramLoader.h"
#include "NathanArchive.h"
#include "MomCorr.h"
#include "PhysicsEventBuilder.h"
#include "PhysicsEventSelector.h"

#ifndef data_loader_h
#define data_loader_h

class DataLoader : public HistogramLoader{
 public:
  DataLoader(PhysicsEventSelector *eventCriteria, MomCorr_e1f * momCorr, std::string outputFile, std::string saveOpts);
  ~DataLoader();

 protected:
  BaseDISHistograms dataEvents;
  NathanEIDWrapper eID; 
  MomCorr_e1f * momCorrector; 

 protected:
  void Initialize();
  void ProcessEvent();
  void Save();

};

#endif
