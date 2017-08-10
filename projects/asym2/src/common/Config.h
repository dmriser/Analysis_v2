#ifndef config_h 
#define config_h 

// standard library includes 
#include <map>
#include <string>
#include <vector>

// bins from h22
#include "DBins.h"

// configuration structure 
struct Config {
  std::string name, outputPath, fileList, parameterFile; 
  int filesToProcess; 

  std::map<std::string, std::map<std::string, double>> cuts; 
  std::map<std::string, std::vector<double>>           bins; 

  std::map<std::string, DLineBins>                     axes; 
};

#endif 
