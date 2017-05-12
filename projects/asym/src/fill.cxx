////////////////////////////////////////
/*
 David Riser, University of Connecticut


 
 */
////////////////////////////////////////

#include <iostream>
#include <vector>
#include <map>

#include "CommonTools.h"
#include "Corrections.h"
#include "h22Event.h"
#include "h22Option.h"
#include "GenericAnalysis.h"
#include "MomCorr.h"
#include "Parameters.h"
#include "ParticleFilter.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"
#include "StandardHistograms.h"

#include "common/Histos.h"

#include "TFile.h"
#include "TLorentzVector.h"
#include "TF1.h"
#include "TH1.h"

std::vector<std::string> loadFilesFromList(std::string fileList, int numFiles);
std::vector<std::string> loadFilesFromCommandLine(h22Options * theseOpts, int numFiles);

class Analysis : public GenericAnalysis {

public:
    Analysis(h22Options *opts, Parameters *params) : GenericAnalysis(opts), pars(params) { }
    ~Analysis(){ }

public:
  Parameters          *pars;
  ParticleFilter      *filter;
  Corrections          corr;
  PhysicsEventBuilder *builder;   
  MomCorr_e1f         *momCorr; 
  
  AsymmetryHistograms *kAsym; 


  void Initialize();
  void ProcessEvent();
  void Save(std::string outfile);
  void InitHistos();
  bool CurrentParticleIsNotElectronCandidate(std::vector<int> &electronCandidates, int index);
};

void Analysis::InitHistos() {
  kAsym = new AsymmetryHistograms("test"); 
}

void Analysis::Initialize(){
  InitHistos();
  
  filter = new ParticleFilter(pars);
  filter->set_info(GSIM, GetRunNumber());

  builder = new PhysicsEventBuilder(); 

  // not really safe for use on the farm because I don't set vars 
  // so not sure what to do about this.  maybe passing in the path
  // as a parameter would be ok? 
  std::string path = Global::Environment::GetAnalysisPath(); 
  std::string momCorrPath = Form("%s/momCorr/",path.c_str());
  momCorr = new MomCorr_e1f(momCorrPath);
}

void Analysis::ProcessEvent(){
    // Load up hadrons if we've electron.
  std::vector<int> electronCandidates = filter->getVectorOfParticleIndices(event, 11);

  if ( !electronCandidates.empty()){
    
    // Take the fastest one
    int electronIndex = electronCandidates[0];
    event.SetElectronIndex(electronIndex); 
    corr.correctEvent(&event, GetRunNumber(), GSIM); 

    std::vector<int> kpCandidates = filter->getVectorOfParticleIndices(event, 321);
    std::vector<int> kmCandidates = filter->getVectorOfParticleIndices(event, -321);
    
    int helicity = -1; 
    if (event.corr_hel > 0)     { helicity = 1; }
    else if (event.corr_hel < 0){ helicity = 0; }

    if (!kpCandidates.empty() && helicity > -1) {
      TLorentzVector electron  = event.GetTLorentzVector(electronIndex, 11); 
      electron = momCorr->PcorN(electron, -1, 11);    
      
      TLorentzVector kaon = event.GetTLorentzVector(kpCandidates[0], 321);
      //    pion = momCorr->PcorN(pion, 1, 211);
      
      PhysicsEvent candidateEvent = builder->getPhysicsEvent(electron, kaon);
      
      if (candidateEvent.w > 2.00 && candidateEvent.qq > 1.00 && candidateEvent.mm2 > 1.1
	  && candidateEvent.z > 0.3 && kaon.P() < 2.05) {
	
	kAsym->Fill(candidateEvent, 0, helicity);
	
      }
    }


    if (!kmCandidates.empty() && helicity > -1) {
      TLorentzVector electron  = event.GetTLorentzVector(electronIndex, 11); 
      electron = momCorr->PcorN(electron, -1, 11);    
      
      TLorentzVector kaon = event.GetTLorentzVector(kmCandidates[0], -321);
      //    pion = momCorr->PcorN(pion, 1, 211);
      
      PhysicsEvent candidateEvent = builder->getPhysicsEvent(electron, kaon);
      
      if (candidateEvent.w > 2.00 && candidateEvent.qq > 1.00 && candidateEvent.mm2 > 1.1
	  && candidateEvent.z > 0.3 && kaon.P() < 2.75) {
	
	kAsym->Fill(candidateEvent, 1, helicity);
      }
    }

  }
}

bool Analysis::CurrentParticleIsNotElectronCandidate(std::vector<int> &electronCandidates,int index){
  return (std::find(electronCandidates.begin(), electronCandidates.end(), index) == electronCandidates.end());
}

void Analysis::Save(std::string outfile){
  kAsym->Save(outfile, "recreate", 1); 
}
 
int main(int argc, char * argv[]){
  
  // Setup Options
  h22Options opts;
  opts.args["PARS"].args = "/u/home/dmriser/Analysis_v2/lists/data.pars";
  opts.args["PARS"].type = 1;
  opts.args["PARS"].name = "Parameter file";
  opts.args["LIST"].args = "UNSET";
  opts.args["LIST"].type = 1;
  opts.args["LIST"].name = "List of files";
  opts.set(argc,argv);

  Parameters pars;
  pars.loadParameters(opts.args["PARS"].args);
  
  std::vector<std::string> files; 
  if (opts.args["LIST"].args != "UNSET"){
    files = loadFilesFromList(opts.args["LIST"].args, 10000);
  } else {
    files = loadFilesFromCommandLine(&opts, 10000);
  }

  Analysis *Ana = new Analysis(&opts, &pars); 
  for (int i=0; i<files.size(); i++) {   
    Ana->AddFile(files[i]); 
  }
  Ana->RunAnalysis();
  Ana->kAsym->CalculateAsymmetry();
  Ana->Save(opts.args["OUT"].args); 
  
  return 0;
}

std::vector<std::string> loadFilesFromList(std::string fileList, int numFiles){
  std::vector<std::string> theseFiles;
  
  std::ifstream inputFile;
  inputFile.open(fileList.c_str());

  int ifile = 0; string line;
  while (getline(inputFile, line) && ifile < numFiles){
    theseFiles.push_back(line);
    ifile++;
  }

  inputFile.close();
  return theseFiles;
}

std::vector<std::string> loadFilesFromCommandLine(h22Options * theseOpts, int numFiles){
  std::vector<std::string> theseFiles;

  for(int ifile = 0; ifile < theseOpts->ifiles.size(); ifile++){
    theseFiles.push_back(theseOpts->ifiles[ifile]);

    if (ifile == numFiles){ break; }
  }

  return theseFiles;
}
