#ifndef SKIMMER_CXX 
#define SKIMMER_CXX 

// c++ standard libraries 
#include <iostream>

// cern root 
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"

// this project 
#include "Skimmer.h"

Skimmer::Skimmer(TChain *chainToCopy){
  fChain = (TChain*) chainToCopy->CloneTree(0); 
  fTree  = fChain->GetTree(); 
}

void Skimmer::Fill(){
  fTree->Fill(); 
}

void Skimmer::Save(std::string outputFilename){
  TFile *outputFile = TFile::Open(outputFilename.c_str(), "recreate"); 
  fTree->Write();
  outputFile->Save(); 
}

#endif
