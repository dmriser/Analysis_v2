
// c++ includes 
#include <iostream>
#include <vector>

// my includes 
#include "BaseDISHistograms.h"
#include "h22Reader.h"
#include "PhysicsEventBuilder.h"
#include "PhysicsEventSelector.h"

#ifndef histogram_loader_h
#define histogram_loader_h

class HistogramLoader : public h22Reader{
 public:
  HistogramLoader(PhysicsEventSelector *eventCriteria, std::string outputFile, std::string saveOpts);
  ~HistogramLoader();

 protected:
  // Put some histograms here
  std::string outputFilenameWithExtension; 
  std::string saveOption; 
  PhysicsEventSelector *eventSelector; 
  PhysicsEventBuilder builder; 
  
 public:
  void Execute();

 protected:
  virtual void Initialize();
  virtual void Loop();
  virtual void ProcessEvent(); /** We don't need to pass in the event because h22Reader has access to the events already. */
  virtual void Save();

};

#endif
