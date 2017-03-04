#ifndef printer_cxx
#define printer_cxx

// -------------------------
//         c++ 
// -------------------------
#include <iostream>
using std::cout; 
using std::endl; 
using std::string; 

// -------------------------
//         root 
// -------------------------
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TStyle.h"
// -------------------------
//         this
// -------------------------
#include "Printer.h"
#include "Style.h"
#include "Tools.h"

namespace SimplePlot{
  
  Printer::Printer(){
    canvas = new TCanvas("canvas","",800,500);
    setupCommonOptions();
    
    style = new Style(); 
    style->loadStyle("/u/home/dmriser/mydoc/analysis/root_scripts/Analysis_v2/SimplePlot/styles/default.style");
  }
  
  Printer::~Printer(){
  }
  
  void Printer::setupCommonOptions(){
    gStyle->SetOptTitle(0);
    gStyle->SetOptStat(0);
    //    gStyle->SetTitleFont(style->getFontIndex());
  }

  void Printer::print(TH1 *histo, string title, string xTitle, string yTitle){
    canvas->Clear();
    canvas->SetCanvasSize(800,500);
    gPad  ->SetMargin(0.1, 0.1, 0.15, 0.15);

    // Configure fill options 
    if(style->getFillStatus()){
      histo->SetFillColorAlpha(style->getPrimaryColor(),   style->getAlpha());
    }

    // Configure grid status 
    if(style->getGridX()){
      gPad->SetGridx();
    }

    if(style->getGridY()){
      gPad->SetGridy();
    }

    // Setup title printers in latex
    TLatex tit, xtit, ytit;
    tit.SetNDC();
    tit.SetTextSize(0.05);
    tit.SetTextFont(style->getFontIndex());
    xtit.SetNDC();
    xtit.SetTextSize(0.04);
    xtit.SetTextFont(style->getFontIndex());
    ytit.SetNDC();
    ytit.SetTextSize(0.04);
    ytit.SetTextFont(style->getFontIndex());
    ytit.SetTextAngle(90.0);

    histo ->Draw();

    tit.DrawLatex( tools.getTitleCenterPosition(title), 0.9,  title.c_str());
    xtit.DrawLatex(tools.getTitleCenterPosition(title), 0.08, xTitle.c_str());
    ytit.DrawLatex(0.05, tools.getTitleCenterPosition(title), yTitle.c_str());

    canvas->Print(Form("%s%s.png",path.c_str(),outputTitle.c_str()));
  }
  
  void Printer::printCompare(TH1 *histo1, TH1 *histo2, string title, string xTitle, string yTitle){
    canvas->Clear();
    canvas->SetCanvasSize(800,500);
    gPad  ->SetMargin(0.1, 0.1, 0.15, 0.15);

    tools.setIntegralSame(histo1, histo2);
    tools.setMaximumSame(histo1, histo2);

    // Configure fill options 
    if(style->getFillStatus()){
      histo1->SetFillColorAlpha(style->getPrimaryColor(),   style->getAlpha());
      histo2->SetFillColorAlpha(style->getSecondaryColor(), style->getAlpha());
    }

    // Configure grid status 
    if(style->getGridX()){
      gPad->SetGridx();
    }

    if(style->getGridY()){
      gPad->SetGridy();
    }

    // Setup title printers in latex
    TLatex tit, xtit, ytit;
    tit.SetNDC();
    tit.SetTextSize(0.05);
    tit.SetTextFont(style->getFontIndex());
    xtit.SetNDC();
    xtit.SetTextSize(0.04);
    xtit.SetTextFont(style->getFontIndex());
    ytit.SetNDC();
    ytit.SetTextSize(0.04);
    ytit.SetTextFont(style->getFontIndex());
    ytit.SetTextAngle(90.0);

    // Start drawing 
    histo1 ->Draw();
    histo2 ->Draw("same");

    tit.DrawLatex( tools.getTitleCenterPosition(title), 0.9,  title.c_str());
    xtit.DrawLatex(tools.getTitleCenterPosition(title), 0.08, xTitle.c_str());
    ytit.DrawLatex(0.05, tools.getTitleCenterPosition(title), yTitle.c_str());

    canvas->Print(Form("%s%s.png",path.c_str(),outputTitle.c_str()));
  }

