// Loads Faraday Cup Information to TFile

// c++ includes 
#include <iostream>
#include <vector>

// my includes 
#include "DInformation.h"

// cern root libs
#include "TFile.h"
#include "TH1.h"

class FaradayCupAppender{
 public:
  FaradayCupAppender(DInformation * runInfo);
  ~FaradayCupAppender();

 protected:
  DInformation * runInfoDatabase; 
  double totalCharge; 
  TH1D * totalChargeHistogram; 
  
 public:
  void AddFiles(std::vector<std::string> dataFiles); 
  void AppendToFile(std::string outputFilenameWithExtension); 
  
};
