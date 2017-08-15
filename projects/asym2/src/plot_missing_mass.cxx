#include <iostream>


// this proj
#include "common/Config.h"
#include "common/ConfigLoader.h"
#include "common/ConfigLoader.cxx"
#include "common/MissingMassHistograms.h"
#include "common/Types.h"

// h22 
#include "h22Option.h"

// root 
#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TLatex.h"
#include "TLine.h"
#include "TStyle.h"

int main(int argc, char *argv[]){

  // setup options 
  h22Options opts; 
  opts.args["input_file"].args = "unset"; 
  opts.args["input_file"].name = "file containing root histos"; 
  opts.args["input_file"].type = 1;

  opts.args["config"].args = "unset";   
  opts.args["config"].name = "configuration file"; 
  opts.args["config"].type = 1;

  opts.args["output_path"].args = "/volatile/clas12/dmriser/plots/asymmetry";   
  opts.args["ouptut_path"].name = "plot destination"; 
  opts.args["output_path"].type = 1;

  opts.set(argc, argv); 

  std::string configFile = opts.args["config"].args; 
  std::string inputFile  = opts.args["input_file"].args; 
  std::string outputPath = opts.args["output_path"].args; 

  // stying opts 
  const int TEXT_FONT = 42; 
  
  if(configFile != "unset" && 
     inputFile != "unset"){

    // getting from json document 
    Config config = ConfigLoader::getConfiguration(configFile);

    // business can begin 
    std::map<int, MissingMassHistos*> histos; 
    histos[pass::all] = new MissingMassHistos(inputFile, "PassALL");
    histos[pass::mesonID] = new MissingMassHistos(inputFile, "PassMesonID");
    histos[pass::SIDIS] = new MissingMassHistos(inputFile, "PassSIDIS");

    // start plotting the missing mass 
    TCanvas *can = new TCanvas("can","",1200,900); 
    can->cd(); 
    can->SetBottomMargin(0.12); 

    gStyle->SetOptStat(0);
    gStyle->SetOptFit(0);
    gStyle->SetOptTitle(0);
    //    gStyle->SetMargin(0.1, 0.1, 0.1, 0.1); 

    // setup writer 
    TLatex *title = new TLatex(); 
    title->SetNDC(); 
    title->SetTextFont(TEXT_FONT); 
    title->SetTextSize(0.03); 

    // cut bounts 
    float max = histos[pass::mesonID]->h1_zoom->GetMaximum(); 

    TLine *leftCut = new TLine(config.cuts["missing_mass"]["min"], 0, 
			       config.cuts["missing_mass"]["min"], max); 
    TLine *rightCut = new TLine(config.cuts["missing_mass"]["max"], 0, 
			       config.cuts["missing_mass"]["max"], max); 

    leftCut->SetLineStyle(8); 
    leftCut->SetLineWidth(2); 

    rightCut->SetLineStyle(8); 
    rightCut->SetLineWidth(2); 

    histos[pass::mesonID]->h1_zoom->SetLineColor(99); 
    histos[pass::mesonID]->h1_zoom->Draw(); 
    histos[pass::SIDIS]->h1_zoom->SetLineColor(55); 
    histos[pass::SIDIS]->h1_zoom->Draw("same"); 

    leftCut ->Draw(); 
    rightCut->Draw(); 

    title->DrawLatex(0.48, 0.05, "M_{X} (GeV/c^{2})"); 
    title->DrawLatex(0.2, 0.92, "Cuts applied: #color[99]{Meson ID}, #color[55]{Meson ID + SIDIS}"); 
    can->Print(Form("%s/missing_mass_zoom_%s.pdf",outputPath.c_str(),config.name.c_str()));

    histos[pass::mesonID]->h1_wide->SetLineColor(99); 
    histos[pass::mesonID]->h1_wide->Draw(); 
    histos[pass::SIDIS]->h1_wide->SetLineColor(55); 
    histos[pass::SIDIS]->h1_wide->Draw("same"); 

    title->DrawLatex(0.48, 0.05, "M_{X} (GeV/c^{2})"); 
    title->DrawLatex(0.2, 0.92, "Cuts applied: #color[99]{Meson ID}, #color[55]{Meson ID + SIDIS}"); 
    can->Print(Form("%s/missing_mass_wide_%s.pdf",outputPath.c_str(),config.name.c_str()));

  } else {
    std::cerr << "Please set the following options: -input_file=x.json, -config=x.root" << std::endl; 
  }

  return 0;
}
