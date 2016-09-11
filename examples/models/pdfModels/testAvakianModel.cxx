
#include <iostream>
using namespace std; 

#include "DBins.h"
#include "GenericPDF.h"
#include "Avakian2007PDF.h"

#include "TCanvas.h"
#include "TH1.h"
#include "TLatex.h"
#include "TStyle.h"

int main(){

  DBins xBins(200,0.01,0.99);
  PDFUpPlus_HA2007 PDFupPlus;
  PDFDownPlus_HA2007 PDFdownPlus;
  PDFUpMinus_HA2007 PDFupMinus;
  PDFDownMinus_HA2007 PDFdownMinus;

  TH1D * h1_upPlusPDF = new TH1D("h1_upPlusPDF","",xBins.number(),xBins.min(),xBins.max());
  TH1D * h1_upMinusPDF = new TH1D("h1_upMinusPDF","",xBins.number(),xBins.min(),xBins.max());
  TH1D * h1_downPlusPDF = new TH1D("h1_DownPlusPDF","",xBins.number(),xBins.min(),xBins.max());
  TH1D * h1_downMinusPDF = new TH1D("h1_DownMinusPDF","",xBins.number(),xBins.min(),xBins.max());  

  TH1D * h1_du_u = new TH1D("h1_du_u","",xBins.number(),xBins.min(),xBins.max());
  TH1D * h1_dd_d = new TH1D("h1_dd_d","",xBins.number(),xBins.min(),xBins.max()); 
  
  for (int ibin = 0; ibin < xBins.number(); ibin++){
    double x = xBins.bin_center(ibin);
    h1_upPlusPDF->SetBinContent(ibin+1, PDFupPlus.getValue(x));
    h1_downPlusPDF->SetBinContent(ibin+1, PDFdownPlus.getValue(x));
    h1_upMinusPDF->SetBinContent(ibin+1, PDFupMinus.getValue(x));
    h1_downMinusPDF->SetBinContent(ibin+1, PDFdownMinus.getValue(x)); 

    double u = PDFupPlus.getValue(x) + PDFupMinus.getValue(x);
    double du = PDFupPlus.getValue(x) - PDFupMinus.getValue(x);

    double d = PDFdownPlus.getValue(x) + PDFdownMinus.getValue(x);
    double dd = PDFdownPlus.getValue(x) - PDFdownMinus.getValue(x);

    h1_du_u->SetBinContent(ibin+1, du/u);
    h1_dd_d->SetBinContent(ibin+1, dd/d); 
  }

  cout << endl;
  
  TLatex lab;
  lab.SetNDC(0); 

  TCanvas * c1 = new TCanvas("c1","",800,800);

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  //  c1->SetLogx();
  //  h1_upPlusPDF->Draw();
  //  h1_downPlusPDF->Draw("same");
  //  h1_upMinusPDF->Draw("same");
  //  h1_downMinusPDF->Draw("same");

  h1_du_u->SetLineColor(kRed);
  h1_du_u->SetFillColorAlpha(kRed, 0.1); 
  h1_dd_d->SetLineColor(kBlue);
  h1_dd_d->SetFillColorAlpha(kBlue, 0.1); 
  h1_du_u->SetMinimum(-1);
  h1_du_u->SetMaximum(1); 
  h1_du_u->Draw();
  h1_dd_d->Draw("same"); 
  lab.SetTextColor(kRed); 
  lab.DrawLatex(0.02, 0.84," #frac{#Delta u}{u}");
  lab.SetTextColor(kBlue); 
  lab.DrawLatex(0.02, -0.84," #frac{#Delta d}{d}");

  c1->Print("testAvakianModel.pdf");
  
  return 0;
}
