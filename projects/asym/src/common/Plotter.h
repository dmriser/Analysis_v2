#ifndef plotter_h
#define plotter_h

#include <iostream>

#include "Histos.h"

#include "TCanvas.h"
#include "TF1.h"
#include "TH1.h"
#include "TLatex.h"
#include "TPad.h"
#include "TGaxis.h"

class Plotter {
 public:
 Plotter(AsymmetryHistograms *h) : fHistos(h){
    fOutputPath = "/volatile/clas12/dmriser/plots/asymmetry/"; 
  }

  void SetOutputPath(std::string p){
    fOutputPath = p; 
  }

  void PlotGridXZKp(){

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
 
    TGaxis *xAxis = new TGaxis(0.32, 0.1, 0.9, 0.1, fHistos->xbins[1].GetMin(), fHistos->xbins[1].GetMax(), 6, "");
    xAxis->SetTitle("x");
    xAxis->SetLabelSize(0.03);
    xAxis->Draw();

    TGaxis *zAxis = new TGaxis(0.1, 0.32, 0.1, 0.9, fHistos->zbins[1].GetMin(), fHistos->zbins[1].GetMax(), 8, "");
    zAxis->SetTitle("z");
    zAxis->SetLabelSize(0.03);
    zAxis->Draw();    

    TPad *pads[fHistos->nx_kp+1][fHistos->nz_kp+1];
    
    double padding = 0.01; 
    double xstop = 0.9; 
    double xstart = 0.18; 
    double ystop = 0.9; 
    double ystart = 0.18; 
    double xwidth = ((xstop-xstart) - (fHistos->nx_kp-1)*padding)/(1+fHistos->nx_kp); 
    double ywidth = ((ystop-ystart) - (fHistos->nz_kp-1)*padding)/(1+fHistos->nz_kp); 


    for (int x=0; x<fHistos->nx_kp+1; x++){
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
	fHistos->h1_asym_kp[0][x][y][0]->SetLineColor(color);
	fHistos->h1_asym_kp[0][x][y][0]->SetLineWidth(2);
	fHistos->h1_asym_kp[0][x][y][0]->SetMarkerColor(color);
	fHistos->h1_asym_kp[0][x][y][0]->SetMarkerStyle(24);
	fHistos->h1_asym_kp[0][x][y][0]->SetMarkerSize(1);
	fHistos->h1_asym_kp[0][x][y][0]->Draw("pe");

	fHistos->fit_kp[0][x][y][0]->SetLineColor(color);
	fHistos->fit_kp[0][x][y][0]->Draw("same");
      }
    }

    can->cd(); 

    TLatex title; 
    title.SetNDC();
    title.SetTextSize(0.03); 
    title.DrawLatex(0.4, 0.95, "K^{+} Asymmetry"); 

    title.SetTextSize(0.02); 
    title.DrawLatex(0.05, 0.12, "#color[99]{Integrated x}"); 
    title.DrawLatex(0.05, 0.09, "#color[55]{Integrated z}"); 
    title.DrawLatex(0.05, 0.06, "#color[77]{Integrated both}"); 
    
