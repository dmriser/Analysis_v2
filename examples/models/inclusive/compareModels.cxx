#include <iostream>
using namespace std; 

#include "BostedInclusiveWrapper.h"
#include "KeppelInclusiveWrapper.h"
#include "CommonTools.h"

// cern root libs
#include "TH1.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TPad.h"

int main(){

  F1F209Wrapper bosted; 
  KeppelInclusiveWrapper keppel; 

  double beamEnergyDouble = 5.75; 
  float beamEnergyFloat = 5.75; 

  const int numberQQBins = 200; 
  const double wFixed = 1.21; 
  const double qqMin = 1.5;  
  const double qqMax = 4.0;  

  const int numberWBins = 200; 
  const double qqFixed = 2.05; 
  const double wMin = 1.1;  
  const double wMax = 1.9;  

  // Histograms 
  TH1D *bostedModelQQ = new TH1D("bostedModelQQ","bostedModelQQ",numberQQBins,qqMin,qqMax);
  TH1D *bostedModelQQWithFlux = new TH1D("bostedModelQQWithFlux","bostedModelQQ",numberQQBins,qqMin,qqMax);
  TH1D *keppelModelQQ = new TH1D("keppelModelQQ","keppelModelQQ",numberQQBins,qqMin,qqMax);
  TH1D *ratioQQ = new TH1D("ratioQQ","ratioQQ",numberQQBins,qqMin,qqMax);

  TH1D *bostedModelW = new TH1D("bostedModelW","bostedModelW",numberWBins,wMin,wMax);
  TH1D *bostedModelWWithFlux = new TH1D("bostedModelWWithFlux","bostedModelW",numberWBins,wMin,wMax);
  TH1D *keppelModelW = new TH1D("keppelModelW","keppelModelW",numberWBins,wMin,wMax);
  TH1D *ratioW = new TH1D("ratioW","ratioW",numberWBins,wMin,wMax);

  for (int ibin=0; ibin< numberQQBins; ibin++){
    double qq = qqMin + ibin*(qqMax-qqMin)/numberQQBins; 
    
    double bostedValue = bosted.GetXSByWQQ(1,1,beamEnergyDouble,wFixed,qq);
    double bostedValueWithFlux = bosted.GetXSByWQQ(1,1,beamEnergyDouble,wFixed,qq)*calculatePhotonFlux(beamEnergyDouble,wFixed,qq);
    float keppelValue = keppel.GetXS(beamEnergyFloat,qq,wFixed);      
    
    bostedModelQQ->SetBinContent(ibin+1,bostedValue);
    bostedModelQQWithFlux->SetBinContent(ibin+1,bostedValueWithFlux);
    keppelModelQQ->SetBinContent(ibin+1,keppelValue);
    ratioQQ->SetBinContent(ibin+1,bostedValueWithFlux/keppelValue);
  }

  for (int ibin=0; ibin< numberWBins; ibin++){
    double w = wMin + ibin*(wMax-wMin)/numberWBins; 
    
    double bostedValue = bosted.GetXSByWQQ(1,1,beamEnergyDouble,w,qqFixed);
    double bostedValueWithFlux = bosted.GetXSByWQQ(1,1,beamEnergyDouble,w,qqFixed)*calculatePhotonFlux(beamEnergyDouble,w,qqFixed);
    float keppelValue = keppel.GetXS(beamEnergyFloat,qqFixed,w);      
    
    bostedModelW->SetBinContent(ibin+1,bostedValue);
    bostedModelWWithFlux->SetBinContent(ibin+1,bostedValueWithFlux);
    keppelModelW->SetBinContent(ibin+1,keppelValue);
    ratioW->SetBinContent(ibin+1,bostedValueWithFlux/keppelValue);
  }
 
  TLatex caption;
  caption.SetNDC();
  caption.SetTextFont(12);
  caption.SetTextSize(0.05);

  TCanvas *modelCanvas = new TCanvas("modelCanvas","",800,800);

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  modelCanvas->Divide(2,2);
  modelCanvas->SetMargin(0.15, 0.15, 0.15, 0.15);

  modelCanvas->cd(1);
  gPad->SetLogy();
  gPad->SetMargin(0.12, 0.12, 0.12, 0.12);
  bostedModelQQ->SetMinimum(1e-6);
  bostedModelQQ->SetMaximum(1e2);
  bostedModelQQWithFlux->SetMinimum(1e-6);
  bostedModelQQWithFlux->SetMaximum(1e2);
  bostedModelQQ->SetLineColor(kRed);
  bostedModelQQ->Draw("l");
  bostedModelQQWithFlux->Draw("lsame");
  keppelModelQQ->SetLineColor(kBlue);
  keppelModelQQ->Draw("lsame");
  caption.DrawLatex(0.56, 0.83, "#rightarrow Bosted+Flux");
  caption.SetTextColor(kBlue);
  caption.DrawLatex(0.56, 0.78, "#rightarrow Keppel");
  caption.SetTextColor(kRed);
  caption.DrawLatex(0.56, 0.73, "#rightarrow Bosted");
  caption.SetTextColor(kBlack);
  caption.DrawLatex(0.7, 0.025, "Q^{2} [GeV^{2}/c^{2}]");
  caption.DrawLatex(0.4, 0.95, Form("W = %.3f ",wFixed));

  modelCanvas->cd(2);
  gPad->SetMargin(0.12, 0.12, 0.12, 0.12);
  ratioQQ->Draw("l");
  caption.DrawLatex(0.7, 0.025, "Q^{2} [GeV^{2}/c^{2}]");
  caption.DrawLatex(0.56, 0.83, "#rightarrow Ratio");
  caption.DrawLatex(0.4, 0.95, Form("W = %.3f ",wFixed));

  modelCanvas->cd(3);
  gPad->SetLogy();
  gPad->SetMargin(0.12, 0.12, 0.12, 0.12);
  //  bostedModelWWithFlux->SetLineColor(kRed);
  keppelModelW->SetLineColor(kBlue);
  keppelModelW->Draw("l");
  bostedModelWWithFlux->Draw("lsame");
  caption.SetTextColor(kBlue);
  caption.DrawLatex(0.56, 0.83, "#rightarrow Keppel");
  caption.SetTextColor(kBlack);
  caption.DrawLatex(0.56, 0.78, "#rightarrow Bosted+Flux");
  caption.SetTextColor(kBlack);
  caption.DrawLatex(0.7, 0.025, "W [GeV/c^{2}]");
  caption.DrawLatex(0.4, 0.95, Form("Q^{2} = %.3f ",qqFixed));

  modelCanvas->cd(4);
  gPad->SetMargin(0.12, 0.12, 0.12, 0.12);
  ratioW->Draw("l");
  caption.DrawLatex(0.7, 0.025, "W [GeV/c^{2}]");
  caption.DrawLatex(0.56, 0.83, "#rightarrow Ratio");
  caption.DrawLatex(0.4, 0.95, Form("Q^{2} = %.3f ",qqFixed));

  modelCanvas->Print("compareModels.png");

  return 0;
}
