#include <iostream>
#include <vector>

#include "DBins.h"

#include "CommonTools.h"
#include "StandardHistograms.h"

#include "common/Config.h"
#include "common/ConfigLoader.h"
#include "common/ConfigLoader.cxx"

#include "TH1.h"

int main(int argc, char *argv[]){

  
  if (argc < 2){
    std::cout << "Please pass in configuration files." << std::endl; 
    return 0;
  }
  
  std::vector<Config> configs; 
  for(int iarg=1; iarg<argc; iarg++){
    std::string configFile(argv[iarg]); 
    
    configs.push_back( ConfigLoader::getConfiguration(configFile) ); 
  }

  for(Config c : configs){
    StandardHistograms histos("PassAll", 1); 
    histos.Load(Form("%s/%s/histos.root",c.analysisHome.c_str(),c.name.c_str()));

    int bins  = Global::BinningTools::GetNumber((TH1F*) histos.h1_xbj[0]);
    float min = Global::BinningTools::GetQuantile((TH1F*) histos.h1_xbj[0], 0.01); 
    float max = Global::BinningTools::GetQuantile((TH1F*) histos.h1_xbj[0], 0.99); 

    std::vector<float> binLimits = Global::BinningTools::GetBins((TH1F*) histos.h1_xbj[0], 
								 ceil(bins/16.0), min, max, 0.3); 

    std::cout << "Number of bins = " << bins/16.0 << std::endl; 
    std::cout << "min = " << min << std::endl; 
    std::cout << "max = " << max << std::endl; 

    std::cout << "[";
    for(float f : binLimits){
      std::cout << f << ", "; 
    }
    std::cout << "]" << std::endl;
  }
 
  return 0;
}
