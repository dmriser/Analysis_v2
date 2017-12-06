#ifndef PARAMETER_META_DATA_CXX
#define PARAMETER_META_DATA_CXX

#include <fstream>
#include <string>
#include <iostream>
using std::cout; 
using std::endl;
using std::string; 

// My 
#include "ParameterMetaData.h"

// Root 
#include "TDatime.h"

ParameterMetaData::ParameterMetaData(){
  time = new TDatime(); 
}

ParameterMetaData::~ParameterMetaData(){
}

void ParameterMetaData::updateTimeOfLastEdit(){
  timeOfLastEdit = (string) (time->AsString());
}

void ParameterMetaData::print(){
  cout << "# [ParameterMetaData] Entry for file " << nameOfFile << endl; 
  cout << timeOfLastEdit << endl; 
  cout << "# Number of parameter sets " << numberOfParameterSets << endl; 
  cout << "# Number of files used " << numberOfFilesUsed << endl; 
}

#endif
