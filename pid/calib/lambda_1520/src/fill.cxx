#include <iostream>

// h22 libs 
#include "CommonTools.h"
#include "Corrections.h"
#include "CheckPoints.h"
#include "DBins.h"
#include "h22Event.h"
#include "h22Reader.h"
#include "MesonHistograms.h"
#include "MomCorr.h"
#include "ParticleFilter.h"
#include "Parameters.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"
#include "StatusBar.h"
#include "StandardHistograms.h"

// root 
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TLorentzVector.h"
#include "TStopwatch.h"

const static float M_SIGMA_PLUS  = 1.189;
const static float M_SIGMA_MINUS = 1.197;

class Analysis : public h22Reader {
public:  
  Analysis() {

    std::string path        = Global::Environment::GetAnalysisPath(); 
    std::string momCorrPath = Form("%s/momCorr/",path.c_str());
    momCorr                 = new MomCorr_e1f(momCorrPath);

    // setup reader options 
    GSIM = false; 
    Init();

    // needs parameters 
    params = new Parameters(); 
    params->loadParameters(Form("%s/lists/data_tofmass.pars", path.c_str())); 
    filter = new ParticleFilter(params);

    // histos 
    mm_ek[0] = new TH1I("mm_ek_no_cut", "", 100, 1.1, 1.8);
    mm_ek[1] = new TH1I("mm_ek_prot", "", 100, 1.1, 1.8);
    mm_ek[2] = new TH1I("mm_ek_pip", "", 100, 1.1, 1.8);
    mm_ek[3] = new TH1I("mm_ek_pim", "", 100, 1.1, 1.8);

    mm_ekpion[0] = new TH1I("mm_ekpion_plus","",100,0.9,1.6);
    mm_ekpion[1] = new TH1I("mm_ekpion_minus","",100,0.9,1.6);

    mm_ekproton  = new TH1I("mm_ekproton","",100,0.0,0.9);
  }

  ~Analysis(){
  }

  void Loop(){
    
    // setup particle filter 
    filter->set_info(GSIM, GetRunNumber());

    StatusBar stat; 

    TStopwatch timer; 
    timer.Reset();
    timer.Start();

    // for every event
    for(int ientry=0; ientry<GetEntries(); ientry++){
      GetEntry(ientry);
      
      std::vector<int> electronIndices = filter->getVectorOfParticleIndices(event, 11);
      if(!electronIndices.empty()){
	
	int electronIndex = electronIndices[0];
	event.SetElectronIndex(electronIndex);
	Corrections::correctEvent(&event, GetRunNumber(), GSIM);
	
	std::vector<int> mesonIndices = filter->getVectorOfParticleIndices(event, 321);
	if (!mesonIndices.empty()){
	  
	  // build an event 
	  int mesonIndex = mesonIndices[0];
	  TLorentzVector electron = event.GetTLorentzVector(electronIndex, 11);  
	  TLorentzVector meson    = event.GetTLorentzVector(mesonIndex, 321);
	  
	  electron = momCorr->PcorN(electron, -1, 11);
	  PhysicsEvent ev = builder.getPhysicsEvent(electron,meson); 
	  
	  
	  // Dan Carman method for rejecting some pi+ events in the 
	  // kaon sample. 
	  //
	  // Classify an event and if it's a kaon you pretend 
	  // it's a pion.  Then check the missing mass spectrum 
	  // and if the event is near the neutron peak it must 
	  // really be a pion, so reassign it to be.  
	  //
	  // Currently all it does it throw out the pion, 
	  // if doesn't actually add it to the pion sample 
	  // because the code never makes it that far in the 
	  // pion case it would just return 0 pions and not enter.
	  TLorentzVector pion = event.GetTLorentzVector(mesonIndex, 211); 
	  PhysicsEvent pionEvent = builder.getPhysicsEvent(electron,pion); 
	  
	  // reject and dont process event 
	  if (sqrt(pionEvent.mm2) < 1.07){
	    continue; 
	  }

	  // fill the normal one 
	  mm_ek[0]->Fill(sqrt(ev.mm2));

	  // look for things 
	  std::vector<int> protonIndices = filter->getVectorOfParticleIndices(event, 2212);
	  std::vector<int> pipIndices    = filter->getVectorOfParticleIndices(event, 211);
	  std::vector<int> pimIndices    = filter->getVectorOfParticleIndices(event,-211);

	  if (!protonIndices.empty()){
	    TLorentzVector proton    = event.GetTLorentzVector(protonIndices[0], 2212);
	    PhysicsEvent protonEvent = builder.getPhysicsEvent(electron,meson,proton); 

	    mm_ekproton->Fill(sqrt(protonEvent.mm2));

	    if(sqrt(protonEvent.mm2) > 0.45 && 
	       sqrt(protonEvent.mm2) < 0.55){
	      mm_ek[1]->Fill(sqrt(ev.mm2));
	    }
	  }

	  if(!pipIndices.empty()){
	    TLorentzVector pion    = event.GetTLorentzVector(pipIndices[0], 211);
	    PhysicsEvent pionEvent = builder.getPhysicsEvent(electron,meson,pion); 

	    mm_ekpion[0]->Fill(sqrt(pionEvent.mm2));

	    if(sqrt(pionEvent.mm2) > 1.13 && 
	       sqrt(pionEvent.mm2) < 1.26){
	      mm_ek[2]->Fill(sqrt(ev.mm2));
	    }
	  }

	  if(!pimIndices.empty()){
	    TLorentzVector pion    = event.GetTLorentzVector(pimIndices[0], -211);
	    PhysicsEvent pionEvent = builder.getPhysicsEvent(electron,meson,pion); 

	    mm_ekpion[1]->Fill(sqrt(pionEvent.mm2));

	    if(sqrt(pionEvent.mm2) > 1.13 && 
	       sqrt(pionEvent.mm2) < 1.26){
	      mm_ek[3]->Fill(sqrt(ev.mm2));
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

  void Save(std::string outFile){
    TFile *out = new TFile(outFile.c_str(), "recreate");

    mm_ek[0]->Write(); 
    mm_ek[1]->Write(); 
    mm_ek[2]->Write(); 
    mm_ek[3]->Write(); 
    mm_ekpion[0]->Write(); 
    mm_ekpion[1]->Write(); 
    mm_ekproton->Write(); 

    out->Close();
  }


protected:
  MomCorr_e1f             *momCorr; 
  PhysicsEventBuilder      builder; 
  ParticleFilter          *filter; 
  Parameters              *params; 

  // histograms 
  // 0 - no cuts 
  // 1 - proton in event and cut on k- mass
  // 2 - pi+ in event and cut on sigma mass 
  // 3 - pi- in event and cut on sigma mass 
  TH1I *mm_ek[4];

  // centered at sigma mass 
  // 0 - pi+ 
  // 1 - pi- 
  TH1I *mm_ekpion[2];

  // centered at kaon mass 
  // 0 - proton 
  TH1I *mm_ekproton;
  
};

int main(int argc, char *argv[]){

  if (argc > 1){
  // setup analysis 
  Analysis analysis; 

  for (int iarg=1; iarg<argc; iarg++){
    std::string f(argv[iarg]);
    analysis.AddFile(f); 
  }

  // run analysis loop
  analysis.Loop();
  analysis.Save("/u/home/dmriser/Analysis_v2/pid/calib/lambda_1520/out/out.root");
  
  } else {
    std::cerr << "No files found." << std::endl; 
  }

  return 0; 
}