  void Printer::printSectors(TH1D *histo[], string title, string xTitle, string yTitle){
    canvas->Clear();
    canvas->SetCanvasSize(1200,800);
    canvas->Divide(3,2); 

    // Setup title printers in latex
    TLatex tit, xtit, ytit;
    tit.SetNDC();
    tit.SetTextSize(0.05);
    tit.SetTextFont(style->getFontIndex());
    xtit.SetNDC();
    xtit.SetTextSize(0.04);
    xtit.SetTextFont(style->getFontIndex());
    ytit.SetNDC();
    ytit.SetTextSize(0.04);
    ytit.SetTextFont(style->getFontIndex());
    ytit.SetTextAngle(90.0);

    for(int s=0; s<6; s++){
      canvas->cd(s+1);

    // Preparing Canvas 
      if(tools.padIsLeft(s+1, 3, 2)){
	gPad->SetLeftMargin(0.15);
	gPad->SetRightMargin(0.05); 
      } 
      if (tools.padIsRight(s+1, 3, 2)){	
	gPad->SetLeftMargin(0.05);
	gPad->SetRightMargin(0.15); 	
      }
      if(tools.padIsBottom(s+1, 3, 2)){
	gPad->SetBottomMargin(0.15);
	gPad->SetTopMargin(0.05); 
      }
      if(tools.padIsTop(s+1, 3, 2)){
	gPad->SetBottomMargin(0.05);
	gPad->SetTopMargin(0.15); 
      }

      // Configure fill options 
      if(style->getFillStatus()){
	histo[s]->SetFillColorAlpha(style->getPrimaryColor(),   style->getAlpha());
      }
      
      // Configure grid status 
      if(style->getGridX()){
	gPad->SetGridx();
      }
      
      if(style->getGridY()){
	gPad->SetGridy();
      }
      
      histo[s]->Draw();
      
      //    tit.DrawLatex( tools.getTitleCenterPosition(title), 0.9,  title.c_str());
      //    xtit.DrawLatex(tools.getTitleCenterPosition(title), 0.08, xTitle.c_str());
      //    ytit.DrawLatex(0.05, tools.getTitleCenterPosition(title), yTitle.c_str());
    }
    
    canvas->Print(Form("%s%s.png",path.c_str(),outputTitle.c_str()));
  }

  void Printer::printSectorsCompare(TH1D *histo1[], TH1D *histo2[], string title, string xTitle, string yTitle){
    canvas->Clear();
    canvas->SetCanvasSize(1200,800);
    canvas->Divide(3,2); 

    // Setup title printers in latex
    TLatex tit, xtit, ytit;
    tit.SetNDC();
    tit.SetTextSize(0.05);
    tit.SetTextFont(style->getFontIndex());
    xtit.SetNDC();
    xtit.SetTextSize(0.04);
    xtit.SetTextFont(style->getFontIndex());
    ytit.SetNDC();
    ytit.SetTextSize(0.04);
    ytit.SetTextFont(style->getFontIndex());
    ytit.SetTextAngle(90.0);

    for(int s=0; s<6; s++){
      canvas->cd(s+1);

      tools.setIntegralSame(histo1[s], histo2[s]);
      tools.setMaximumSame(histo1[s], histo2[s]);

    // Preparing Canvas 
      if(tools.padIsLeft(s+1, 3, 2)){
	gPad->SetLeftMargin(0.15);
	gPad->SetRightMargin(0.05); 
      } 
      if (tools.padIsRight(s+1, 3, 2)){	
	gPad->SetLeftMargin(0.05);
	gPad->SetRightMargin(0.15); 	
      }
      if(tools.padIsBottom(s+1, 3, 2)){
	gPad->SetBottomMargin(0.15);
	gPad->SetTopMargin(0.05); 
      }
      if(tools.padIsTop(s+1, 3, 2)){
	gPad->SetBottomMargin(0.05);
	gPad->SetTopMargin(0.15); 
      }

      // Configure fill options 
      if(style->getFillStatus()){
	histo1[s]->SetFillColorAlpha(style->getPrimaryColor(),   style->getAlpha());
	histo2[s]->SetFillColorAlpha(style->getSecondaryColor(),   style->getAlpha());
      }
      
      // Configure grid status 
      if(style->getGridX()){
	gPad->SetGridx();
      }
      
      if(style->getGridY()){
	gPad->SetGridy();
      }
      
      histo1[s]->Draw();
      histo2[s]->Draw("same");
      
      //    tit.DrawLatex( tools.getTitleCenterPosition(title), 0.9,  title.c_str());
      //    xtit.DrawLatex(tools.getTitleCenterPosition(title), 0.08, xTitle.c_str());
      //    ytit.DrawLatex(0.05, tools.getTitleCenterPosition(title), yTitle.c_str());
    }
    
    canvas->Print(Form("%s%s.png",path.c_str(),outputTitle.c_str()));
  }
  
  void Printer::setStyle(string styleName){
    // This is bad to hardcode.
    string totalPath = Form("/u/home/dmriser/mydoc/analysis/root_scripts/Analysis_v2/SimplePlot/styles/%s.style",styleName.c_str());
    style->loadStyle(totalPath);
  }

}


#endif
