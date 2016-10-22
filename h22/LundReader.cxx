#ifndef lund_reader_cxx
#define lund_reader_cxx

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std; 

#include "CommonTools.h"
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
      cout << "[LundReader] New event detected " << endl;

    } else if (isParticle(currentLine)) {
      

    } else {
      cout << "[LundReader::LoadEvents] Error reading input file " << inputFilename << " improper formatting line length=" << parseLine(currentLine).size() << endl; 
    }
    

  }

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
