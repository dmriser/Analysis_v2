
#include <iostream>
using namespace std; 

#include "DBins.h"
#include "GenericPDF.h"
#include "Avakian2007PDF.h"

#include "TCanvas.h"
#include "TH1.h"

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
  
  for (int ibin = 0; ibin < xBins.number(); ibin++){
    h1_upPlusPDF->SetBinContent(ibin+1, PDFupPlus.getValue(xBins.bin_center(ibin)));
    h1_downPlusPDF->SetBinContent(ibin+1, PDFdownPlus.getValue(xBins.bin_center(ibin)));
    h1_upMinusPDF->SetBinContent(ibin+1, PDFupMinus.getValue(xBins.bin_center(ibin)));
    h1_downMinusPDF->SetBinContent(ibin+1, PDFdownMinus.getValue(xBins.bin_center(ibin))); 
  }

  cout << endl;
  
  TCanvas * c1 = new TCanvas("c1","",800,800);

  h1_upPlusPDF->Draw();
  h1_downPlusPDF->Draw("same");
  h1_upMinusPDF->Draw("same");
  h1_downMinusPDF->Draw("same");
  c1->Print("testAvakianModel.pdf");
  
  return 0;
}
