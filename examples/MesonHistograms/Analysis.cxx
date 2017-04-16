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
#include "MomCorr.h"
#include "ParticleFilter.h"
#include "Parameters.h"

class MyAnalysis : public GenericAnalysis {
public:
  MyAnalysis(h22Options *opt, Parameters *pars);
  ~MyAnalysis(){}

  MesonHistograms     *pipHistos; 
  MesonHistograms     *kapHistos; 
  MesonHistograms     *posHistos; 
  MesonHistograms     *negHistos; 
  MesonHistograms     *pimHistos; 

  PhysicsEventBuilder *builder; 
  ParticleFilter      *filter; 
  Parameters          *params; 
  Corrections          corr; 
  MomCorr_e1f         *momCorr;

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
  vector<int> pipIndices           = filter->getVectorOfParticleIndices(event, 211); 
  vector<int> kapIndices           = filter->getVectorOfParticleIndices(event, 321); 

  vector<TLorentzVector> electrons = filter->getVectorOfTLorentzVectors(event, 11);
  vector<TLorentzVector> pims      = filter->getVectorOfTLorentzVectors(event, -211);
  vector<TLorentzVector> pips      = filter->getVectorOfTLorentzVectors(event, 211);


  if(!electrons.empty() && !electronIndices.empty()){
    h22ElectronEvent electronEvent(event);   
    electronEvent.SetElectronIndex(electronIndices.front());
    electronEvent.SetElectron(electrons.front()); 
    corr.CorrectElectronEvent(&electronEvent, GetRunNumber(), GSIM); 

      if(!pims.empty()){
	pimHistos->Fill(electronEvent, pimIndices[0]);
      }
      if(!pips.empty()){
	pipHistos->Fill(electronEvent, pipIndices[0]);
      }
      if(!kapIndices.empty()){
	kapHistos->Fill(electronEvent, kapIndices[0]); 
      }

      // grab just positives 
      for (int ipart=1; ipart<event.gpart; ipart++){

	if (ipart != electronEvent.GetElectronIndex()){
	  
	  TLorentzVector part = event.GetTLorentzVector(ipart, event.id[ipart]);
	  part = momCorr->PcorN(part, event.q[ipart], event.id[ipart]);	
	  
	  electronEvent.p[ipart] = part.P();
	  //	event.cx[ipart] = part.Px()/part.P();
	  //	event.cy[ipart] = part.Py()/part.P();
	  //	event.cz[ipart] = part.Pz()/part.P();
	  
	  //	double tof_mass = electronEvent.p[ipart] * sqrt((1-pow(electronEvent.corr_b[ipart],2))/pow(electronEvent.corr_b[ipart],2)); 
	  
	  PhysicsEvent physicsEvent = builder->getPhysicsEvent(electrons.front(), part); 
	  
	    if ( fabs( electronEvent.corr_vz[ipart] - electronEvent.corr_vz[electronEvent.GetElectronIndex()] ) <= 2.50)
	      if (physicsEvent.mm2 > 1.0 && physicsEvent.w > 2.0 && physicsEvent.qq > 1.0){
		if (event.q[ipart] == 1)
		  posHistos->Fill(electronEvent, ipart); 
		
		else if (event.q[ipart] == -1)
		  negHistos->Fill(electronEvent, ipart); 
	      }
	  //	      if (tof_mass > 0.35 && tof_mass < 0.65)
	}
      }
      
  }


}

void MyAnalysis::Initialize(){
  pipHistos = new MesonHistograms("testPipHistos", 211); 
  kapHistos = new MesonHistograms("testKapHistos", 321); 
  posHistos = new MesonHistograms("testPosHistos", 321); 
  negHistos = new MesonHistograms("testNegHistos", -321); 
  pimHistos = new MesonHistograms("testPimHistos", -211); 
  builder = new PhysicsEventBuilder(); 
  filter = new ParticleFilter(params); 
  momCorr = new MomCorr_e1f("/u/home/dmriser/Analysis_v2/momCorr/");
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
  analysis.pipHistos->Save(out); 
  analysis.kapHistos->Save(out); 
  analysis.pimHistos->Save(out); 
  analysis.posHistos->Save(out); 
  analysis.negHistos->Save(out); 
  out->Close(); 

  // statistics 
  analysis.filter->GetSelector(11)->PrintSummary();

  return 0;
}
