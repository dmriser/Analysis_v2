#include <iostream>

#include "common/Config.h"
#include "common/ConfigLoader.h"
#include "common/ConfigLoader.cxx"

// h22 libs 
#include "CommonTools.h"
#include "Corrections.h"
#include "DBins.h"
#include "h22Event.h"
#include "h22Reader.h"
#include "MomCorr.h"
#include "ParticleFilter.h"
#include "Parameters.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"
#include "StatusBar.h"

// root 
#include "TStopwatch.h"

struct configPack{
  Config          conf;
  Parameters     *pars; 
  ParticleFilter *filter;
};

class Analysis : public h22Reader {
public:  
  Analysis(std::vector<Config> confs) {

    for(int iconf=0; iconf<confs.size(); iconf++){
      configPack currentPack; 
      currentPack.conf = confs[iconf]; 
      currentPack.pars = new Parameters(); 
      currentPack.pars->loadParameters(currentPack.conf.parameterFile);      
      currentPack.filter = new ParticleFilter(currentPack.pars);
      packs.push_back(currentPack);
    }

    Init();
  }

  ~Analysis(){
    for(configPack p : packs){
      std::cout << p.conf.name << std::endl; 
      p.filter->GetSelector(11)->PrintSummary();
    }
  }

  void Loop(){
    StatusBar stat; 

    TStopwatch timer; 
    timer.Reset();
    timer.Start();

    // for every event
    for(int ientry=0; ientry<GetEntries(); ientry++){
      GetEntry(ientry);
      
      // process each configuration 
      for (configPack pack : packs){

	std::vector<int> electronIndices = pack.filter->getVectorOfParticleIndices(event, 11);
	if(!electronIndices.empty()){

	}

      }
      

      if (ientry%10000 == 0){
	stat.PrintStatus(ientry, GetEntries()); 
      }
    }

    std::cout << std::endl;
    double loopTime  = timer.RealTime(); 
    double eventRate = GetEntries()/loopTime; 
    std::cout << "[GenericAnalysis::Loop] Total time: " 
	      << loopTime << " seconds, Event rate: " 
	      << eventRate << " events/sec. " << std::endl;

  }

protected:
  std::vector<configPack> packs; 

};

int main(int argc, char *argv[]){

  if (argc > 1){

    // get configurations 
    std::vector<Config> configs; 
    for(int iarg=1; iarg<argc; iarg++){
      std::string file(argv[iarg]);
      configs.push_back(ConfigLoader::getConfiguration(file)); 
    }

    // setup analysis 
    Analysis analysis(configs); 

    // use the first configuration list of files 
    // and load the analyzer 
    std::vector<std::string> files = loadFilesFromList(configs[0].fileList, configs[0].filesToProcess);
    for(std::string f : files){
      analysis.AddFile(f); 
      std::cout << Form("Loaded file %s ",f.c_str()) << std::endl; 
    }
    std::cout << std::endl; 

    // run analysis loop
    analysis.Loop();

  } else {
    std::cerr << "No configurations found." << std::endl; 
  }

  return 0; 
}
