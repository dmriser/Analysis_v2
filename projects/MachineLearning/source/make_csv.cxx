#include <iostream>
#include <fstream>

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

class Analysis : public h22Reader {
public:  
  Analysis() {

    std::string path        = Global::Environment::GetAnalysisPath(); 
    std::string momCorrPath = Form("%s/momCorr/",path.c_str());
    momCorr                 = new MomCorr_e1f(momCorrPath);


    GSIM = false; 
    Init();

    outStream.open("out/data.csv", std::ios::trunc); 
  }

  ~Analysis(){
    outStream.close(); 
  }

  void Loop(){

    StatusBar stat; 

    TStopwatch timer; 
    timer.Reset();
    timer.Start();
    
    // for every event
    for(int ientry=0; ientry<GetEntries(); ientry++){
      GetEntry(ientry);
      
            
      std::vector<int> electronIndices = event.GetVectorOfUserAnaParticleIndices(11); 
      if(!electronIndices.empty()){
	int electronIndex = electronIndices[0];
	event.SetElectronIndex(electronIndex);
	Corrections::correctEvent(&event, GetRunNumber(), GSIM);


	std::vector<int> protons = event.GetVectorOfUserAnaParticleIndices(2212); 
	std::vector<int> pions   = event.GetVectorOfUserAnaParticleIndices(211); 
	std::vector<int> kaons   = event.GetVectorOfUserAnaParticleIndices(321); 
	WriteParticles(protons); 
	WriteParticles( pions ); 
	WriteParticles( kaons ); 
      }
      
      
      /*      
      if (ientry%10000 == 0){
	stat.PrintStatus(ientry, GetEntries()); 
      }
      */
    }

      std::cout << std::endl;
      double loopTime  = timer.RealTime(); 
      double eventRate = GetEntries()/loopTime; 
      std::cout << "[GenericAnalysis::Loop] Total time: " 
		<< loopTime << " seconds, Event rate: " 
		<< eventRate << " events/sec. " << std::endl;
      
  }
  

protected:

  void WriteParticles(std::vector<int> indices){
    for(int index : indices){
      outStream << event.p[index] << ","; 
      outStream << event.nphe[index] << ","; 
      outStream << event.etot[index] << ","; 
      outStream << event.ec_ei[index] << ","; 
      outStream << event.ec_eo[index] << ","; 
      outStream << pow(event.p[index],2)*(1-pow(event.corr_b[index],2))/pow(event.corr_b[index],2) << std::endl; 
    }
  }

  std::ofstream outStream;

  MomCorr_e1f            *momCorr; 
  PhysicsEventBuilder     builder; 

};

int main(int argc, char *argv[]){
  
  if (argc < 2){
    exit(0); 
  }

  Analysis analysis; 
  for (int iarg=1; iarg<argc; iarg++){
    std::string file(argv[iarg]); 
    analysis.AddFile(file); 
  }

  analysis.Loop(); 

  return 0; 
}
