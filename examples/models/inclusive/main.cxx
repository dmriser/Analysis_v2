// c++ stuff 
#include <iostream>
#include <cmath>
#include <vector>
using std::cout;
using std::endl;
using std::vector;
using std::string;

// model
extern"C"{
  double brasse_(double *beamEnergy, double *qq, double *w);
}

// my stuff
#include "CommonTools.h"
#include "DBins.h"

// root stuff
#include "TCanvas.h"
#include "TH1.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TPad.h"
#include "TStyle.h"

int main(int argc, char * argv[]){

  F1F209Wrapper model;

  // Constants for XS 
  int A = 1, Z = 1;
  double beam_energy = 6.0; // GeV 

  // Passing in A, Z
  if (argc == 3) {
    A = atoi( argv[1] );
    Z = atoi( argv[2] );
  }
  
  DLineBins thetaBins(60, 5, 35);
  DLineBins energyBins(400,0.5,5.9);

  // Make something to hold histograms 
  vector<TH1D*> h1_xs; 
  for (int b=0; b<thetaBins.GetNumber(); b++) {
    TH1D * this_hist = new TH1D(Form("h1_xs_b%d",b),"",energyBins.GetNumber(), energyBins.GetMin(), energyBins.GetMax());
    h1_xs.push_back( this_hist ); 
  }
  
  // Looping on bins 
  for (int thetaBin = 0; thetaBin < thetaBins.GetNumber(); thetaBin++) {
    for (int energyBin = 0; energyBin < energyBins.GetNumber(); energyBin++) {
      double xs = model.GetXSByAngle(Z, A, beam_energy, energyBins.GetBin(energyBin).GetCenter(), thetaBins.GetBin(thetaBin).GetCenter());
      
      //      cout.width(12); cout << "\r" << thetaBins .GetBin(thetaBin) .GetCenter();
      //      cout.width(12); cout << energyBins.GetBin(energyBin).GetCenter();
      //      cout.width(12); cout << xs << std::flush;

      h1_xs[thetaBin]->SetBinContent(1+energyBin, xs/mev_to_gev);
      
    }
  }
  cout << endl;
  
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  int colorOffset = 15; 

  string name      = Form("a%dz%d.pdf",A,Z);
  string openName  = Form("a%dz%d.pdf[",A,Z);
  string closeName = Form("a%dz%d.pdf]",A,Z); 
  
  // print out 
  TLatex lab;
  lab.SetNDC(); 
  lab.SetTextSize(0.03); 
  
  TLegend leg(0.88,0.05,1.0,0.95); 
  leg.SetBorderSize(0); 
  
  TCanvas * printer = new TCanvas("printer","",1200,800); 
  printer->Print( openName.c_str() );

  gPad->SetLogy();
  gPad->SetMargin(0.12,0.12,0.15,0.1); 

  for (int b=0; b<thetaBins.GetNumber(); b++){
    h1_xs[b]->SetMaximum(1e3);
    h1_xs[b]->SetMinimum(1e-9); 
    h1_xs[b]->SetLineColor(b+colorOffset);
    h1_xs[b]->Draw("same");
    leg.AddEntry(h1_xs[b], Form("#theta = %.1f",thetaBins.GetBin(b).GetMin()), "l"); 
  }

  lab.DrawLatex(0.2,0.95,Form(" Inclusive e^{-} on A=%d Z=%d from Bosted Model E_{beam} = %.2f",A,Z,beam_energy));
  lab.DrawLatex(0.45,0.05," E_{Final} [GeV] ");
  lab.SetTextAngle(90); 
  lab.DrawLatex(0.05,0.35," #frac{ d#sigma }{ dE d#Omega } #left[ #frac{#mu b}{GeV-sr} #right]"); 
  lab.SetTextAngle(0);
  leg.Draw(); 
  printer->Print( name.c_str() );
  
  printer->Print( closeName.c_str() ); 
  
  return 0; 
}
