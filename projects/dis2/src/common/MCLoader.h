
// c++ includes 
#include <iostream>
#include <vector>

// my includes 
#include "BaseDISHistograms.h"
#include "h22Reader.h"
#include "HistogramLoader.h"
#include "NathanArchive.h"
#include "PhysicsEventBuilder.h"
#include "PhysicsEventSelector.h"

#ifndef mc_loader_h
#define mc_loader_h

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
  
 protected:
  void Initialize();
  void ProcessEvent();
  void Save();

};

#endif
