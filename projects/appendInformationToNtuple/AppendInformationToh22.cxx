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
#include "TFile.h"

 // Class Constructor/Destructor 
AppendInformationToh22::AppendInformationToh22(){ 
 }

AppendInformationToh22::~AppendInformationToh22(){ 
 }

void AppendInformationToh22::Loop(){

  GeneratedEvents genEvents = LoadEventFile("events.gen"); 
  
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

		event.mcp[0] = true_mcp;
		event.mcid[0] = true_pid;
		event.mcpid[0] = true_pid; 
		event.mctheta[0] = true_mctheta; 
		event.mcphi[0] = true_mcphi; 
	} 
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
    vector<string> data = split(line,' '); 
    thisEventInfo.pid = atoi(data[0].c_str());
    thisEventInfo.px  = atof(data[1].c_str());
    thisEventInfo.py  = atof(data[2].c_str());
    thisEventInfo.pz  = atof(data[3].c_str());
    theseEvents.push_back(thisEventInfo); 
  }
  
  eventFile.close();
  return theseEvents; 
}
#endif
