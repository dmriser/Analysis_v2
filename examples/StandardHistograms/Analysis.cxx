#include <iostream>
#include <vector>
using namespace std; 

#include "StandardHistograms.h"

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
#include "PhotonHistograms.h"
#include "ParticleFilter.h"
#include "Parameters.h"

class MyAnalysis : public GenericAnalysis {
public:
  MyAnalysis(h22Options *opt, Parameters *pars);
  ~MyAnalysis(){}

  StandardHistograms  *histos; 
  PhotonHistograms    *photonHistos; 
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
  filter->GetSelector(11)->Optimize();
}

void MyAnalysis::ProcessEvent(){
  vector<int> electronIndices      = filter->getVectorOfParticleIndices(event, 11); 
  vector<int> pimIndices           = filter->getVectorOfParticleIndices(event, -211); 
  vector<int> photIndices          = filter->getVectorOfParticleIndices(event, 22);

  vector<TLorentzVector> electrons = filter->getVectorOfTLorentzVectors(event, 11);
  vector<TLorentzVector> pims      = filter->getVectorOfTLorentzVectors(event, -211);
  vector<TLorentzVector> phots     = filter->getVectorOfTLorentzVectors(event, 22);


  if(!electrons.empty() && !electronIndices.empty()){
    event.SetElectronIndex(electronIndices.front());
    corr.correctEvent(&event, GetRunNumber(), GSIM); 

    //    std::cout << event.GetPhi(electronIndices.front()) << std::endl; 

      if(!pims.empty()){
	PhysicsEvent ev = builder->getPhysicsEvent(electrons[0], pims.back()); 
	histos->Fill(event, electronIndices[0], pimIndices.back(), ev);
      }

      if(!phots.empty()){
	photonHistos->Fill(event, photIndices[0]);
      }
  }

}

void MyAnalysis::Initialize(){
  histos  = new StandardHistograms("testHistos",0); 
  photonHistos = new PhotonHistograms("testPhotonHistos"); 
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
    analysis.AddFile( opts->ifiles[i] ); 
  } 

  analysis.RunAnalysis();

  TFile *out = new TFile(opts->args["OUT"].args.c_str(),"recreate"); 
  analysis.histos->Save(out); 
  analysis.photonHistos->Save(out); 
  out->Close(); 

  // statistics 
  analysis.filter->GetSelector(11)->PrintSummary();

  return 0;
}
