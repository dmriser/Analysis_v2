#ifndef plotter_h
#define plotter_h

#include <iostream>

#include "Bins.h"
#include "Constants.h"
#include "Histograms.h"
#include "Fits.h"
#include "Types.h"

#include "TCanvas.h"
#include "TF1.h"
#include "TH1.h"
#include "TLatex.h"
#include "TPad.h"
#include "TGaxis.h"

class Plotter {
 public:
 Plotter(Histos *h, Fits *f, int index) : fHistos(h), fFits(f), fMesonIndex(index) {
    fOutputPath = "/volatile/clas12/dmriser/plots/asymmetry/"; 
    fBins       = Bins::GetInstance(fMesonIndex);
  }

  void SetOutputPath(std::string p){
    fOutputPath = p; 
  }

  /*
  void PlotX(){

    TCanvas *can = new TCanvas("can","",1200,900); 
    
    gPad->SetMargin(0.15, 0.15, 0.15, 0.15); 
    gStyle->SetOptStat(0); 
    gStyle->SetErrorX(0.00); 
    gStyle->SetLabelSize(0.00,"y");
    gStyle->SetTitleSize(0.0,"y");
    gStyle->SetTitleFont(22,"xyz");
    gStyle->SetLabelFont(22,"xyz");
    gStyle->SetTitleOffset(1.2,"y");

    gStyle->SetCanvasColor(0);
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetCanvasBorderSize(0);
    can->cd(); 

    fHistos->h1_x_kp->SetFillColor(99);
    fHistos->h1_x_kp->SetLineColor(99);
    fHistos->h1_x_kp->SetMarkerColor(99);
    fHistos->h1_x_kp->SetMarkerStyle(24);
    fHistos->h1_x_kp->SetMinimum(-0.06);
    fHistos->h1_x_kp->SetMaximum(0.06);
    fHistos->h1_x_kp->Draw("pe");

    fHistos->h1_x_km->SetFillColor(55);
    fHistos->h1_x_km->SetLineColor(55);
    fHistos->h1_x_km->SetMarkerColor(55);
    fHistos->h1_x_km->SetMarkerStyle(24);
    fHistos->h1_x_km->Draw("pesame");

    TLatex title; 
    title.SetNDC();
    title.SetTextSize(0.03); 
    title.DrawLatex(0.4, 0.95, "Asymmetry #color[99]{K^{+}}, #color[55]{K^{-}}"); 

    title.DrawLatex(0.47, 0.05, "x_{Bjorken}"); 
    
    title.SetTextAngle(90.0);
    title.DrawLatex(0.07, 0.47, "A_{LU}^{sin(#phi)}"); 

    can->Print(Form("%sx.png",fOutputPath.c_str())); 
  }

  void PlotZ(){

    TCanvas *can = new TCanvas("can","",1200,900); 
    
    gPad->SetMargin(0.15, 0.15, 0.15, 0.15); 
    gStyle->SetOptStat(0); 
    gStyle->SetErrorX(0.00); 
    gStyle->SetLabelSize(0.00,"y");
    gStyle->SetTitleSize(0.0,"y");
    gStyle->SetTitleFont(22,"xyz");
    gStyle->SetLabelFont(22,"xyz");
    gStyle->SetTitleOffset(1.2,"y");

    gStyle->SetCanvasColor(0);
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetCanvasBorderSize(0);
    can->cd(); 

    fHistos->h1_z_kp->SetFillColor(99);
    fHistos->h1_z_kp->SetLineColor(99);
    fHistos->h1_z_kp->SetMarkerColor(99);
    fHistos->h1_z_kp->SetMarkerStyle(24);
    fHistos->h1_z_kp->SetMinimum(-0.06);
    fHistos->h1_z_kp->SetMaximum(0.06);
    fHistos->h1_z_kp->Draw("pe");

    fHistos->h1_z_km->SetFillColor(55);
    fHistos->h1_z_km->SetLineColor(55);
    fHistos->h1_z_km->SetMarkerColor(55);
    fHistos->h1_z_km->SetMarkerStyle(24);
    fHistos->h1_z_km->Draw("pesame");

    TLatex title; 
    title.SetNDC();
    title.SetTextSize(0.03); 
    title.DrawLatex(0.4, 0.95, "Asymmetry #color[99]{K^{+}}, #color[55]{K^{-}}"); 

    title.DrawLatex(0.47, 0.05, "z"); 
    
    title.SetTextAngle(90.0);
    title.DrawLatex(0.07, 0.47, "A_{LU}^{sin(#phi)}"); 

    can->Print(Form("%sz.png",fOutputPath.c_str())); 
  }

  void PlotPt(){

    TCanvas *can = new TCanvas("can","",1200,900); 
    
    gPad->SetMargin(0.15, 0.15, 0.15, 0.15); 
    gStyle->SetOptStat(0); 
    gStyle->SetErrorX(0.00); 
    gStyle->SetLabelSize(0.00,"y");
    gStyle->SetTitleSize(0.0,"y");
    gStyle->SetTitleFont(22,"xyz");
    gStyle->SetLabelFont(22,"xyz");
    gStyle->SetTitleOffset(1.2,"y");

    gStyle->SetCanvasColor(0);
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetCanvasBorderSize(0);
    can->cd(); 

    fHistos->h1_pt_kp->SetFillColor(99);
    fHistos->h1_pt_kp->SetLineColor(99);
    fHistos->h1_pt_kp->SetMarkerColor(99);
    fHistos->h1_pt_kp->SetMarkerStyle(24);
    fHistos->h1_pt_kp->SetMinimum(-0.06);
    fHistos->h1_pt_kp->SetMaximum(0.06);
    fHistos->h1_pt_kp->Draw("pe");

    fHistos->h1_pt_km->SetFillColor(55);
    fHistos->h1_pt_km->SetLineColor(55);
    fHistos->h1_pt_km->SetMarkerColor(55);
    fHistos->h1_pt_km->SetMarkerStyle(24);
    fHistos->h1_pt_km->Draw("pesame");

    TLatex title; 
    title.SetNDC();
    title.SetTextSize(0.03); 
    title.DrawLatex(0.4, 0.95, "Asymmetry #color[99]{K^{+}}, #color[55]{K^{-}}"); 

    title.DrawLatex(0.47, 0.05, "P_{T}"); 
    
    title.SetTextAngle(90.0);
    title.DrawLatex(0.07, 0.47, "A_{LU}^{sin(#phi)}"); 

    can->Print(Form("%spt.png",fOutputPath.c_str())); 

  }
  */

