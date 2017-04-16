#ifndef printer_cxx
#define printer_cxx

// -------------------------
//         c++ 
// -------------------------
#include <iostream>
#include <vector>
using std::cout; 
using std::endl; 
using std::string; 
using std::vector; 
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
    
    style = new Style(); 
    style->loadStyle("/u/home/dmriser/Analysis_v2/SimplePlot/styles/default.style");

    setupCommonOptions();
  }
  
  Printer::~Printer(){
  }
  
  void Printer::setupCommonOptions(){
    gStyle->SetOptTitle(0);
    gStyle->SetOptStat(0);

    tit.SetNDC();
    tit.SetTextFont(style->getFontIndex());
    xtit.SetNDC();
    xtit.SetTextFont(style->getFontIndex());
    ytit.SetNDC();
    ytit.SetTextFont(style->getFontIndex());
    ytit.SetTextAngle(90.0);

  }

  //! Index refers to the primary or secondary histogram 
  void Printer::applyStylingOptions(TH1 *histo, int index){

    // Configure fill options 
    if(style->getFillStatus()){
      if(index == 1) { 
	histo->SetFillColorAlpha(style->getPrimaryColor(), style->getAlpha()); 
      }

      if(index == 2) { 
	histo->SetFillColorAlpha(style->getSecondaryColor(),   style->getAlpha()); 
      }
    } else {
      if(index == 1) { 
	histo->SetLineColor(style->getPrimaryColor());
      }

      if(index == 2) { 
	histo->SetLineColor(style->getSecondaryColor());
      }
    }

    // Setup markers, lines, or none
    if(style->getMarkerStatus() && index == 1){ 
      histo->SetMarkerStyle(style->getMarkerStyle());
      histo->SetMarkerSize(style->getMarkerSize());
    }

    if(style->getLineStatus() && index == 2){ 
      histo->SetLineStyle(style->getLineStyle());
      histo->SetLineWidth(style->getLineWidth());
    }

  }

  void Printer::print(TH1 *histo, string title, string xTitle, string yTitle){
    canvas->Clear();
    canvas->SetCanvasSize(800,500);
    canvas->SetFillColor(style->getBackgroundColor());
    gPad  ->SetMargin(0.1, 0.1, 0.15, 0.15);

    setupCommonOptions();

    // Configure grid status 
    if(style->getGridX()){
      gPad->SetGridx();
    }

    if(style->getGridY()){
      gPad->SetGridy();
    }

    applyStylingOptions(histo, 1);

    // Setup draw options 
    string drawOption = "";

    // Setup markers, lines, or none
    if(style->getMarkerStatus()){ 
      drawOption = "pe"; 
    }

    else if(style->getLineStatus()){ 
      drawOption = "C"; 
    }

    // Setup title printers in latex
    tit.SetTextSize(0.05);
    xtit.SetTextSize(0.04);
    ytit.SetTextSize(0.04);

    histo->Draw(drawOption.c_str());

    tit.DrawLatex( tools.getTitleCenterPosition(title, 0.0075), 0.9,  title.c_str());
    xtit.DrawLatex(tools.getTitleCenterPosition(xTitle, 0.0075), 0.08, xTitle.c_str());
    ytit.DrawLatex(0.05, tools.getTitleCenterPosition(yTitle, 0.0075), yTitle.c_str());

    canvas->Print(Form("%s%s.png",path.c_str(),outputTitle.c_str()));
  }
  
  void Printer::printCompare(TH1 *histo1, TH1 *histo2, string title, string xTitle, string yTitle){
    canvas->Clear();
    canvas->SetCanvasSize(800,500);
    canvas->SetFillColor(style->getBackgroundColor());
    gPad  ->SetMargin(0.1, 0.1, 0.15, 0.15);
    setupCommonOptions();
    tools.setIntegralSame(histo1, histo2);
    tools.setMaximumSame(histo1, histo2);

    applyStylingOptions(histo1, 1);
    applyStylingOptions(histo2, 2);

    // Setup draw options 
    string drawOption1 = "";
    string drawOption2 = "same";

    if(style->getMarkerStatus()){ 
      drawOption1 = "pe"; 
    }

    if(style->getLineStatus()){ 
      drawOption2 = "lCsame"; 
    }

    // Configure grid status 
    if(style->getGridX()){
      gPad->SetGridx();
    }

    if(style->getGridY()){
      gPad->SetGridy();
    }

    // Setup title printers in latex
    tit.SetTextSize(0.05);
    xtit.SetTextSize(0.04);
    ytit.SetTextSize(0.04);

    // Start drawing 
    histo1 ->Draw(drawOption1.c_str());
    histo2 ->Draw(drawOption2.c_str());

    tit.DrawLatex( tools.getTitleCenterPosition(title, 0.0075), 0.9,  title.c_str());
    xtit.DrawLatex(tools.getTitleCenterPosition(xTitle, 0.0075), 0.08, xTitle.c_str());
    ytit.DrawLatex(0.05, tools.getTitleCenterPosition(yTitle, 0.0075), yTitle.c_str());

    canvas->Print(Form("%s%s.png",path.c_str(),outputTitle.c_str()));
  }

  void Printer::printSectors(TH1D *histo[], string title, string xTitle, string yTitle){
    canvas->Clear();
    canvas->SetCanvasSize(1200,800);
    canvas->SetFillColor(style->getBackgroundColor());
    canvas->Divide(3,2); 
    setupCommonOptions();

    // Setup title printers in latex
    tit.SetTextSize(0.03);
    xtit.SetTextSize(0.03);
    ytit.SetTextSize(0.03);

    // Prepare Canvas 
    tools.setupCanvasSpacing(canvas, 3, 2, 0.02, 0.15);

    for(int s=0; s<6; s++){
      canvas->cd(s+1);
      
      applyStylingOptions(histo[s], 1);
      
      // Setup draw options 
      string drawOption = "";
      
      if(style->getMarkerStatus()){ 
	drawOption = "pe"; 
      }
      
      if(style->getLineStatus()){ 
	drawOption = "C"; 
      }
    
      // Configure grid status 
    if(style->getGridX()){
      gPad->SetGridx();
    }
    
    if(style->getGridY()){
      gPad->SetGridy();
    }
    
    histo[s]->Draw(drawOption.c_str());
    }
    
    canvas->cd(0);
    xtit.DrawLatex(0.5, 0.02, xTitle.c_str());
    ytit.DrawLatex(0.02, 0.5, yTitle.c_str());
    tit.DrawLatex( tools.getTitleCenterPosition(title, 0.005), 0.95,  title.c_str());    
    canvas->Print(Form("%s%s.png",path.c_str(),outputTitle.c_str()));
  }
  
  void Printer::printSectorsCompare(TH1D *histo1[], TH1D *histo2[], string title, string xTitle, string yTitle){
    canvas->Clear();
    setupCommonOptions();
    canvas->SetCanvasSize(1200,800);
    canvas->SetFillColor(style->getBackgroundColor());
    canvas->Divide(3,2); 

    // Setup title printers in latex
    tit .SetTextSize(0.03);
    xtit.SetTextSize(0.03);
    ytit.SetTextSize(0.03);

    // Prepare Canvas 
    tools.setupCanvasSpacing(canvas, 3, 2, 0.01, 0.15);


    for(int s=0; s<6; s++){
      canvas->cd(s+1);

      tools.setIntegralSame(histo1[s], histo2[s]);
      tools.setMaximumSame(histo1[s], histo2[s]);

      applyStylingOptions(histo1[s], 1);
      applyStylingOptions(histo2[s], 2);
      
      // Configure grid status 
      if(style->getGridX()){
	gPad->SetGridx();
      }
      
      if(style->getGridY()){
	gPad->SetGridy();
      }

    // Setup draw options 
    string drawOption1 = "";
    string drawOption2 = "same";

    if(style->getMarkerStatus()){ 
      drawOption1 = "pe"; 
    }
    
    if(style->getLineStatus()){ 
      drawOption2 = "Csame"; 
    }
          
    histo1[s]->Draw(drawOption1.c_str());
    histo2[s]->Draw(drawOption2.c_str());
    }

    canvas->cd(0);
    xtit.DrawLatex(tools.getTitleCenterPosition(xTitle, 0.001), 0.02, xTitle.c_str());
    ytit.DrawLatex(0.02, tools.getTitleCenterPosition(yTitle, 0.005), yTitle.c_str());
    tit .DrawLatex( tools.getTitleCenterPosition(title, 0.005), 0.95,  title.c_str());    
    canvas->Print(Form("%s%s.png",path.c_str(),outputTitle.c_str()));
  }

  void Printer::printSectorsCompareWithRatio(TH1D *histo1[], TH1D *histo2[], string title, string xTitle, string yTitle){
    canvas->Clear();
    setupCommonOptions();
    canvas->SetCanvasSize(1200,800);
    canvas->SetFillColor(style->getBackgroundColor());
    canvas->Divide(4,3); 

    // Setup title printers in latex
    tit .SetTextSize(0.03);
    xtit.SetTextSize(0.03);
    ytit.SetTextSize(0.03);

    // Prepare Canvas 
    tools.setupCanvasSpacing(canvas, 4, 3, 0.02, 0.2);


    // Setup new histograms for the ratio
    TH1D *ratio[6];

    for(int s=0; s<6; s++){
      canvas->cd(2*s+1);

      tools.setIntegralSame(histo1[s], histo2[s]);
      tools.setMaximumSame(histo1[s], histo2[s]);

      ratio[s] = (TH1D*) histo1[s]->Clone();
      ratio[s]->Divide(histo2[s]);
      ratio[s]->SetMinimum(0.6);
      ratio[s]->SetMaximum(1.4);
      ratio[s]->SetName(Form("ratioTempSector%d",s));
      ratio[s]->SetMarkerStyle(style->getMarkerStyle());
      ratio[s]->SetMarkerSize(style->getMarkerSize());

      applyStylingOptions(histo1[s], 1);
      applyStylingOptions(histo2[s], 2);
      
      // Configure grid status 
      if(style->getGridX()){
	gPad->SetGridx();
      }
      
      if(style->getGridY()){
	gPad->SetGridy();
      }

    // Setup draw options 
    string drawOption1 = "";
    string drawOption2 = "same";

    if(style->getMarkerStatus()){ 
      drawOption1 = "pe"; 
    }
    
    if(style->getLineStatus()){ 
      drawOption2 = "Csame"; 
    }
          
    histo1[s]->Draw(drawOption1.c_str());
    histo2[s]->Draw(drawOption2.c_str());

    // Draw the ratio 
    canvas->cd(2*s+2);
      
    // Configure grid status for this pad too 
    if(style->getGridX()){
      gPad->SetGridx();
    }
    
    if(style->getGridY()){
      gPad->SetGridy();
    }
      
    ratio[s]->Draw("p");
    }

    canvas->cd(0);
    xtit.DrawLatex(tools.getTitleCenterPosition(xTitle, 0.005), 0.02, xTitle.c_str());
    ytit.DrawLatex(0.02, tools.getTitleCenterPosition(yTitle, 0.005), yTitle.c_str());
    tit .DrawLatex( tools.getTitleCenterPosition(title, 0.005), 0.95,  title.c_str());    
    canvas->Print(Form("%s%s.png",path.c_str(),outputTitle.c_str()));
  }

  void Printer::printSectorsCompare(TH1D *histo1[], TH1D *histo2, string title, string xTitle, string yTitle){
    canvas->Clear();
    setupCommonOptions();
    canvas->SetCanvasSize(1200,800);
    canvas->SetFillColor(style->getBackgroundColor());
    canvas->Divide(3,2); 

    // Setup title printers in latex
    tit .SetTextSize(0.03);
    xtit.SetTextSize(0.03);
    ytit.SetTextSize(0.03);

    // Prepare Canvas 
    tools.setupCanvasSpacing(canvas, 3, 2, 0.02, 0.15);

    string drawOption2 = "same";
    if(style->getLineStatus()){ 
      drawOption2 = "Csame"; 
      histo2->SetLineStyle(style->getLineStyle());
      histo2->SetLineWidth(style->getLineWidth());
    }

    applyStylingOptions(histo2, 2);    
    
    for(int s=0; s<6; s++){
      canvas->cd(s+1);
      
      applyStylingOptions(histo1[s], 1);

      // Configure fill options 
      if(style->getFillStatus()){
	histo1[s]->SetFillColorAlpha(style->getPrimaryColor(),   style->getAlpha());
      }
      
      // Configure grid status 
      if(style->getGridX()){
	gPad->SetGridx();
      }
      
      if(style->getGridY()){
	gPad->SetGridy();
      }

      // Setup draw options 
      string drawOption1 = "";
      
      if(style->getMarkerStatus()){ 
	drawOption1 = "pe"; 
      }
      
      histo1[s]->Draw(drawOption1.c_str());
      histo2   ->Draw(drawOption2.c_str());
    }

    canvas->cd(0);
    xtit.DrawLatex(tools.getTitleCenterPosition(xTitle, 0.005), 0.02, xTitle.c_str());
    ytit.DrawLatex(0.02, tools.getTitleCenterPosition(yTitle, 0.005), yTitle.c_str());
    tit .DrawLatex( tools.getTitleCenterPosition(title, 0.005), 0.95,  title.c_str());    
    canvas->Print(Form("%s%s.png",path.c_str(),outputTitle.c_str()));
  }
  
  void Printer::printBins(vector<TH1D*> histos, string title, string xTitle, string yTitle){
    for(int histo=0; histo<histos.size(); histo++){
      setOutputTitle(Form("%s",histos[histo]->GetName()));
      print(histos[histo], Form("%s Bin %d",title.c_str(),histo), xTitle, yTitle);
    }
  }
  
  void Printer::printBinsCompare(vector<TH1D*> histos1, vector<TH1D*> histos2, string title, string xTitle, string yTitle){

    if(histos1.size() == histos2.size()){
      for(int histo=0; histo<histos1.size(); histo++){
	setOutputTitle(Form("%s",histos1[histo]->GetName()));
	printCompare(histos1[histo], histos2[histo], Form("%s Bin %d",title.c_str(),histo), xTitle, yTitle);
      }
    } else {
      cout << "[Printer::printBinsCompare] The vectors are not the same size! " << endl; 
    }

  }
  
  void Printer::printBinsCompare(vector<TH1D*> histos1, TH1D *histo2, string title, string xTitle, string yTitle){

      for(int histo=0; histo<histos1.size(); histo++){
	setOutputTitle(Form("%s",histos1[histo]->GetName()));
	printCompare(histos1[histo], histo2, Form("%s Bin %d",title.c_str(),histo), xTitle, yTitle);
      }
  
  }  
  
  void Printer::setStyle(string styleName){
    // This is bad to hardcode.
    //    string totalPath = Form("/u/home/dmriser/mydoc/analysis/root_scripts/Analysis_v2/SimplePlot/styles/%s.style",styleName.c_str());
    string totalPath = Form("/u/home/dmriser/Analysis_v2/SimplePlot/styles/%s.style",styleName.c_str());
    style->loadStyle(totalPath);
  }

}


#endif
