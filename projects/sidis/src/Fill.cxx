#include <iostream>
#include <vector>
using std::cout; 
using std::endl; 

// Mine
#include "CommonTools.h"
#include "h22Option.h"
#include "GenericAnalysis.h"
#include "Parameters.h"
#include "ParticleFilter.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"

// Root
#include "TString.h"
#include "TLorentzVector.h"
#include "TH1.h"
#include "TH2.h"
#include "THnSparse.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TPad.h"
#include "TStyle.h"

#define DIM 5

class Fill : public GenericAnalysis {
public:
  Fill(Parameters *pars, std::string outputFile) : outputFilename(outputFile) { filter = new ParticleFilter(pars); } 
  ~Fill(){}
  
  ParticleFilter      *filter;
  PhysicsEventBuilder *builder;

  THnSparseI *pipEvents, *pimEvents, *pizEvents; 

  std::string outputFilename;

  void ProcessEvent();
  void Initialize();
  void Save();
};

void Fill::Initialize(){
  filter->set_info(runno(), GSIM);
  builder     = new PhysicsEventBuilder();

  // Dimensions 
  // [0] - x 
  // [1] - z 
  // [2] - Q^{2} 
  // [3] - Phi_{Hadron}
  // [4] - P_{T}^{2}

  int numberOfBins[DIM] = {100, 100, 100, 100, 100};
  double min[DIM]       = {0.0, 0.0, 0.8, -180.0, 0.0};
  double max[DIM]       = {1.0, 1.0, 5.0,  180.0, 1.2};

  pipEvents = new THnSparseI("pipEvents","pipEvents",DIM,numberOfBins,min,max); 
  pimEvents = new THnSparseI("pimEvents","pimEvents",DIM,numberOfBins,min,max); 
  pizEvents = new THnSparseI("pizEvents","pizEvents",DIM,numberOfBins,min,max); 

  cout << "[Fill::Init] Initialization done" << endl;
}

void Fill::ProcessEvent(){  
  std::vector<TLorentzVector> pims, pips, pi0s, electrons; 
  electrons = filter->getVectorOfTLorentzVectors(event,   11);
  pims      = filter->getVectorOfTLorentzVectors(event, -211);
  pips      = filter->getVectorOfTLorentzVectors(event,  211);
  pi0s      = filter->getVectorOfTLorentzVectors(event,  111);

  if ( !electrons.empty() ){
    if( !pims.empty() ) {
      PhysicsEvent pionEvent = builder->getPhysicsEvent(electrons[0], pims[0]);    
      double dataPoint[DIM]  = {pionEvent.x, pionEvent.z, pionEvent.qq, pionEvent.phiHadron, TMath::Power(pionEvent.pT,2)};
      pimEvents->Fill(dataPoint);
    } 
    
    if( !pips.empty() ) {
      PhysicsEvent pionEvent = builder->getPhysicsEvent(electrons[0], pips[0]);    
      double dataPoint[DIM]  = {pionEvent.x, pionEvent.z, pionEvent.qq, pionEvent.phiHadron, TMath::Power(pionEvent.pT,2)};
      pipEvents->Fill(dataPoint);
    } 
    
    if( !pi0s.empty() ) {
      PhysicsEvent pionEvent = builder->getPhysicsEvent(electrons[0], pi0s[0]);    
      double dataPoint[DIM]  = {pionEvent.x, pionEvent.z, pionEvent.qq, pionEvent.phiHadron, TMath::Power(pionEvent.pT,2)};
      pizEvents->Fill(dataPoint);
    } 
  }
  
}

void Fill::Save(){
  TFile *outputFile = new TFile(outputFilename.c_str(),"RECREATE"); 
  pipEvents ->Write();
  pimEvents ->Write();
  pizEvents ->Write();
  outputFile->Write();
  outputFile->Close();
}

////////////////////////////////////////////////////////////////////////////////
/*

                                Start main code

 */
////////////////////////////////////////////////////////////////////////////////


int main(int argc, char *argv[]){

  h22Options *options = new h22Options();

  options->args["PARS"].args = "/u/home/dmriser/mydoc/analysis/root_scripts/Analysis_v2/lists/data.pars";
  options->args["PARS"].type = 1;
  options->args["PARS"].name = "Parameter file";
  options->set(argc, argv);

  Parameters *pars = new Parameters(); 
  pars->loadParameters(options->args["PARS"].args); 

  Fill analysis(pars, options->args["OUT"].args);

  if(options->ifiles.size() > 0){
    for (int i=0; i<options->ifiles.size(); i++) { analysis.AddFile(options->ifiles[i]); } 
    analysis.RunAnalysis(options->args["N"].arg);
  }

  else {
    cout << "[Fill::Main] No files added to options->ifiles " << endl; 
  }


  return 0;
}
