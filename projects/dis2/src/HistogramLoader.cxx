
// c++ includes 
#include <iostream>
#include <vector>
using namespace std;

// my includes 
#include "BaseDISHistograms.h"
#include "HistogramLoader.h"
#include "h22Reader.h"

#ifndef histogram_loader_cxx
#define histogram_loader_cxx

HistogramLoader::HistogramLoader(PhysicsEventSelector *eventCriteria, string outputFile, string saveOpts){

  eventSelector = eventCriteria; 
  outputFilenameWithExtension = outputFile;
  saveOption = saveOpts; 

}

HistogramLoader::~HistogramLoader(){

}

void HistogramLoader::Execute(){
  Initialize();
  Loop();
  Save();
}

void HistogramLoader::Save(){
  // Just save to the file here. 
  TFile * outputFile = TFile::Open(outputFilenameWithExtension.c_str(), saveOption.c_str());
  outputFile->Write();
  outputFile->Close();
}

void HistogramLoader::Initialize(){
  // Here you initialize histograms. 
}

void HistogramLoader::Loop(){

  // Assumes that file lists are loaded in h22Reader. 
  if (GSIM == -1) { Init(); }
  
  int numberOfEntries = fchain->GetEntries();

  for (int iEntry = 0; iEntry < numberOfEntries; iEntry++) {
    if (iEntry%1000 == 0){ cout << "\r done " << iEntry << flush; } 

    GetEntry(iEntry); // Tell the fchain where to point 
    ProcessEvent();    
  }
  cout << endl; 
  eventSelector->summarize();
}

void HistogramLoader::ProcessEvent(){
  event.printEvent();
}

#endif
