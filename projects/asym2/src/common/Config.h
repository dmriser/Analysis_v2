#ifndef config_h 
#define config_h 

// standard library includes 
#include <map>
#include <string>
#include <vector>

// configuration structure 
struct Config {
  std::string name, outputPath, fileList; 
  int filesToProcess; 

  std::map<std::string, std::map<std::string, float>> cuts; 
  std::map<std::string, std::vector<float>>          bins; 
  
};

#endif 
