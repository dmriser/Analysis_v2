#ifndef AppendInformationToh22_cxx
#define AppendInformationToh22_cxx

#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::vector;
using std::string; 

#include "AppendInformationToh22.h"

 // Put your includes here
#include "CommonTools.h"
#include "DBins.h" 
#include "DCut.h" 
#include "DEvent.h" 
#include "DSelection.h" 
#include "h22Event.h" 
#include "h22Reader.h" 

// cern root
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"

 // Class Constructor/Destructor 
AppendInformationToh22::AppendInformationToh22(){ 
}

AppendInformationToh22::~AppendInformationToh22(){ 
}

void AppendInformationToh22::Loop(){

  GeneratedEvents genEvents = LoadEventFile("events.gen"); 

  cout << "Detected type:" << GSIM << endl; 
  
  TFile * newNtuple = TFile::Open("modifiedFile.root","recreate");
  TChain * newChain = (TChain*) fchain->CloneTree(0);
  TTree * newTree   = newChain->GetTree();
  


  cout << "Looping on " << GetEntries() << " events. " << endl;
  // Event loop below. 
  for(int ievent=0; ievent<GetEntries(); ievent++){
    GetEntry(ievent); 
    
    // Calculate true
    int true_pid = genEvents[ievent].pid; 
    double true_mcp = sqrt( pow(genEvents[ievent].px,2) + pow(genEvents[ievent].py,2) + pow(genEvents[ievent].pz,2) );
    double true_cx = genEvents[ievent].px/true_mcp;
    double true_cy = genEvents[ievent].py/true_mcp;
    double true_cz = genEvents[ievent].pz/true_mcp; 
    double true_mctheta = acos(true_cz)*to_degrees;
    double true_mcphi = atan(true_cy/true_cx)*to_degrees; 
    
    newTree->Fill();
  }
  
  newTree->AutoSave();
  newNtuple->Close(); 

}

void AppendInformationToh22::SaveNewFile(string outputFilename){
  TFile * outputFile = TFile::Open(outputFilename.c_str(), "RECREATE");

  fchain->Write();
  outputFile->Write();
  outputFile->Close(); 
}

GeneratedEvents AppendInformationToh22::LoadEventFile(string inputFile){

  ifstream eventFile(inputFile.c_str());

  string line;
  GeneratedEvents theseEvents; 
  
  while(getline(eventFile, line)){
    TrueInformation thisEventInfo;
    eventFile >> thisEventInfo.pid >> thisEventInfo.px >> thisEventInfo.py >> thisEventInfo.pz;
    theseEvents.push_back(thisEventInfo); 
  }
  
  eventFile.close();

  cout << " Done loading gen events " << theseEvents.size() << endl; 
  return theseEvents; 
}
#endif
