
#include "DBins.h"
#include "DHistogramTools.h"

#include <iostream>
using namespace std; 

#include "TCanvas.h"
#include "TH2.h"
#include "TRandom3.h"

int main(){

  string outputFilenameNoExtension = "testTheTH2DFeature"; 

  TCanvas * can = new TCanvas("can","",1200,800);
  TRandom3 random; 
 
  DBins xBins(40, -2, 2);
  DBins yBins(10, -2, 2);

  TH2D * sourceHistogram = new TH2D("sourceHistogram","",xBins.number(),xBins.min(),xBins.max(),yBins.number(),yBins.min(),yBins.max());

  for (int igen=0; igen<1000000; igen++){
    double rx = random.Gaus(0.2, 0.5); 
    double ry = random.Gaus(-0.1, 0.5); 
    sourceHistogram->Fill(rx,ry);
  }
  
  VectorTH1D x_by_y("my_title_%d",sourceHistogram); 

  can->Print(Form("%s.pdf[",outputFilenameNoExtension.c_str())); 
  x_by_y.Draw(can, outputFilenameNoExtension, " Gaussian Random Var. ", " Bin Number %d ");
  sourceHistogram->Draw("colz"); 
  can->Print(Form("%s.pdf", outputFilenameNoExtension.c_str()));
  can->Print(Form("%s.pdf]",outputFilenameNoExtension.c_str()));
    
  return 0;
}
