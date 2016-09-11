
#include "DBins.h"
#include "DHistogramTools.h"

#include <iostream>
using namespace std; 

#include "TCanvas.h"
#include "TRandom3.h"

int main(){


  TCanvas * can = new TCanvas("can","",1200,800);
  TRandom3 random; 
 
  DBins xBins(50, -2, 2);
  DBins yBins(28, 0, 1);
  
  CLASVectorTH1D x_by_y("h1_x_by_y_%d",xBins,yBins); 

  for (int isect = 1; isect < 7; isect ++) {
    for (int yBin = 1; yBin <= yBins.number(); yBin++) {
      for (int igen = 0; igen < 1000; igen++) {
	double filler = random.Gaus(0.0, 0.5);
	x_by_y.getSectorHistograms(0)    .getHistogram(0)    ->Fill(filler);
	x_by_y.getSectorHistograms(isect).getHistogram(0)    ->Fill(filler);
	x_by_y.getSectorHistograms(0)    .getHistogram(yBin) ->Fill(filler);
	x_by_y.getSectorHistograms(isect).getHistogram(yBin) ->Fill(filler);
      }
    }
  }

  can->Print("test.pdf[");
  x_by_y.Draw(can, "test", " My Title ", " Bin Number %d ");
  can->Print("test.pdf]");
    
  return 0;
}
