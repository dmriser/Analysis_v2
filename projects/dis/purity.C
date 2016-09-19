#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TStyle.h"

void purity(string fileTitle){
  
  TFile * inputFile = TFile::Open(fileTitle.c_str()); 

  TH2F *recAndGenEventsxByQQ = (TH2F*) inputFile->Get("h2_rec_gen_norad_x_qq_0");
  TH2F *recEventsxByQQ = (TH2F*) inputFile->Get("h2_rec_norad_x_qq_0"); 

  int xRebinFactor = 1;
  int qqRebinFactor = 1; 

  recAndGenEventsxByQQ->Rebin2D(xRebinFactor, qqRebinFactor);
  recEventsxByQQ->Rebin2D(xRebinFactor, qqRebinFactor); 

  TH2F *purity2D = (TH2F*) recAndGenEventsxByQQ->Clone(); 
  purity2D->Divide( recEventsxByQQ ); 
  
  int numberXBins = recEventsxByQQ->GetXaxis()->GetNbins();
  int numberQQBins = recEventsxByQQ->GetYaxis()->GetNbins();  
  double xMin = recEventsxByQQ->GetXaxis()->GetBinLowEdge(1);
  double qqMin = recEventsxByQQ->GetYaxis()->GetBinLowEdge(1); 
  double xMax = recEventsxByQQ->GetXaxis()->GetBinUpEdge(numberXBins);
  double qqMax = recEventsxByQQ->GetYaxis()->GetBinUpEdge(numberQQBins); 

  TH1D * purity = new TH1D("purity", "", numberXBins, xMin, xMax); 
  purity2D->ProjectionX("purity",1,numberQQBins); 
  purity->Scale(1.0/numberQQBins); 
  
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
  recAndGenEventsxByQQ->Draw("colz");
  canvas->cd(3);
  purity->SetMinimum(0.0);
  purity->SetMaximum(1.0);
  purity->SetFillColorAlpha(kGreen,0.25); 
  purity->Draw(); 
}
