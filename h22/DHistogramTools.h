////////////////////////////////////////////////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  

*/
////////////////////////////////////////////////////////////////////////////////////

#ifndef dhistogramtools_h
#define dhistogramtools_h

#include <iostream>
#include <vector>
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TLatex.h"

#include "DBins.h"

class VectorTH1D{
 public:
  VectorTH1D(std::string titleFormat, DBins xAxisBins, DBins vectorBins);
  VectorTH1D(std::string titleFormat, TH2D * sourceHistogram);
  VectorTH1D();

  std::vector<TH1D*> histos;

  void Draw(TCanvas * canvas, std::string pdfNameWithoutExtension, std::string xTitle, std::string vectorTitleFormat);
  TH1D * getHistogram(int index){ return histos[index]; }
  
};

class CLASVectorTH1D{
 public:
  CLASVectorTH1D(std::string titleFormat, DBins xAxisBins, DBins vectorBins);
  ~CLASVectorTH1D();

  VectorTH1D sectors[7];

  void Draw(TCanvas * canvas, std::string pdfNameWithoutExtension, std::string xTitle, std::string vectorTitleFormat);
  VectorTH1D getSectorHistograms(int sector){ return sectors[sector]; }
  
};
/*
inline TH2D * TH2DStatisticalRebin(TH2D * sourceHistogram, int xBins, int yBins){

  DPlaneBins sourceBins(sourceHistogram);
  sourceBins.StatisticalRebin(xBins, yBins);  

  return sourceBins.toTH2D(); 
}
*/
inline int findCanvasDivisions(int numberOfHistograms){

  int divs = 0; 
  int total = 1; 
  
  while (total < numberOfHistograms){
    divs++;
    total = divs*(divs+1);
    
    if (total == numberOfHistograms) return divs; 
  }
  
  return divs; 
}

#endif
