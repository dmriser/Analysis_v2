#include <iostream>
#include <vector>
using namespace std; 

#include "CommonTools.h"
#include "h22Option.h"
#include "GenericAnalysis.h"
#include "StandardHistograms.h"
#include "TFile.h"
#include "TH1.h"
#include "TLorentzVector.h"
#include "TString.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"
#include "ParticleFilter.h"
#include "Parameters.h"

class MyAnalysis : public GenericAnalysis {
public:
  MyAnalysis(h22Options *opt, Parameters *pars);
  ~MyAnalysis(){}

  StandardHistograms  *histos; 
  PhysicsEventBuilder *builder; 
  ParticleFilter      *filter; 
  Parameters          *params; 

public:

  void ProcessEvent();
  void Initialize();  
};

MyAnalysis::MyAnalysis(h22Options *opts, Parameters *pars) : GenericAnalysis(opts), params(pars){

}

void MyAnalysis::ProcessEvent(){
  vector<int> electronIndices      = filter->getVectorOfParticleIndices(event, 11); 
  vector<int> pimIndices           = filter->getVectorOfParticleIndices(event, -211); 
  vector<TLorentzVector> electrons = filter->getVectorOfTLorentzVectors(event, 11);
  vector<TLorentzVector> pims      = filter->getVectorOfTLorentzVectors(event, -211);

  if(!electrons.empty() && !pims.empty()){
    PhysicsEvent ev = builder->getPhysicsEvent(electrons[0], pims.back()); 
    histos->Fill(event, electronIndices[0], pimIndices.back(), ev);
  }

}

void MyAnalysis::Initialize(){
  histos  = new StandardHistograms("testHistos",0); 
  builder = new PhysicsEventBuilder(); 
  filter  = new ParticleFilter(params); 
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

  for (int i=0; i<opts->ifiles.size(); i++) { analysis.AddFile(opts->ifiles[i]); } 
  analysis.RunAnalysis();

  TFile *out = new TFile(opts->args["OUT"].args.c_str(),"recreate"); 
  analysis.histos->Save(out); 
  out->Close(); 

  return 0;
}
