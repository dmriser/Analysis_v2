#ifndef findElectronIDParameters_h
#define findElectronIDParameters_h


 // Put your includes here 
#include "DBins.h" 
#include "DCut.h" 
#include "DEvent.h" 
#include "DSelection.h" 
#include "h22Event.h" 
#include "h22Reader.h" 
#include "Parameters.h"
#include "ParameterSet.h"

#include "histos.h"
#include "histos.cxx"

class findElectronIDParameters : public h22Reader {
 public:
  findElectronIDParameters();
  ~findElectronIDParameters();
  
  Histograms histos;  
  Parameters eidParameters; 
  
  void Loop();
  void ProcessEvent();
  void WriteHardCodedParameters();
  void CalculateSamplingFractionCut();

};
#endif