    can->Print(Form("%sgrid_x_z_kp.png",fOutputPath.c_str())); 


  }

  void PlotGridXZKm(){

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
 
    TGaxis *xAxis = new TGaxis(0.32, 0.1, 0.9, 0.1, fHistos->xbins[0].GetMin(), fHistos->xbins[0].GetMax(), 6, "");
    xAxis->SetTitle("x");
    xAxis->SetLabelSize(0.03);
    xAxis->Draw();

    TGaxis *zAxis = new TGaxis(0.1, 0.32, 0.1, 0.9, fHistos->zbins[0].GetMin(), fHistos->zbins[0].GetMax(), 8, "");
    zAxis->SetTitle("z");
    zAxis->SetLabelSize(0.03);
    zAxis->Draw();    

    TPad *pads[fHistos->nx_km+1][fHistos->nz_km+1];
    
    double padding = 0.01; 
    double xstop = 0.9; 
    double xstart = 0.18; 
    double ystop = 0.9; 
    double ystart = 0.18; 
    double xwidth = ((xstop-xstart) - (fHistos->nx_km-1)*padding)/(1+fHistos->nx_km); 
    double ywidth = ((ystop-ystart) - (fHistos->nz_km-1)*padding)/(1+fHistos->nz_km); 


    for (int x=0; x<fHistos->nx_km+1; x++){
      for (int y=0; y<fHistos->nz_km+1; y++){
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
	fHistos->h1_asym_km[0][x][y][0]->SetLineColor(color);
	fHistos->h1_asym_km[0][x][y][0]->SetLineWidth(2);
	fHistos->h1_asym_km[0][x][y][0]->SetMarkerColor(color);
	fHistos->h1_asym_km[0][x][y][0]->SetMarkerStyle(24);
	fHistos->h1_asym_km[0][x][y][0]->SetMarkerSize(1);
	fHistos->h1_asym_km[0][x][y][0]->Draw("pe");

	fHistos->fit_km[0][x][y][0]->SetLineColor(color);
	fHistos->fit_km[0][x][y][0]->Draw("same");
      }
    }

    can->cd(); 

    TLatex title; 
    title.SetNDC();
    title.SetTextSize(0.03); 
    title.DrawLatex(0.4, 0.95, "K^{-} Asymmetry"); 

    title.SetTextSize(0.02); 
    title.DrawLatex(0.05, 0.12, "#color[99]{Integrated x}"); 
    title.DrawLatex(0.05, 0.09, "#color[55]{Integrated z}"); 
    title.DrawLatex(0.05, 0.06, "#color[77]{Integrated both}"); 
    
    can->Print(Form("%sgrid_x_z_km.png",fOutputPath.c_str())); 


  }

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

  void PlotGridPtZKm(){

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
 
    TGaxis *xAxis = new TGaxis(0.32, 0.1, 0.9, 0.1, fHistos->ptbins[0].GetMin(), fHistos->ptbins[0].GetMax(), 6, "");
    xAxis->SetTitle("P_{T}");
    xAxis->SetLabelSize(0.03);
    xAxis->Draw();

    TGaxis *zAxis = new TGaxis(0.1, 0.32, 0.1, 0.9, fHistos->zbins[0].GetMin(), fHistos->zbins[0].GetMax(), 8, "");
    zAxis->SetTitle("z");
    zAxis->SetLabelSize(0.03);
    zAxis->Draw();    

    TPad *pads[fHistos->npt_km+1][fHistos->nz_km+1];
    
    double padding = 0.01; 
    double xstop = 0.9; 
    double xstart = 0.18; 
    double ystop = 0.9; 
    double ystart = 0.18; 
    double xwidth = ((xstop-xstart) - (fHistos->npt_km-1)*padding)/(1+fHistos->npt_km); 
    double ywidth = ((ystop-ystart) - (fHistos->nz_km-1)*padding)/(1+fHistos->nz_km); 


    for (int x=0; x<fHistos->npt_km+1; x++){
      for (int y=0; y<fHistos->nz_km+1; y++){
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
	fHistos->h1_asym_km[0][0][y][x]->SetLineColor(color);
	fHistos->h1_asym_km[0][0][y][x]->SetLineWidth(2);
	fHistos->h1_asym_km[0][0][y][x]->SetMarkerColor(color);
	fHistos->h1_asym_km[0][0][y][x]->SetMarkerStyle(24);
	fHistos->h1_asym_km[0][0][y][x]->SetMarkerSize(1);
	fHistos->h1_asym_km[0][0][y][x]->Draw("pe");

	fHistos->fit_km[0][0][y][x]->SetLineColor(color);
	fHistos->fit_km[0][0][y][x]->Draw("same");
      }
    }

    can->cd(); 
 
    TLatex title; 
    title.SetNDC();
    title.SetTextSize(0.03); 
    title.DrawLatex(0.4, 0.95, "K^{-} Asymmetry"); 

    title.SetTextSize(0.02); 
    title.DrawLatex(0.05, 0.12, "#color[99]{Integrated P_{T}}"); 
    title.DrawLatex(0.05, 0.09, "#color[55]{Integrated z}"); 
    title.DrawLatex(0.05, 0.06, "#color[77]{Integrated both}"); 
    
    can->Print(Form("%sgrid_pt_z_km.png",fOutputPath.c_str())); 
  }
  
  
 protected:
  AsymmetryHistograms *fHistos; 
  std::string          fOutputPath; 
  

};

#endif
