#ifndef config_loader_cxx 
#define config_loader_cxx 

// standard libs 
#include <iostream>
#include <fstream>
#include <map>
#include <vector>

// external non-standard
#include "json.hpp"
using namespace nlohmann; 

// this project 
#include "Config.h"
#include "ConfigLoader.h"

Config* ConfigLoader::getConfiguration(std::string jsonFile){
  Config *conf = new Config(); 
  
  std::ifstream inputFile(jsonFile.c_str());

  // create a local json object and
  // insert the input file contents.
  json jsonObject; 

  if (inputFile.is_open()){
    inputFile >> jsonObject; 

    // set simple string and integer options from the config 
    conf->name           = jsonObject.at("system").at("config_name").get<std::string>();
    conf->outputPath     = jsonObject.at("system").at("output").get<std::string>();
    conf->fileList       = jsonObject.at("system").at("file_list").get<std::string>();
    conf->filesToProcess = jsonObject.at("system").at("n_files").get<int>(); 
    
    // more complicated objects 
    conf->bins = jsonObject.at("bins").get<std::map<std::string, std::vector<float>>>(); 
    conf->cuts = jsonObject.at("cuts").get<std::map<std::string, std::map<std::string, float>>>();

  } else {
    std::cerr << "[ConfigLoader::getConfiguration] Problem loading configuration file: " << 
      jsonFile << std::endl; 
  }

  return conf; 
}

#endif 
