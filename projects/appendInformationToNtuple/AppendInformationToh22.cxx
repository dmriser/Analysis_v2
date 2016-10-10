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
using std::flush; 

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

  TFile * newNtuple = TFile::Open("modifiedFile.root","recreate");
  TChain * newChain = (TChain*) fchain->CloneTree(0);
  TTree * newTree   = newChain->GetTree();

  cout << "Looping on " << GetEntries() << " events." << endl;

  // Event loop below. 
  for(int ievent=0; ievent<GetEntries(); ievent++){
    GetEntry(ievent); 
    
    int jevent = event.evntid-1;

    // Calculate true
    int true_pid = genEvents[jevent].pid; 
    double true_mcp = sqrt( pow(genEvents[jevent].px,2) + pow(genEvents[jevent].py,2) + pow(genEvents[jevent].pz,2) );
    double true_cx = genEvents[jevent].px/true_mcp;
    double true_cy = genEvents[jevent].py/true_mcp;
    double true_cz = genEvents[jevent].pz/true_mcp; 
    double true_mctheta = acos(true_cz)*to_degrees;
    double true_mcphi = atan(true_cy/true_cx)*to_degrees; 

    event.mcpid[0] = (Float_t) true_pid; 
    event.mcp[0] = (Float_t) true_mcp; 
    event.mcphi[0] = (Float_t) true_mcphi; 
    event.mctheta[0] = (Float_t) true_mctheta; 

    cout << "deltaP/P=" << (true_mcp-event.p[0])/event.p[0] << endl; 
    
    newTree->Fill();
  }
   
  newTree->AutoSave();
  newNtuple->Close(); 
  
  cout << "Done loop."  << endl; 

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

  cout << "Done loading gen events " << theseEvents.size() << endl; 
  return theseEvents; 
}
#endif
