#ifndef PARAMETER_META_DATA_CXX
#define PARAMETER_META_DATA_CXX

#include <fstream>
#include <string>
#include <iostream>
using std::cout; 
using std::endl;

#include "ParameterMetaData.h"

ParameterMetaData::ParameterMetaData(){
}

ParameterMetaData::~ParameterMetaData(){
}

void ParameterMetaData::updateTimeOfLastEdit(){
  timeOfLastEdit = "# [ParameterMetaData::updateTimeOfLastEdit] Feature not supported yet!"; 
}

void ParameterMetaData::print(){
  cout << "# [ParameterMetaData] Entry for file " << nameOfFile << endl; 
  cout << timeOfLastEdit << endl; 
  cout << "# Number of parameter sets " << numberOfParameterSets << endl; 
  cout << "# Number of files used " << numberOfFilesUsed << endl; 
}

#endif