  void PlotGridZX(){

    TCanvas *can = new TCanvas("can","",1200,900); 

    gStyle->SetOptStat(0); 
    gStyle->SetErrorX(0.00); 
    gStyle->SetLabelSize(0.00,"y");
    gStyle->SetTitleSize(0.0,"y");
    gStyle->SetTitleFont(22,"xyz");
    gStyle->SetLabelFont(22,"xyz");
    gStyle->SetTitleOffset(1.2,"y");

    gStyle->SetCanvasColor(0);
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetCanvasBorderSize(0);
    can->cd(); 
 
    TGaxis *zAxis = new TGaxis(0.32, 0.1, 0.9, 0.1, fBins->GetZBins()->GetMin(), fBins->GetZBins()->GetMax(), 5, "");
    zAxis->SetTitle("z");
    zAxis->SetLabelSize(0.03);
    zAxis->Draw();

    TGaxis *xAxis = new TGaxis(0.1, 0.32, 0.1, 0.9, fBins->GetXBins()->GetMin(), fBins->GetXBins()->GetMax(), 5, "");
    xAxis->SetTitle("x");
    xAxis->SetLabelSize(0.03);
    xAxis->Draw();    

    TPad *pads[constants::MAX_BINS_Z][constants::MAX_BINS_X];
    
    int nx = fBins->GetXBins()->GetNumber(); 
    int nz = fBins->GetZBins()->GetNumber(); 

    double padding = 0.01; 
    double xstop   = 0.9; 
    double xstart  = 0.18; 
    double ystop   = 0.9; 
    double ystart  = 0.18; 
    double xwidth  = ((xstop-xstart) - (nz-1)*padding)/(1+nz); 
    double ywidth  = ((ystop-ystart) - (nx-1)*padding)/(1+nx); 

    for (int x=0; x<nz+1; x++){
      for (int y=0; y<nx+1; y++){
	std::string padTitle(Form("pad_%d%d",x,y)); 
	
	double padStartX = xstart + (x)*xwidth + (x)*padding; 
	double padStopX  = padStartX + xwidth; 
	
	double padStartY = ystart + (y)*ywidth + (y)*padding; 
	double padStopY  = padStartY + ywidth; 

	can->cd(); 
	pads[y][x] = new TPad(padTitle.c_str(), padTitle.c_str(), padStartX, padStartY, padStopX, padStopY); 
	pads[y][x]->Draw(); 
	pads[y][x]->cd(); 
	pads[y][x]->SetGrid(); 
	pads[y][x]->SetMargin(0.01, 0.01, 0.01, 0.01);

	int color = kBlack; 
	if (x == 0)          { color = 99; }
	else if (y == 0)     { color = 55; }
	if (x == 0 && y == 0){ color = 77; }
	fHistos->h1_asym[0][y][x][0]->SetLineColor(color);
 	fHistos->h1_asym[0][y][x][0]->SetLineWidth(2);
	fHistos->h1_asym[0][y][x][0]->SetMarkerColor(color);
	fHistos->h1_asym[0][y][x][0]->SetMarkerStyle(24);
	fHistos->h1_asym[0][y][x][0]->SetMarkerSize(1);
	fHistos->h1_asym[0][y][x][0]->SetMinimum(-0.1);
	fHistos->h1_asym[0][y][x][0]->SetMaximum(0.1);

	fHistos->h1_asym[0][y][x][0]->Draw("pe");

	fFits->fit_asym[0][y][x][0]->SetLineColor(color);
	fFits->fit_asym[0][y][x][0]->Draw("same");
      }
    }

    can->cd(); 

    TLatex title; 
    title.SetNDC();
    title.SetTextSize(0.03); 
    title.DrawLatex(0.4, 0.95, Form("%s Asymmetry", constants::Names::latex[fMesonIndex].c_str())); 

    title.SetTextSize(0.02); 
    title.DrawLatex(0.05, 0.12, "#color[99]{Integrated z}"); 
    title.DrawLatex(0.05, 0.09, "#color[55]{Integrated x}"); 
    title.DrawLatex(0.05, 0.06, "#color[77]{Integrated both}"); 
    
    can->Print(Form("%sgrid_z_x_%s.png",fOutputPath.c_str(), constants::Names::mesons[fMesonIndex].c_str())); 
  }
  /*
  void PlotGridPtZKp(){

    TCanvas *can = new TCanvas("can","",1200,900); 

    gStyle->SetOptStat(0); 
    gStyle->SetErrorX(0.00); 
    gStyle->SetLabelSize(0.00,"y");
    gStyle->SetTitleSize(0.0,"y");
    gStyle->SetTitleFont(22,"xyz");
    gStyle->SetLabelFont(22,"xyz");
    gStyle->SetTitleOffset(1.2,"y");

    gStyle->SetCanvasColor(0);
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetCanvasBorderSize(0);
    can->cd(); 
 
    TGaxis *xAxis = new TGaxis(0.32, 0.1, 0.9, 0.1, fHistos->ptbins[1].GetMin(), fHistos->ptbins[1].GetMax(), 6, "");
    xAxis->SetTitle("P_{T}");
    xAxis->SetLabelSize(0.03);
    xAxis->Draw();

    TGaxis *zAxis = new TGaxis(0.1, 0.32, 0.1, 0.9, fHistos->zbins[1].GetMin(), fHistos->zbins[1].GetMax(), 8, "");
    zAxis->SetTitle("z");
    zAxis->SetLabelSize(0.03);
    zAxis->Draw();    

    TPad *pads[fHistos->npt_kp+1][fHistos->nz_kp+1];
    
    double padding = 0.01; 
    double xstop = 0.9; 
    double xstart = 0.18; 
    double ystop = 0.9; 
    double ystart = 0.18; 
    double xwidth = ((xstop-xstart) - (fHistos->npt_kp-1)*padding)/(1+fHistos->npt_kp); 
    double ywidth = ((ystop-ystart) - (fHistos->nz_kp-1)*padding)/(1+fHistos->nz_kp); 


    for (int x=0; x<fHistos->npt_kp+1; x++){
      for (int y=0; y<fHistos->nz_kp+1; y++){
	std::string padTitle(Form("pad_%d%d",x,y)); 
	
	double padStartX = xstart + (x)*xwidth + (x)*padding; 
	double padStopX  = padStartX + xwidth; 
	
	double padStartY = ystart + (y)*ywidth + (y)*padding; 
	double padStopY  = padStartY + ywidth; 

	can->cd(); 
	pads[x][y] = new TPad(padTitle.c_str(), padTitle.c_str(), padStartX, padStartY, padStopX, padStopY); 
	pads[x][y]->Draw(); 
	pads[x][y]->cd(); 
	pads[x][y]->SetGrid(); 
	pads[x][y]->SetMargin(0.01, 0.01, 0.01, 0.01);

	int color = kBlack; 
	if (x == 0){ color = 99; }
	else if (y == 0){ color = 55; }
	if (x == 0 && y == 0){ color = 77; }
	fHistos->h1_asym_kp[0][0][y][x]->SetLineColor(color);
	fHistos->h1_asym_kp[0][0][y][x]->SetLineWidth(2);
	fHistos->h1_asym_kp[0][0][y][x]->SetMarkerColor(color);
	fHistos->h1_asym_kp[0][0][y][x]->SetMarkerStyle(24);
	fHistos->h1_asym_kp[0][0][y][x]->SetMarkerSize(1);
	fHistos->h1_asym_kp[0][0][y][x]->Draw("pe");

	fHistos->fit_kp[0][0][y][x]->SetLineColor(color);
	fHistos->fit_kp[0][0][y][x]->Draw("same");
      }
    }

    can->cd(); 
 
    TLatex title; 
    title.SetNDC();
    title.SetTextSize(0.03); 
    title.DrawLatex(0.4, 0.95, "K^{+} Asymmetry"); 

    title.SetTextSize(0.02); 
    title.DrawLatex(0.05, 0.12, "#color[99]{Integrated P_{T}}"); 
    title.DrawLatex(0.05, 0.09, "#color[55]{Integrated z}"); 
    title.DrawLatex(0.05, 0.06, "#color[77]{Integrated both}"); 
    
    can->Print(Form("%sgrid_pt_z_kp.png",fOutputPath.c_str())); 
  }
  */
  
 protected:
  Bins          *fBins; 
  Histos        *fHistos; 
  Fits          *fFits;
  std::string    fOutputPath; 
  int            fMesonIndex; 
  

};

#endif
