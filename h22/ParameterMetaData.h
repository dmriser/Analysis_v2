#ifndef PARAMETER_META_DATA_H
#define PARAMETER_META_DATA_H

#include <fstream>
#include <string>

class ParameterMetaData{
 public:
  ParameterMetaData();
  ~ParameterMetaData();

 protected:
  int numberOfParameterSets;
  int numberOfFilesUsed; 
  std::string timeOfLastEdit; 
  std::string nameOfFile;

 public:
  void setNumberOfParameterSets(int numberOfSets){ numberOfParameterSets = numberOfSets; }
  void setNumberOfFilesUsed(int numberOfFiles){ numberOfFilesUsed = numberOfFiles; }
  void setNameOfFile(std::string name){ nameOfFile = name; }
  void updateTimeOfLastEdit();

  int getNumberOfParameterSets(){ return numberOfParameterSets; }
  int getNumberOfFilesUsed(){ return numberOfFilesUsed; }
  std::string getName(){ return nameOfFile; }
  std::string getTimeOfLastEdit(){ return timeOfLastEdit; }

  void print();
};

#endif
