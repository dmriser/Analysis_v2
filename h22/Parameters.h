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
  std::map<std::string, ParameterSet> parameters; 

 public:
  ParameterSet getParameter(std::string name);
  ParameterMetaData getMetaData(){ return metaData; }
  std::vector<std::string> parseLine(std::string);

  void addParameterSet(ParameterSet inputParameterSet){ parameters[inputParameterSet.getName()] = inputParameterSet; } 
  void loadParameters(std::string inputFilename);
  void saveParameters(std::string outputFilename);
  void setNumberOfFiles(int numberOfFiles){ metaData.setNumberOfFilesUsed(numberOfFiles); } 

};

#endif
