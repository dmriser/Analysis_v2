
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
  DBins yBins(10, 0, 1);
  
  VectorTH1D h1_x_by_y("h1_x_by_y_%d",xBins,yBins); 

  for (int yBin = 1; yBin <= yBins.number(); yBin++) {
    for (int igen = 0; igen < 10000; igen++) {
      double filler = random.Gaus(0.0, 0.5);
      h1_x_by_y.getHistogram(yBin) ->Fill(filler);
      h1_x_by_y.getHistogram(0)   ->Fill(filler);
    }
  }
  
  h1_x_by_y.Draw(can, "long_title_test", " Title ", " s^{4} Bin %d ");
  
  return 0;
}
