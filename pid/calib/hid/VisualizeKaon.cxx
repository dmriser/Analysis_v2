
/*

  David Riser 
  VisualizeKaon.cxx

  Draw histograms for 
  Kaon ID. 

  April 16, 2017 

 */

#include <iostream>
#include <vector>

#include "h22Option.h"
#include "MesonHistograms.h"
#include "Parameters.h"
#include "StandardHistograms.h"

#include "TCanvas.h"
#include "TColor.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TLatex.h"
#include "TStyle.h"

int PrintUsage(); 

class KaonVisualization {
public:
  KaonVisualization(){
  }

  ~KaonVisualization(){
  }

  void Configure(int argc, char *argv[]){
    opts.args["INPUT"].args = "UNSET";
    opts.args["INPUT"].type = 1;
    opts.args["INPUT"].name = "Input file";

    opts.args["PARS"].args = "/u/home/dmriser/Analysis_v2/pid/calib/hid/hid_run.pars";
    opts.args["PARS"].type = 1;
    opts.args["PARS"].name = "Parameter file";

    opts.args["OUT"].args = "/volatile/clas12/dmriser/plots/pid/kp/";
    opts.args["OUT"].type = 1;
    opts.args["OUT"].name = "Output folder";

    opts.set(argc, argv);
    
    noCutsHistos = new MesonHistograms("allPositive",321); 
    dvzCutHistos = new MesonHistograms("dvzCut",321); 
    dcFidCutHistos = new MesonHistograms("dcFidCut",321); 
    massCutHistos = new MesonHistograms("massCut",321); 
    allCutHistos = new MesonHistograms("allCut",321); 
    kaonEvents = new StandardHistograms("kaonEvents",1); 

    std::string input = opts.args["INPUT"].args;
    if(input != "UNSET"){
      noCutsHistos->Load(input);
      dvzCutHistos->Load(input);
      dcFidCutHistos->Load(input);
      massCutHistos->Load(input);
      allCutHistos->Load(input);
      kaonEvents->Load(input); 

      for(int s=1; s<7; s++){
	std::cout << "[KaonVisualization] Loaded Sector " << s << " with Entries = " << noCutsHistos->h1_p[s]->GetEntries() << std::endl;  
      }

    }
    else {
      std::cerr << "[KaonVisualization::Configure] Input file not set." << std::endl; 
      exit(0);
    }

    pars.loadParameters(opts.args["PARS"].args); 

    std::cout << "[KaonVisualization::Configure] Finished configuration successfully. " << std::endl;  
  }

  void Process(){
    std::string outputPath = opts.args["OUT"].args; 

    gStyle->SetOptStat(0); 

    const static UInt_t rNCOLORS = 5;

    Double_t rRed[rNCOLORS]     = {0.00, 0.00, 0.87, 1.00, 0.51};
    Double_t rGreen[rNCOLORS]     = {0.00, 0.81, 1.00, 0.20, 0.00};
    Double_t rBlue[rNCOLORS]     = {0.51, 1.00, 0.12, 0.00, 0.00};
    Double_t rStops[rNCOLORS] = {0.00, 0.34, 0.61, 0.84, 1.00};
    UInt_t rNSTEPS = 255;

    TColor::CreateGradientColorTable(rNCOLORS, rStops, rRed, rGreen, rBlue, rNSTEPS);
    gStyle->SetNumberContours(rNSTEPS);

    TCanvas *singleCanvas = new TCanvas("singleCanvas","",800,500);
    noCutsHistos->h2_p_beta[0]->Draw("colz");
    gPad->SetLogz(); 
    singleCanvas->Print(Form("%sBetaPNoCuts.png",outputPath.c_str()));

  }

private:
  h22Options          opts; 
  Parameters          pars; 
  MesonHistograms    *noCutsHistos; 
  MesonHistograms    *dvzCutHistos; 
  MesonHistograms    *dcFidCutHistos; 
  MesonHistograms    *massCutHistos; 
  MesonHistograms    *allCutHistos; 
  StandardHistograms *kaonEvents; 
};

int main(int argc, char *argv[]){

  if (argc < 2){
    return PrintUsage();
  }
  
  KaonVisualization kViz; 
  kViz.Configure(argc, argv); 
  kViz.Process();

  return 0;
}

int PrintUsage(){

  std::cout << "Please pass in the file containing MesonHistograms created by kaon.cxx" << std::endl; 
  std::cout << "with the flag ./VisualizeKaon -INPUT=kaon.root -PARS=kaon.pars -OUT=images/" << std::endl; 

  return -1;
}
