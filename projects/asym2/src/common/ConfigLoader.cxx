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


// from h22 libs 
#include "DBins.h"

Config ConfigLoader::getConfiguration(std::string jsonFile){
  Config conf; 
  
  std::ifstream inputFile(jsonFile.c_str());

  // create a local json object and
  // insert the input file contents.
  json jsonObject; 

  if (inputFile.is_open()){
    inputFile >> jsonObject; 

    // set simple string and integer options from the config 
    conf.name           = jsonObject.at("system").at("config_name").get<std::string>();
    conf.outputPath     = jsonObject.at("system").at("output").get<std::string>();
    conf.fileList       = jsonObject.at("system").at("file_list").get<std::string>();
    conf.parameterFile  = jsonObject.at("system").at("parameter_file").get<std::string>();
    conf.filesToProcess = jsonObject.at("system").at("n_files").get<int>(); 
    
    // more complicated objects 
    conf.cuts = jsonObject.at("cuts").get<std::map<std::string, std::map<std::string, double>>>();
    conf.bins = jsonObject.at("bins").get<std::map<std::string, std::vector<double>>>(); 

    // load the binned axes 
    for(std::pair<std::string, std::vector<double>> b : conf.bins){
      std::vector<double> limits = b.second; 
      conf.axes[b.first] = DLineBins(limits.size()-1, limits);
    }

    inputFile.close(); 

  } else {
    std::cerr << "[ConfigLoader::getConfiguration] Problem loading configuration file: " 
	      << jsonFile << std::endl; 
  }

  return conf; 
}

#endif 
