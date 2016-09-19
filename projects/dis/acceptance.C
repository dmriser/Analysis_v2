#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TStyle.h"

void acceptance(string fileTitle){
  
  TFile * inputFile = TFile::Open(fileTitle.c_str()); 

  TH2F *genEventsxByQQ = (TH2F*) inputFile->Get("h2_gen_norad_x_qq_0");
  TH2F *recEventsxByQQ = (TH2F*) inputFile->Get("h2_rec_norad_x_qq_0"); 

  int xRebinFactor = 1;
  int qqRebinFactor = 1; 

  genEventsxByQQ->Rebin2D(xRebinFactor, qqRebinFactor);
  recEventsxByQQ->Rebin2D(xRebinFactor, qqRebinFactor); 

  TH2F *acceptance2D = (TH2F*) recEventsxByQQ->Clone(); 
  acceptance2D->Divide( genEventsxByQQ ); 
  
  int numberXBins = recEventsxByQQ->GetXaxis()->GetNbins();
  int numberQQBins = recEventsxByQQ->GetYaxis()->GetNbins();  
  double xMin = recEventsxByQQ->GetXaxis()->GetBinLowEdge(1);
  double qqMin = recEventsxByQQ->GetYaxis()->GetBinLowEdge(1); 
  double xMax = recEventsxByQQ->GetXaxis()->GetBinUpEdge(numberXBins);
  double qqMax = recEventsxByQQ->GetYaxis()->GetBinUpEdge(numberQQBins); 

  TH1D * acceptance = new TH1D("acceptance", "", numberXBins, xMin, xMax); 
  acceptance2D->ProjectionX("acceptance",1,numberQQBins); 
  acceptance->Scale(1.0/numberQQBins); 
  
  // -------------------------------------------------------------------------------------
  //                            Drawing To Canvas 
  // -------------------------------------------------------------------------------------
  
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  TCanvas * canvas = new TCanvas("canvas","",1200,400); 
  canvas->Divide(3,1);
  canvas->cd(1);
  gPad->SetLogz();
  recEventsxByQQ->Draw("colz"); 
  canvas->cd(2);
  gPad->SetLogz();
  genEventsxByQQ->Draw("colz");
  canvas->cd(3);
  acceptance->SetMinimum(0.0);
  acceptance->SetMaximum(1.0);
  acceptance->SetFillColorAlpha(kRed,0.25); 
  acceptance->Draw(); 
}
