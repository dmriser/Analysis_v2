
#include "DBins.h"
#include "DHistogramTools.h"

#include <iostream>
using namespace std; 

#include "TCanvas.h"
#include "TH2.h"
#include "TRandom3.h"

int main(){

  string outputFilenameNoExtension = "testTheTH2DFeature"; 

  TRandom3 random; 

  DBins xBins(100, -2, 2);
  DBins yBins(100, -2, 2);
 
  TH2D * sourceHistogram = new TH2D("sourceHistogram","",xBins.number(),xBins.min(),xBins.max(),yBins.number(),yBins.min(),yBins.max());

  for (int igen=0; igen<1000000; igen++){
    double rx = random.Gaus(0.2, 1.5); 
    double ry = random.Gaus(-0.1, 0.5); 
    sourceHistogram->Fill(rx,ry);
  }
  
  DPlaneBins xyBinsFromHistogram( sourceHistogram );
  
  TH2D * rebinnedHistogram = TH2DStatisticalRebin(sourceHistogram, 20, 20); 

  TCanvas * can = new TCanvas("can","",1000,500); 
  can->Divide(2,1); 
  can->cd(1);
  sourceHistogram->Draw("colz");
  can->cd(2);
  rebinnedHistogram->Draw("colz"); 
  can->Print("TH2Rebin.pdf"); 
  
  return 0;
}
