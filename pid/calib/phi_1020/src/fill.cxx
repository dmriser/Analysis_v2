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
#include "SimpleNTupleWriter.h"
#include "StatusBar.h"
#include "StandardHistograms.h"

// root 
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TLorentzVector.h"
#include "TStopwatch.h"
#include "TVector3.h"

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
    mm_epx[0]  = new TH1F("mm_epx_no_cuts",          "", 100, -0.2, 1.8);
    mm_epx[1]  = new TH1F("mm_epx_tag_kp",           "", 100, -0.2, 1.8);
    mm_epx[2]  = new TH1F("mm_epx_tag_kp_cut_km_mm", "", 100, -0.2, 1.8);
    mm_epx[3]  = new TH1F("mm_epx_tag_kp_cut_im_kk", "", 100, -0.2, 1.8);
    mm_epkx[0] = new TH1F("mm_epkx_no_cut", "", 100, -0.2, 1.2);
    mm_epkx[1] = new TH1F("mm_epkx_cut_mm_phi", "", 100, -0.2, 1.2);
    im_kk      = new TH1F("im_kk", "", 100, -0.2, 1.8);

    // ntuple 
    tupleWriter.addInt("helicity");
    tupleWriter.addFloat("im_kk"); 
    tupleWriter.addFloat("missing_mass_epx"); 
    tupleWriter.addFloat("missing_mass_epkx"); 

  }

  ~Analysis(){
    // total destruction 
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
	
	std::vector<int> kpIndices = filter->getVectorOfParticleIndices(event, 321);
	std::vector<int> protonIndices = filter->getVectorOfParticleIndices(event, 2212);

	if (!protonIndices.empty()){

	  // building event 
	  int protonIndex         = protonIndices[0]; 
	  TLorentzVector electron = event.GetTLorentzVector(electronIndex, 11);  
	  TLorentzVector proton   = event.GetTLorentzVector(protonIndex, 2212);
	  
	  // momentum correction done here 
	  // because if we dont find proton it's useless 
	  // to waste cpu doing it above 
	  electron = momCorr->PcorN(electron, -1, 11);

	  // build physics event with only proton 
	  PhysicsEvent ev = builder.getPhysicsEvent(electron, proton); 	  

	  mm_epx[0]->Fill( sqrt(ev.mm2) );

	  //  now use kp if it exists 
	  if (!kpIndices.empty()){
	    int kaonIndex = kpIndices[0];
	    
	    // stop doing things if its actually a pion 
	    if( IsMisidentifiedPion(electron, kaonIndex) ){
	      continue; 
	    }

	    TLorentzVector kp     = event.GetTLorentzVector(kaonIndex, 321);
	    PhysicsEvent epkEvent = builder.getPhysicsEvent(electron, proton, kp);

	    mm_epx[1]->Fill( sqrt(ev.mm2) );
	    mm_epkx[0]->Fill( sqrt(epkEvent.mm2) );

	    tupleWriter.setInt("helicity",            event.corr_hel); 
	    tupleWriter.setFloat("missing_mass_epx",  sqrt(ev.mm2));  
	    tupleWriter.setFloat("missing_mass_epkx", sqrt(epkEvent.mm2));  
	    tupleWriter.writeEvent(); 

	    if ( (sqrt(ev.mm2) > 0.990) && (sqrt(ev.mm2) < 1.050) ){
	      mm_epkx[1]->Fill( sqrt(epkEvent.mm2) );
	    }
	      
	    if ( (sqrt(epkEvent.mm2) > 0.45) && (sqrt(epkEvent.mm2) < 0.55) ){
	      mm_epx[2]->Fill( sqrt(ev.mm2) );
	      
	      // the final state is a km 
	      TLorentzVector km = epkEvent.finalState; 
	      TLorentzVector phi = km+kp; 
	      im_kk->Fill( sqrt(phi.M2()) );
	      
	      // if im_kk is phi mass then fill final sample 
	      if ( (sqrt(phi.M2()) > 0.990) && (sqrt(phi.M2()) < 1.050) ){
		mm_epx[3]->Fill( sqrt(ev.mm2) );
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

  bool IsMisidentifiedPion(TLorentzVector & electron, int mesonIndex){
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
      return true; 
    }
    
    return false; 
  }

  void Save(std::string outFile){
    TFile *out = new TFile(outFile.c_str(), "recreate");

    for (int i = 0; i < 4; i++) { mm_epx[i]->Write(); }
    mm_epkx[0]->Write();
    mm_epkx[1]->Write();
    im_kk     ->Write();     

    tupleWriter.save(out); 

    out->Close();
  }


protected:
  MomCorr_e1f             *momCorr; 
  PhysicsEventBuilder      builder; 
  ParticleFilter          *filter; 
  Parameters              *params; 

  SimpleNTupleWriter      tupleWriter; 

  // histograms 

  // 0 - only proton, no cuts
  // 1 - proton and kp
  // 2 - proton and kp, cut on missing km mass from epkx  
  // 3 - proton and kp, cut on im of km 
  TH1F *mm_epx[4];

  // 0 - proton and kp, no cuts 
  // 1 - proton and kp, cut on X ~ phi-mass
  TH1F *mm_epkx[2];

  // no comment 
  TH1F *im_kk;
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
  analysis.Save("out.root");
  
  } else {
    std::cerr << "No files found." << std::endl; 
  }

  return 0; 
}
