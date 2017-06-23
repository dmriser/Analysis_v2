#include <iostream>
#include <vector>
using namespace std;

#include "DBins.h"
#include "DHistogramTools.h"

#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TRandom3.h"

int main(){

  DBins xBins(20, -50, 50); 
  DBins yBins(20, -50, 50);

  TRandom3 randomNumbers; 

  TH2D * sourceHistogram = new TH2D("sourceHistogram","sourceTitle",xBins.number(), xBins.min(), xBins.max(), yBins.number(), yBins.min(), yBins.max());

  for (int rn = 0; rn < 1000000; rn++) {
    sourceHistogram->Fill(randomNumbers.Gaus(0, 25), -50+randomNumbers.Uniform(100)); 
  }
  
    vector<TH1D*> destinationHistograms = projectXTH2DToVectorOfTH1D(sourceHistogram, "x-title", "bin %.2f #rightarrow %.2f"); 

  cout << "Detected size: " << destinationHistograms.size() << endl; 
  
  TCanvas * can = new TCanvas("can","",800,800); 

    can->Print("TH2DtoVectorOfTH1D.pdf[");
    sourceHistogram->Draw("colz"); 
    can->Print("TH2DtoVectorOfTH1D.pdf");
    drawVectorOfTH1D(destinationHistograms, can, "TH2DtoVectorOfTH1D","x-title","y-title","PE");
    can->Print("TH2DtoVectorOfTH1D.pdf]");
      
  
  return 0; 
}
