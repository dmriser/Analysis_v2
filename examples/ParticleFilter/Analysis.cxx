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
#include "TCanvas.h"
#include "TApplication.h"
#include "TPad.h"
#include "TStyle.h"

class MyAnalysis : public GenericAnalysis {
public:
  MyAnalysis(Parameters *pars){ filter = new ParticleFilter(pars); } 
  ~MyAnalysis(){}
  
  ParticleFilter *filter;
  PhysicsEventBuilder *builder;

  TH1I *pTpip, *pTpim, *pTpi0;
  TH1I *phiPip, *phiPim, *phiPi0;
  TH2I *pTzPip, *pTzPim, *pTzPi0, *xQQ, *wQQ;

  void ProcessEvent();
  void Initialize();
  void Display();
};

// Example override of Init using 
// the methods of h22Reader runno()
// and GSIM const.
void MyAnalysis::Initialize(){
  filter->set_info(GetRunNumber(), GSIM);
  builder     = new PhysicsEventBuilder();

  pTpip  = new TH1I("pTpip","",100,0,1.1);
  pTpim  = new TH1I("pTpim","",100,0,1.1);
  pTpi0  = new TH1I("pTpi0","",100,0,1.1);
  phiPip = new TH1I("phiPip","",100,-180,180);
  phiPim = new TH1I("phiPim","",100,-180,180);
  phiPi0 = new TH1I("phiPi0","",100,-180,180);
  pTzPip = new TH2I("pTzPip","",100,0.0,1.1,100,0.0,1.1);
  pTzPim = new TH2I("pTzPim","",100,0.0,1.1,100,0.0,1.1);
  pTzPi0 = new TH2I("pTzPi0","",100,0.0,1.1,100,0.0,1.1);
  xQQ    = new TH2I("xQQ","",100,0.0,0.7,100,0.5,4.5);
  wQQ    = new TH2I("wQQ","",100,0.5,3.0,100,0.5,4.5);

  cout << "[MyAnalysis::Init] Initialization done" << endl;
}

// Example overwriting the event action 
void MyAnalysis::ProcessEvent(){  
  std::vector<TLorentzVector> pims, pips, pi0s, electrons; 
  electrons = filter->getVectorOfTLorentzVectors(event,   11);
  pims      = filter->getVectorOfTLorentzVectors(event, -211);
  pips      = filter->getVectorOfTLorentzVectors(event,  211);
  pi0s      = filter->getVectorOfTLorentzVectors(event,  111);

  if (electrons.size()>0){
    PhysicsEvent elEvent = builder->getPhysicsEvent(electrons[0]);
    xQQ->Fill(elEvent.x, elEvent.qq);
    wQQ->Fill(elEvent.w, elEvent.qq);
  }

  if( pims.size()>0 && electrons.size()>0) {
    PhysicsEvent pimEvent = builder->getPhysicsEvent(electrons[0], pims[0]);    
    pTpim  ->Fill(pimEvent.pT);
    phiPim ->Fill(pimEvent.phiHadron);
    pTzPim ->Fill(pimEvent.z, pimEvent.pT*pimEvent.pT); 
  } 
  
  if( pips.size()>0 && electrons.size()>0) {
    PhysicsEvent pipEvent = builder->getPhysicsEvent(electrons[0], pips[0]);    
    pTpip  ->Fill(pipEvent.pT);
    phiPip ->Fill(pipEvent.phiHadron);
    pTzPip ->Fill(pipEvent.z, pipEvent.pT*pipEvent.pT); 
  } 
  
  if( pi0s.size()>0 && electrons.size()>0) {
    PhysicsEvent pi0Event = builder->getPhysicsEvent(electrons[0], pi0s[0]);    
    pTpi0  ->Fill(pi0Event.pT);
    phiPi0 ->Fill(pi0Event.phiHadron);
    pTzPi0 ->Fill(pi0Event.z, pi0Event.pT*pi0Event.pT); 
  } 

}

void MyAnalysis::Display(){
  TCanvas *display = new TCanvas("display","",1000,500);
  display->Divide(3,2);

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  display->cd(1);
  pTpip->SetLineColor(95);
  pTpim->SetLineColor(65);
  pTpi0->SetLineColor(75);
  pTpip->Draw();
  pTpim->Draw("same");
  pTpi0->Draw("same");

  display->cd(2);
  phiPip->SetLineColor(95);
  phiPim->SetLineColor(65);
  phiPi0->SetLineColor(75);
  phiPip->Draw();
  phiPim->Draw("same");
  phiPi0->Draw("same");

  display->cd(3);
  gPad->SetLogz();
  pTzPip->Draw("colz");

  display->cd(4);
  gPad->SetLogz();
  pTzPim->Draw("colz");

  display->cd(5);
  gPad->SetLogz();
  pTzPi0->Draw("colz");
  


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
    TApplication *app = new TApplication("app", &argc, argv); 

    for (int i=0; i<options->ifiles.size(); i++) { analysis.AddFile(options->ifiles[i]); } 
    analysis.RunAnalysis(options->args["N"].arg);
    analysis.Display();
    

    app->Run();
  }

  else {
    cout << "[MyAnalysis::Main] No files added to options->ifiles " << endl; 
  }


  return 0;
}
