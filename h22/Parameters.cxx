#ifndef PARAMETERS_CXX
#define PARAMETERS_CXX

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

#include "Parameters.h"
#include "ParameterMetaData.h"
#include "ParameterSet.h"

Parameters::Parameters(){
}

Parameters::~Parameters(){
}

ParameterSet * Parameters::getParameter(string name){

}

void Parameters::loadParameters(string inputFilename){
  ifstream file(inputFilename.c_str());

  file.close();
}

void Parameters::saveParameters(string outputFilename){
  ofstream file(outputFilename.c_str(), std::ios::trunc);

  map<string, ParameterSet*>::iterator iter; 
  for (iter = parameters.begin(); iter != parameters.end(); ++iter){
    // Do the save code here. 
  }

  file.close();
}

#endif
