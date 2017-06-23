#include <cmath>
#include <iostream>
using namespace std; 

#include "BostedElasticWrapper.h"
#include "CommonTools.h"

// cern root libs
#include "TH1.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TPad.h"

int main(){

  BostedElasticWrapper bostedModel; 
  
  double beamEnergy = 5.498;        // GeV 
  double targetRadLengths = 0.0057; // Fractional 
  double wCut = 3.00;               // GeV/c^2 

  int numberThetaBins = 200; 
  double thetaMin = 18.0; 
  double thetaMax = 60.0; 
  double thetaWidth = (thetaMax-thetaMin)/numberThetaBins; 

  int numberWBins = 200; 
  double wMin = 0.1; 
  double wMax = 2.7; 
  double wWidth = (wMax-wMin)/numberWBins;

  TH1D *elastic    = new TH1D("elastic","",numberThetaBins, thetaMin, thetaMax);
  TH1D *elasticRad = new TH1D("elasticRad","",numberThetaBins, thetaMin, thetaMax);
  TH1D *wRad       = new TH1D("wRad","",numberWBins,wMin,wMax);
  
  for (int bin=0; bin< numberThetaBins; bin++){
    double theta = thetaMin + bin*thetaWidth; 
    double elasticValue = bostedModel.getValue(beamEnergy, theta); 
    double elasticRadValue = bostedModel.getRadiatedValue(beamEnergy, theta, targetRadLengths, wCut);

    elastic->SetBinContent(bin+1, elasticValue);
    elasticRad->SetBinContent(bin+1, elasticRadValue);

    cout.width(16); cout << theta;
    cout.width(16); cout << elasticValue; 
    cout.width(16); cout << elasticRadValue << endl; 
  }

  double qqFixed = 2.75;
 
  for (int bin=0; bin<numberWBins; bin++){
    double w = wMin + bin*wWidth; 
    double nu = (pow(w,2)-pow(proton_mass,2)+qqFixed)/(2*proton_mass);
    double ePrime = beamEnergy - nu;
    double theta = 2 * asin( sqrt( qqFixed/(4*ePrime*beamEnergy) ) )*to_degrees;
    
    double jacobian = (2*proton_mass*ePrime*beamEnergy)/(w); 
    double radValue = jacobian*bostedModel.getRadiatedValue(beamEnergy, theta, targetRadLengths, wCut);
    wRad->SetBinContent(bin+1,radValue);

    cout.width(16); cout << theta;
    cout.width(16); cout << radValue << endl; 
  }

  TLatex caption, radTitle, noRadTitle;
  caption.SetNDC();
  caption.SetTextSize(0.05);

  radTitle.SetNDC();
  radTitle.SetTextSize(0.05);

  noRadTitle.SetNDC();
  noRadTitle.SetTextSize(0.05);

  TCanvas *modelCanvas = new TCanvas("modelCanvas","",1200,600);
  modelCanvas->Divide(2,1);

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  modelCanvas->cd(1);
  gPad->SetLogy();
  gPad->SetMargin(0.18, 0.18, 0.18, 0.18);

  elastic->SetLineColor(kBlue); 
  elastic->Draw("l");
  elasticRad->SetLineColor(kRed);
  elasticRad->Draw("lsame");

  radTitle.SetTextColor(kRed);
  radTitle.DrawLatex(0.61, 0.82, "Radiated");
  noRadTitle.SetTextColor(kBlue);
  noRadTitle.DrawLatex(0.61, 0.77, "Not Radiated");
  
  caption.DrawLatex(0.72, 0.041, "#theta_{lab}"); 
  caption.SetTextAngle(90);
  caption.DrawLatex(0.05, 0.71, "#frac{d#sigma}{d#theta_{lab}}"); 
 
  modelCanvas->cd(2);
  gPad->SetLogy();
  gPad->SetMargin(0.18, 0.18, 0.18, 0.18);
  wRad->Draw("l");

  caption.DrawLatex(0.05, 0.71, "#frac{d#sigma}{dW dQ^{2}}"); 
  caption.SetTextAngle(0.0);
  caption.DrawLatex(0.72, 0.041, "W (GeV/c^{2})"); 
  
  modelCanvas->Print("plotModels.png");

  return 0;
}
