#ifndef lund_reader_cxx
#define lund_reader_cxx

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
using namespace std; 

#include "LundEvent.h"
#include "LundReader.h"

LundReader::LundReader(){
  currentEvent = 0; 
}

LundReader::~LundReader(){

}

LundReader::LundReader(std::string inputFilename){
  currentEvent = 0; 
  LoadEvents(inputFilename);
}

  
void LundReader::LoadEvents(std::string inputFilename){
  ifstream file(inputFilename.c_str());

  string currentLine;
  while(getline(file, currentLine)){
    
    // Do we have a header, or an event? 
    if (isHeader(currentLine)){
      vector<string> headerTokens = parseLine(currentLine);
      LundEvent thisEvent; 
      LundHeader thisHeader;

      thisHeader.numberOfParticles      = atoi(headerTokens[0].c_str());
      thisHeader.numberOfTargetNucleons = atoi(headerTokens[1].c_str());
      thisHeader.numberOfTargetProtons  = atoi(headerTokens[2].c_str());
      thisHeader.beamPolarization       = atof(headerTokens[3].c_str());
      thisHeader.targetPolarization     = atof(headerTokens[4].c_str());
      thisHeader.x  = atof(headerTokens[5].c_str());
      thisHeader.y  = atof(headerTokens[6].c_str());
      thisHeader.w  = atof(headerTokens[7].c_str());
      thisHeader.qq = atof(headerTokens[8].c_str());
      thisHeader.nu = atof(headerTokens[9].c_str()); 
	
      thisEvent.SetHeader(thisHeader);
							    
      for (int particle=0; particle< thisHeader.numberOfParticles; particle++){
	getline(file, currentLine);

	LundParticle thisParticle; 
	vector<string> particleTokens = parseLine(currentLine);

	thisParticle.index = atoi(particleTokens[0].c_str());
	thisParticle.charge = atoi(particleTokens[1].c_str());
	thisParticle.type = atoi(particleTokens[2].c_str());
	thisParticle.PID = atoi(particleTokens[3].c_str());
	thisParticle.parentIndex = atoi(particleTokens[4].c_str());
	thisParticle.daughterIndex = atoi(particleTokens[5].c_str());
	thisParticle.px = atof(particleTokens[6].c_str());
	thisParticle.py = atof(particleTokens[7].c_str());
	thisParticle.pz = atof(particleTokens[8].c_str());
	thisParticle.energy = atof(particleTokens[9].c_str());
	thisParticle.mass = atof(particleTokens[10].c_str());
	thisParticle.vx = atof(particleTokens[11].c_str());
	thisParticle.vy = atof(particleTokens[12].c_str());
	thisParticle.vz = atof(particleTokens[13].c_str());

	thisEvent.AddParticle(thisParticle);
      }
      AddEvent(thisEvent);
      
    }
    
    else {
      cout << "[LundReader] Error reading input file " << inputFilename << " improper formatting line length=" << parseLine(currentLine).size() << endl; 
    }

  }
  
  file.close();
}

bool LundReader::isHeader(string lineOfInterest){
  int numberOfTokens = parseLine(lineOfInterest).size(); 
  if (numberOfTokens == 10){ return true; }
  return false;
}

bool LundReader::isParticle(string lineOfInterest){
  int numberOfTokens = parseLine(lineOfInterest).size(); 
  if (numberOfTokens == 14){ return true; }
  return false; 
}

// Just splits line into entries 
vector<string> LundReader::parseLine(string line){

  vector<string> tokenizedLine;
  string buffer; 
  
  istringstream tokens(line);
  while(!tokens.eof())
    {
      string buffer;
      tokens >> buffer;
      tokenizedLine.push_back(buffer);
    }

  return tokenizedLine; 
}

#endif 
