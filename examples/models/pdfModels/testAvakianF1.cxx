
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

  F1StructureFunction F1; 

  PDFUpPlus_HA2007 *uPlus = new PDFUpPlus_HA2007();
  PDFUpMinus_HA2007 *uMinus = new PDFUpMinus_HA2007();

  F1.addPDF( uPlus );
  F1.addPDF( uMinus ); 
  
  TH1D * h1_f1 = new TH1D("h1_f1","",xBins.number(),xBins.min(),xBins.max()); 
  
  for (int ibin = 0; ibin < xBins.number(); ibin++){
    double x = xBins.bin_center(ibin);
    h1_f1->SetBinContent(ibin+1, F1.getValue(x)); 
    cout.width(12); cout << x;
    cout.width(12); cout << F1.getValue(x) << endl; 
  }

  cout << endl;
  
  TLatex lab;
  lab.SetNDC(0); 

  TCanvas * c1 = new TCanvas("c1","",800,800);

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  h1_f1->Draw();
  c1->Print("testF1AvakianModel.pdf");
  
  return 0;
}
