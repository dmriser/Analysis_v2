#ifndef PARAMETERS_CXX
#define PARAMETERS_CXX

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <cstdlib>
using namespace std; 

#include "Parameters.h"
#include "ParameterMetaData.h"
#include "ParameterSet.h"

Parameters::Parameters(){
}

Parameters::~Parameters(){
}

ParameterSet Parameters::getParameter(string name){
  if (parameters.find(name) != parameters.end()){
    return parameters[name];
  }
  else {
    cout << "[Parameters::getParameter] Returning null parameter set for not finding key " << name << " check your spelling/file. " << endl; 
  }
  
  return ParameterSet(); 
}

std::vector<std::string> Parameters::getListOfParameters(){
  std::vector<std::string> names; 
  
  for(std::pair<std::string, ParameterSet> p : parameters){
    names.push_back(p.first); 
  }

  return names; 
}

void Parameters::loadParameters(string inputFilename){
  metaData.setNameOfFile(inputFilename);

  ifstream file(inputFilename.c_str());

  string currentLine;
  while(getline(file, currentLine)){

    // Trying to skip whitespace lines
    if (currentLine.size() > 10 && currentLine[0] != '#'){
      ParameterSet thisSet;
      
      vector<string> line = parseLine(currentLine);
      getline(file, currentLine);
      vector<string> nextLine = parseLine(currentLine);
      
      thisSet.setName(line[0]);
      int numberOfValues = line.size()-3;
      int numberOfErrors = nextLine.size()-3;
      
      for (int i=0; i<numberOfValues; i++)    { thisSet.addValue( atof(line[i+3].c_str()) ); } // Setting values 
      for (int i=0; i<numberOfErrors; i++){ thisSet.addError( atof(nextLine[i+3].c_str()) ); } // Setting errors 
      
      parameters[thisSet.getName()] = thisSet; 
    }
  }

  file.close();
}

void Parameters::saveParameters(string outputFilename){
  ofstream file(outputFilename.c_str(), std::ios::trunc);

  metaData.setNameOfFile(outputFilename);
  metaData.updateTimeOfLastEdit(); 

  file << "# Filename: " << metaData.getName() << endl;
  file << "# Last edited at: " << metaData.getTimeOfLastEdit() << endl; 

  map<string, ParameterSet>::iterator iter; 
  for (iter = parameters.begin(); iter != parameters.end(); ++iter){
    ParameterSet currentParameterSet = iter->second;
    file.width(20); file << currentParameterSet.getName(); 
    file.width(6);  file << currentParameterSet.getNumberOfValues();
    file.width(6);  file << 'v';

    for (int param = 0; param < currentParameterSet.getNumberOfValues(); param++){ file.width(20); file << currentParameterSet.getValue(param); }
    file << endl;
    file.width(20); file << currentParameterSet.getName(); 
    file.width(6);  file << currentParameterSet.getNumberOfValues();
    file.width(6); file << 'e';

    for (int param = 0; param < currentParameterSet.getNumberOfValues(); param++){ file.width(20); file << currentParameterSet.getError(param); }
    file << endl;
    //    file << endl;
  }

  file.close();
}

// Just splits line into entries
vector<string> Parameters::parseLine(string line){

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
