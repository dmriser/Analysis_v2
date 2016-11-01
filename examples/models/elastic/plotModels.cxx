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
  double wCut = 1.08;               // GeV/c^2 

  int numberThetaBins = 200; 
  double thetaMin = 18.0; 
  double thetaMax = 60.0; 
  double thetaWidth = (thetaMax-thetaMin)/numberThetaBins; 

  TH1D *elastic = new TH1D("elastic","",numberThetaBins, thetaMin, thetaMax);
  TH1D *elasticRad = new TH1D("elasticRad","",numberThetaBins, thetaMin, thetaMax);
  
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

  TLatex caption, radTitle, noRadTitle;
  caption.SetNDC();
  caption.SetTextSize(0.05);

  radTitle.SetNDC();
  radTitle.SetTextSize(0.05);

  noRadTitle.SetNDC();
  noRadTitle.SetTextSize(0.05);

  TCanvas *modelCanvas = new TCanvas("modelCanvas","",800800);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

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
 
  modelCanvas->Print("plotModels.png");

  return 0;
}
