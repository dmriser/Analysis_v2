#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <string>
#include <map>

#include "ParameterMetaData.h"
#include "ParameterSet.h"

class Parameters{
 public:
  Parameters();
  ~Parameters();

 protected:
  ParameterMetaData metaData; 
  map<std::string, ParameterSet*> parameters; 

 public:
  ParameterSet *getParameter(std::string name);
  ParameterMetaData getMetaData(){ return metaData; }

  void loadParameters(std::string inputFilename);
  void saveParameters(std::string outputFilename);
};

#endif
