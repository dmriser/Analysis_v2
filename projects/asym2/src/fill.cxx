#include <iostream>

#include "common/Config.h"
#include "common/ConfigLoader.h"
#include "common/ConfigLoader.cxx"
#include "common/PhiHistograms.h"
#include "common/TreeOutput.h"
#include "common/Types.h"

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
#include "StandardHistograms.h"

// root 
#include "TFile.h"
#include "TLorentzVector.h"
#include "TStopwatch.h"

struct configPack{
  Config              conf;
  Parameters         *pars; 
  ParticleFilter     *filter;
  PhiHistos          *counts[2]; 

  TreeOutput *tree; 
  std::map<int, StandardHistograms*> standardHistos; 
};

class Analysis : public h22Reader {
public:  
  Analysis(std::vector<Config> confs) {

    std::string path        = Global::Environment::GetAnalysisPath(); 
    std::string momCorrPath = Form("%s/momCorr/",path.c_str());
    momCorr                 = new MomCorr_e1f(momCorrPath);

    for(int iconf=0; iconf<confs.size(); iconf++){
      configPack currentPack; 
      currentPack.conf = confs[iconf]; 
      currentPack.pars = new Parameters(); 
      currentPack.pars->loadParameters(currentPack.conf.parameterFile);      
      currentPack.filter = new ParticleFilter(currentPack.pars);

      // setting up histograms for different cut levels 
      // these types are defined in types.h
      currentPack.standardHistos[pass::mesonID] = new StandardHistograms("PassMesonID",0);
      currentPack.standardHistos[pass::SIDIS]   = new StandardHistograms("PassSIDIS"  ,0);
      currentPack.standardHistos[pass::all]     = new StandardHistograms("PassAll"    ,0);

      // counts in phi for different helicties 
      currentPack.counts[helicity::minus] = new PhiHistos(); 
      currentPack.counts[helicity::minus]->Init(currentPack.conf.axes, "countsMinus");

      currentPack.counts[helicity::plus] = new PhiHistos(); 
      currentPack.counts[helicity::plus]->Init(currentPack.conf.axes, "countsPlus");

      // output tree 
      currentPack.tree = new TreeOutput(); 

      packs.push_back(currentPack);
    }

    GSIM = false; 
    Init();
  }

  ~Analysis(){
    for(configPack p : packs){
      std::cout << p.conf.name << std::endl; 
      p.filter->GetSelector(11)->PrintSummary();
      p.filter->GetSelector(p.conf.mesonIndex)->PrintSummary();
    }
  }

  void Loop(){

    // setup filters with current info after 
    // files ahve been added 
    for (configPack p : packs){
      p.filter->set_info(GSIM, GetRunNumber());
    }

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
	  
	  int electronIndex = electronIndices[0];
	  event.SetElectronIndex(electronIndex);
	  Corrections::correctEvent(&event, GetRunNumber(), GSIM);

	  std::vector<int> mesonIndices = pack.filter->getVectorOfParticleIndices(event, pack.conf.mesonIndex);
	  if (!mesonIndices.empty()){

	    // build an event 
	    int mesonIndex = mesonIndices[0];
	    TLorentzVector electron = event.GetTLorentzVector(electronIndex, 11);  
	    TLorentzVector meson = event.GetTLorentzVector(mesonIndex, pack.conf.mesonIndex);

	    electron = momCorr->PcorN(electron, -1, 11);
	    PhysicsEvent ev = builder.getPhysicsEvent(electron,meson); 


	    pack.standardHistos[pass::mesonID]->Fill(event, electronIndex, mesonIndex, ev);

	    // check that event is kinematically 
	    // desirable
	    if (ev.w  > pack.conf.cuts["w"]["min"]  && 
		ev.w  < pack.conf.cuts["w"]["max"]  && 
		ev.qq > pack.conf.cuts["q2"]["min"] && 
		ev.qq < pack.conf.cuts["q2"]["max"] ){ 
	      
	      // do something 
	      pack.standardHistos[pass::SIDIS]->Fill(event, electronIndex, mesonIndex, ev);

	      if (sqrt(ev.mm2) > pack.conf.cuts["missing_mass"]["min"] && 
		  sqrt(ev.mm2) < pack.conf.cuts["missing_mass"]["max"] ){

		// do something more 
		// these are the real events 
		// determine helicity state 
		int hel = helicity::unknown; 
		if (event.corr_hel > 0){ hel = helicity::plus;  }
		if (event.corr_hel < 0){ hel = helicity::minus; }
		
		if(hel != helicity::unknown){
		  
		  pack.standardHistos[pass::all]->Fill(event, electronIndex, mesonIndex, ev);
		  pack.counts[hel]->Fill(ev);

		  // fill output tree 
		  pack.tree->hel      = hel; 
		  pack.tree->id       = pack.conf.mesonIndex; 
		  pack.tree->electron = electron; 
		  pack.tree->meson    = meson; 
		  pack.tree->tree->Fill();
		}
	      }
	    }


	  }

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

  void Save(){

    for (configPack p : packs){
      TFile *out = new TFile(Form("%s%s.root",
				  p.conf.outputPath.c_str(),
				  p.conf.name.c_str()), 
				  "recreate");
      
      out->SetCompressionLevel(3); 

      p.standardHistos[pass::mesonID]->Save(out);
      p.standardHistos[pass::SIDIS]  ->Save(out);
      p.standardHistos[pass::all]    ->Save(out);

      // saving counts 
      p.counts[helicity::plus] ->Save(out);      
      p.counts[helicity::minus]->Save(out);

      // save only if 
      // asked 
      if (p.conf.writeTree){
	p.tree->Save(out);
      }


      out->Write();
      out->Close();
    }

  }

protected:
  std::vector<configPack> packs; 
  MomCorr_e1f            *momCorr; 
  PhysicsEventBuilder     builder; 

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
    analysis.Save();

  } else {
    std::cerr << "No configurations found." << std::endl; 
  }

  return 0; 
}
