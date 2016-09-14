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

inline TH2D * TH2DStatisticalRebin(TH2D * sourceHistogram, int xBins, int yBins){

  DPlaneBins sourceBins(sourceHistogram);
  sourceBins.Print();
  sourceBins.StatisticalRebin(xBins, yBins);  

  return sourceBins.toTH2D(); 
}

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

inline double getTitleOffset(std::string title){
  return (0.5-title.size()*0.01); 
}

inline void drawVectorOfTH1D(std::vector<TH1D*> histoVector, TCanvas * canvas, std::string xTitle, std::string yTitle){

  TLatex lab; 
  lab.SetNDC(); 
  lab.SetTextFont(22); 
  
  int canvasSize = findCanvasDivisions( histoVector.size() );
  
  canvas->Clear();
  canvas->Divide(canvasSize+1, canvasSize);

  gPad->SetMargin(0.12,0.1,0.1,0.1);
  
  for (int h = 0; h < histoVector.size(); h++){
    canvas->cd(h+1);
    histoVector[h]->Draw("same");
    std::string title = histoVector[h]->GetTitle();
    lab.DrawLatex( getTitleOffset(title), 0.975, title.c_str()); 
    lab.DrawLatex( getTitleOffset(xTitle), 0.025, xTitle.c_str());
    lab.SetTextAngle(90.0); 
    lab.DrawLatex( 0.025, getTitleOffset(yTitle), yTitle.c_str()); 
    lab.SetTextAngle(0.0); 
  }
  
}

#endif
