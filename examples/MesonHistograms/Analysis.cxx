#include <iostream>
#include <vector>
using namespace std; 


#include "TFile.h"
#include "TH1.h"
#include "TLorentzVector.h"
#include "TString.h"

#include "CommonTools.h"
#include "Corrections.h"
#include "h22ElectronEvent.h"
#include "h22Option.h"
#include "GenericAnalysis.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"
#include "MesonHistograms.h"
#include "ParticleFilter.h"
#include "Parameters.h"

class MyAnalysis : public GenericAnalysis {
public:
  MyAnalysis(h22Options *opt, Parameters *pars);
  ~MyAnalysis(){}

  MesonHistograms     *pipHistos; 
  MesonHistograms     *kapHistos; 
  MesonHistograms     *pimHistos; 
  PhysicsEventBuilder *builder; 
  ParticleFilter      *filter; 
  Parameters          *params; 
  Corrections          corr; 

public:

  void ProcessEvent();
  void Initialize();  
  void OptimizeLoop(int numberOfEvents);
};

MyAnalysis::MyAnalysis(h22Options *opts, Parameters *pars) : GenericAnalysis(opts), params(pars){
  //  filter->getSelector(11)->disable_by_regex("CCT");
}

void MyAnalysis::OptimizeLoop(int numberOfEvents){
  cout << "[MyAnalysis::OptimizeLoop] Optimizing electron selector with events = " << numberOfEvents << endl; 

  for(int ievent=0; ievent<numberOfEvents; ievent++){
    GetEntry(ievent);
    
    // just calling the cuts to get statistics 
    vector<int> electrons = filter->getVectorOfParticleIndices(event, 11); 
  }

  // optimize selector 
  filter->getSelector(11)->optimize();
}

void MyAnalysis::ProcessEvent(){
  vector<int> electronIndices      = filter->getVectorOfParticleIndices(event, 11); 
  vector<int> pimIndices           = filter->getVectorOfParticleIndices(event, -211); 
  vector<int> pipIndices           = filter->getVectorOfParticleIndices(event, 211); 
  vector<int> kapIndices           = filter->getVectorOfParticleIndices(event, 321); 

  vector<TLorentzVector> electrons = filter->getVectorOfTLorentzVectors(event, 11);
  vector<TLorentzVector> pims      = filter->getVectorOfTLorentzVectors(event, -211);
  vector<TLorentzVector> pips      = filter->getVectorOfTLorentzVectors(event, 211);


  if(!electrons.empty() && !electronIndices.empty()){
    h22ElectronEvent electronEvent(event);   
    electronEvent.SetElectronIndex(electronIndices.front());
    electronEvent.SetElectron(electrons.front()); 
    corr.CorrectElectronEvent(&electronEvent, runno(), GSIM); 

      if(!pims.empty()){
	pimHistos->Fill(electronEvent, pimIndices[0]);
      }
      if(!pips.empty()){
	pipHistos->Fill(electronEvent, pipIndices[0]);
      }
      if(!kapIndices.empty()){
	kapHistos->Fill(electronEvent, kapIndices[0]); 
      }
  }

}

void MyAnalysis::Initialize(){
  pipHistos = new MesonHistograms("testPipHistos", 211); 
  kapHistos = new MesonHistograms("testKapHistos", 321); 
  pimHistos = new MesonHistograms("testPimHistos", -211); 
  builder = new PhysicsEventBuilder(); 
  filter = new ParticleFilter(params); 
}

int main(int argc, char *argv[]){

  if (argc < 2){
    cout << " No files ? " << endl; 
    return 0;
  }

  h22Options *opts = new h22Options();
  opts->args["PARS"].args = "/u/home/dmriser/mydoc/analysis/root_scripts/Analysis_v2/lists/data.pars";
  opts->args["PARS"].type = 1;
  opts->args["PARS"].name = "Parameter file"; 
  opts->set(argc, argv); 

  Parameters *pars = new Parameters(); 
  pars->loadParameters(opts->args["PARS"].args); 

  MyAnalysis analysis(opts, pars);

  for (int i=0; i<opts->ifiles.size(); i++) { 
    analysis.AddFile( opts->ifiles[i].c_str() ); 
  } 

  analysis.RunAnalysis();

  TFile *out = new TFile(opts->args["OUT"].args.c_str(),"recreate"); 
  analysis.pipHistos->Save(out); 
  analysis.kapHistos->Save(out); 
  analysis.pimHistos->Save(out); 
  out->Close(); 

  // statistics 
  analysis.filter->getSelector(11)->summarize();

  return 0;
}
