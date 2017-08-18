#ifndef plotting_h 
#define plotting_h 

#include <iostream>
#include <vector>
#include <map>

// this project 
#include "PhiFits.h"
#include "PhiHistograms.h"
#include "MissingMassHistograms.h"
#include "Config.h"
#include "ConfigLoader.h"
#include "ConfigLoader.cxx"
#include "Types.h"

// h22 libraries 
#include "DBins.h"

// root 
#include "TCanvas.h"
#include "TF1.h"
#include "TH1.h"
#include "TLatex.h"
#include "TLine.h"
#include "TStyle.h"

class Plotting {

public:

  static void Setup(){
    gStyle->SetOptFit(0);
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
  }

  static void ApplyStyle(TH1F *h, int style){

    if (style == 0){
      h->SetMarkerColor(kBlack);
      h->SetMarkerStyle(8);
      h->SetLineColor(kBlack); 
      h->SetLineWidth(1); 
    }

    else if (style == 1){
      h->SetFillColorAlpha(99, 0.3);
    }

  }

  static void PlotOnePhiHisto(PhiHistos *h, 
			      std::string name, 
			      std::string x, 
			      std::string y,
			      std::string options, 
			      int plotStyle,
			      std::string pdf){

    // retrieve binning from histograms 
    std::map<int, DLineBins> bins = h->GetBins();
    
    // setup a reasonably sized canvas 
    TCanvas *canvas = new TCanvas("canvas","",1600,1200);
    canvas->Divide(4,4);
    canvas->Print(Form("%s[",pdf.c_str()));

    // setup a printer 
    TLatex *title = new TLatex();
    title->SetNDC();
    title->SetTextFont(42);
    title->SetTextSize(0.05);

    int index = 1;

    for(std::pair<int, DLineBins> p : bins){
      int currentAxis       = p.first; 
      DLineBins currentBins = p.second; 


      if (currentAxis != axis::phi){
	for(int b=0; b<currentBins.GetNumber(); b++){
	  canvas->cd(index);
	  canvas->SetMargin(0.2, 0.2, 0.2, 0.2);

	  ApplyStyle(h->histos[currentAxis][b], plotStyle);
	  h->histos[currentAxis][b]->Draw(options.c_str());
	  index++; 
	  

	  title->DrawLatex(0.4, 0.01, x.c_str());
	  title->SetTextAngle(90.0);
	  title->DrawLatex(0.01, 0.4, y.c_str());
	  title->SetTextAngle(0.0);
	  title->DrawLatex(0.14, 0.80, Form("%s #epsilon [%.2f,%.2f]", 
					    axisNames[currentAxis].c_str(), 
					    currentBins.GetBin(b).GetMin(), 
					    currentBins.GetBin(b).GetMax()));
	  
	  // we need to print a page 
	  if (index > 16){
	    index = 1; 
	    canvas->Print(pdf.c_str());
	    canvas->Clear(); 
	    canvas->Divide(4,4); 
	  }

	}
      }
    }

    canvas->Print(pdf.c_str());
    canvas->Print(Form("%s]",pdf.c_str()));
  }

  static void PlotTwoPhiHistos(PhiHistos *h1,
			       PhiHistos *h2,
			       std::string name, 
			       std::string x, 
			       std::string y,
			       std::string options1,
			       std::string options2,
			       int plotStyle1, 
			       int plotStyle2,
			       std::string pdf){
    
    // retrieve binning from histograms 
    std::map<int, DLineBins> bins = h1->GetBins();
    
    // setup a reasonably sized canvas 
    TCanvas *canvas = new TCanvas("canvas","",1600,1200);
    canvas->Divide(4,4);
    canvas->Print(Form("%s[",pdf.c_str()));

    // setup a printer 
    TLatex *title = new TLatex();
    title->SetNDC();
    title->SetTextFont(42);
    title->SetTextSize(0.05);

    int index = 1;

    for(std::pair<int, DLineBins> p : bins){
      int currentAxis       = p.first; 
      DLineBins currentBins = p.second; 


      if (currentAxis != axis::phi){
	for(int b=0; b<currentBins.GetNumber(); b++){
	  canvas->cd(index);
	  canvas->SetMargin(0.2, 0.2, 0.2, 0.2);

	  ApplyStyle(h1->histos[currentAxis][b], plotStyle1);
	  ApplyStyle(h2->histos[currentAxis][b], plotStyle2);
	  h1->histos[currentAxis][b]->Draw(options1.c_str());
	  h2->histos[currentAxis][b]->Draw(options2.c_str());
	  index++; 
	  

	  title->DrawLatex(0.4, 0.01, x.c_str());
	  title->SetTextAngle(90.0);
	  title->DrawLatex(0.01, 0.4, y.c_str());
	  title->SetTextAngle(0.0);
	  title->DrawLatex(0.14, 0.80, Form("%s #epsilon [%.2f,%.2f]", 
					    axisNames[currentAxis].c_str(), 
					    currentBins.GetBin(b).GetMin(), 
					    currentBins.GetBin(b).GetMax()));
	  
	  // we need to print a page 
	  if (index > 16){
	    index = 1; 
	    canvas->Print(pdf.c_str());
	    canvas->Clear(); 
	    canvas->Divide(4,4); 
	  }

	}
      }
    }

    canvas->Print(pdf.c_str());
    canvas->Print(Form("%s]",pdf.c_str()));
  }

};

#endif 
