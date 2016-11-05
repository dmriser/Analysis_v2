#ifndef PARAMETER_META_DATA_CXX
#define PARAMETER_META_DATA_CXX

#include <fstream>
#include <string>
#include <iostream>
#include <time.h>
using std::cout; 
using std::endl;
using std::string; 

#include "ParameterMetaData.h"

ParameterMetaData::ParameterMetaData(){
}

ParameterMetaData::~ParameterMetaData(){
}

void ParameterMetaData::updateTimeOfLastEdit(){
  time_t rawtime;
  struct tm * timeinfo;
  time (&rawtime);
  timeinfo = localtime (&rawtime);

  timeOfLastEdit = (string) (asctime(timeinfo));
}

void ParameterMetaData::print(){
  cout << "# [ParameterMetaData] Entry for file " << nameOfFile << endl; 
  cout << timeOfLastEdit << endl; 
  cout << "# Number of parameter sets " << numberOfParameterSets << endl; 
  cout << "# Number of files used " << numberOfFilesUsed << endl; 
}

#endif
