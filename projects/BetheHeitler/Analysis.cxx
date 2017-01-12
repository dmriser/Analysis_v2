#include <iostream>
#include <vector>
using std::cout; 
using std::endl; 

// Mine
#include "CommonTools.h"
#include "DBins.h"
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
#include "TMath.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TPad.h"
#include "TStyle.h"

#define DIM 5

class MyAnalysis : public GenericAnalysis {
public:
  MyAnalysis(Parameters *pars){ filter = new ParticleFilter(pars); } 
  ~MyAnalysis(){ }
  
  ParticleFilter *filter;
  PhysicsEventBuilder *builder;

  THnSparseI *events; 

  DBins *angleBins, *massBins, *wBins, *dAngleBins, *finalAngleBins; 

  void ProcessEvent();
  void Initialize();
  void Display();
  void Save(); 
};

// Example override of Init using 
// the methods of h22Reader runno()
// and GSIM const.
void MyAnalysis::Initialize(){
  filter->set_info(runno(), GSIM);
  builder     = new PhysicsEventBuilder();

  angleBins      = new DBins(200, -12.0, 65);
  dAngleBins     = new DBins(200,  0, 360); 
  massBins       = new DBins(200, -1.0, 5.0);  
  wBins          = new DBins(200,   1.1, 3.3); 
  finalAngleBins = new DBins(200, -12.0, 80.0); 

  int num[DIM]    = {angleBins->number(), massBins->number(), wBins->number(), dAngleBins->number(), finalAngleBins->number()};
  double min[DIM] = {angleBins->min(),       massBins->min(),    wBins->min(), dAngleBins->min(),    finalAngleBins->min()};
  double max[DIM] = {angleBins->max(),       massBins->max(),    wBins->max(), dAngleBins->max(),    finalAngleBins->max()};

  events = new THnSparseI("events","events",DIM,num,min,max); 

  cout << "[MyAnalysis::Init] Initialization done" << endl;
}

// Example overwriting the event action 
void MyAnalysis::ProcessEvent(){  
  std::vector<TLorentzVector> protons, electrons; 
  electrons = filter->getVectorOfTLorentzVectors(event,   11);
  protons   = filter->getVectorOfTLorentzVectors(event, 2212);

  if ( !electrons.empty() && !protons.empty() ){
    for(int aProton=0; aProton < protons.size(); aProton++){
      PhysicsEvent physicsEvent = builder->getPhysicsEvent(electrons[0], protons[aProton]);
      double point[DIM] = {physicsEvent.finalState.Theta()*to_degrees, physicsEvent.mm2, 
			   physicsEvent.w,to_degrees*(electrons[0].Phi()-protons[aProton].Phi()), to_degrees*physicsEvent.finalState.Angle(electrons[0].Vect())};
      //      double point[DIM] = {physicsEvent.finalState.Theta()*to_degrees, physicsEvent.mm2, 
      //      			   physicsEvent.w,to_degrees*(electrons[0].Phi()-protons[aProton].Phi()), to_degrees*(physicsEvent.finalState.Theta()-electrons[0].Theta())};
      events->Fill(point); 
    }
  }

}

void MyAnalysis::Save(){
  TFile *outputFile = new TFile("BetheHeitlerEvents.root","recreate"); 
  events->Write(); 
  outputFile->Write();
  outputFile->Close();
}

int main(int argc, char *argv[]){

  h22Options *options = new h22Options();

  options->args["PARS"].args = "/u/home/dmriser/mydoc/analysis/root_scripts/Analysis_v2/lists/data.pars";
  options->args["PARS"].type = 1;
  options->args["PARS"].name = "Parameter file";
  options->set(argc, argv);

  Parameters *pars = new Parameters(); 
  pars->loadParameters(options->args["PARS"].args); 

  MyAnalysis analysis(pars);

  if(options->ifiles.size() > 0){
    for (int i=0; i<options->ifiles.size(); i++) { analysis.AddFile(options->ifiles[i]); } 
    analysis.RunAnalysis(options->args["N"].arg);
    analysis.Save();
  }

  else {
    cout << "[MyAnalysis::Main] No files added to options->ifiles " << endl; 
  }


  return 0;
}
