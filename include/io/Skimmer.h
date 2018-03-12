#ifndef SKIMMER_H 
#define SKIMMER_H 

// c++ standard libraries 
#include <iostream>

// cern root  
#include "TChain.h"
#include "TTree.h"

class Skimmer {

 public:
  Skimmer(TChain *chainToCopy);
  ~Skimmer();

  void Fill();
  void Save(std::string outputFilename);

 protected:

  // chain and tree for output 
  // ntuple 
  TChain *fChain; 
  TTree  *fTree; 

};

#endif 
