#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TStyle.h"

void stability(string inputFilename){

  TFile * inputFile = TFile::Open( inputFilename.c_str() ); 

  TH2F *recAndGenEventsxByQQ = (TH2F*) inputFile->Get("h2_rec_gen_norad_x_qq_0");
  TH2F *genEventsxByQQ = (TH2F*) inputFile->Get("h2_gen_norad_x_qq_0"); 

  int xRebinFactor = 1;
  int qqRebinFactor = 1; 

  recAndGenEventsxByQQ->Rebin2D(xRebinFactor, qqRebinFactor);
  genEventsxByQQ->Rebin2D(xRebinFactor, qqRebinFactor); 

  TH2F *stability2D = (TH2F*) recAndGenEventsxByQQ->Clone(); 
  stability2D->Divide( genEventsxByQQ ); 
  
  int numberXBins = genEventsxByQQ->GetXaxis()->GetNbins();
  int numberQQBins = genEventsxByQQ->GetYaxis()->GetNbins();  
  double xMin = genEventsxByQQ->GetXaxis()->GetBinLowEdge(1);
  double qqMin = genEventsxByQQ->GetYaxis()->GetBinLowEdge(1); 
  double xMax = genEventsxByQQ->GetXaxis()->GetBinUpEdge(numberXBins);
  double qqMax = genEventsxByQQ->GetYaxis()->GetBinUpEdge(numberQQBins); 

  TH1D * stability = new TH1D("stability", "", numberXBins, xMin, xMax); 
  stability2D->ProjectionX("stability",1,numberQQBins); 
  stability->Scale(1.0/numberQQBins); 
  
  // -------------------------------------------------------------------------------------
  //                            Drawing To Canvas 
  // -------------------------------------------------------------------------------------
  
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  
  TCanvas * canvas = new TCanvas("canvas","",1200,400); 
  canvas->Divide(3,1);
  canvas->cd(1);
  gPad->SetLogz();
  genEventsxByQQ->Draw("colz"); 
  canvas->cd(2);
  gPad->SetLogz();
  recAndGenEventsxByQQ->Draw("colz");
  canvas->cd(3);
  stability->SetMinimum(0.0);
  stability->SetMaximum(1.0);
  stability->SetFillColorAlpha(kBlue,0.25); 
  stability->Draw(); 
  
}
