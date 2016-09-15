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
  return (0.5-title.size()*0.005); 
}

inline void drawVectorOfTH1D(std::vector<TH1D*> histoVector, TCanvas * canvas, std::string pdfTitle, std::string xTitle, std::string yTitle, std::string drawOptions){

  TLatex lab; 
  lab.SetNDC(); 
  lab.SetTextFont(22); 
  
  int canvasSize = findCanvasDivisions( histoVector.size() );
  int markerStyle = 7; 

  canvas->Clear();
  canvas->Divide(canvasSize+1, canvasSize);

  gPad->SetMargin(0.15,0.1,0.1,0.1);
  
  for (int h = 0; h < histoVector.size(); h++){
    canvas->cd(h+1);
    std::string drawCommand = Form("%ssame",drawOptions.c_str()); 
    histoVector[h]->SetMarkerStyle( markerStyle );
    histoVector[h]->Draw( drawCommand.c_str() );
    std::string title = histoVector[h]->GetTitle();
    lab.DrawLatex( getTitleOffset(title), 0.925, title.c_str()); 
    lab.DrawLatex( getTitleOffset(xTitle), 0.025, xTitle.c_str());
    lab.SetTextAngle(90.0); 
    lab.DrawLatex( 0.04, getTitleOffset(yTitle), yTitle.c_str()); 
    lab.SetTextAngle(0.0); 
  }

  canvas->Print(Form("%s.pdf",pdfTitle.c_str())); 
  canvas->Clear(); 
}

inline void drawVectorOfVectorOfTH1D(std::vector< std::vector<TH1D*> > histoVector, TCanvas * canvas, std::string pdfTitle, std::string xTitle, std::string yTitle, std::string drawOptions){

  for (int iPage = 0; iPage < histoVector.size(); iPage++) {
    drawVectorOfTH1D(histoVector[iPage], canvas, pdfTitle, xTitle, yTitle, drawOptions);
  }
}

inline std::vector<TH1D*> projectXTH2DToVectorOfTH1D(TH2D * histo, std::string nameTemplate, std::string titleTemplate){

  std::vector<TH1D*> aVectorOfTH1D; 
  int numberXBins = histo->GetXaxis()->GetNbins();
  int numberYBins = histo->GetYaxis()->GetNbins(); 
  double xMax = histo->GetXaxis()->GetBinUpEdge(numberXBins);
  double yMax = histo->GetYaxis()->GetBinUpEdge(numberYBins);
  double xMin = histo->GetXaxis()->GetBinLowEdge(1);
  double yMin = histo->GetYaxis()->GetBinLowEdge(1);
  DBins xBins(numberXBins, xMin, xMax);
  DBins yBins(numberYBins, yMin, yMax); 

  TH1D * aTH1D = new TH1D(Form("%s_%d",nameTemplate.c_str(),0),Form(titleTemplate.c_str(),yBins.min(),yBins.max()),xBins.number(), xBins.min(), xBins.max());  
  histo->ProjectionX(Form("%s_%d",nameTemplate.c_str(),0),1,yBins.number());   
  aVectorOfTH1D.push_back( aTH1D );

  for (int ihist = 1; ihist <= yBins.number(); ihist++){
    TH1D * thisTH1D = new TH1D(Form("%s_%d",nameTemplate.c_str(),ihist),Form(titleTemplate.c_str(),yBins.bin_center(ihist),yBins.bin_center(ihist+1)),xBins.number(), xBins.min(), xBins.max());  
    histo->ProjectionX(Form("%s_%d",nameTemplate.c_str(),0),ihist,ihist+1);   
    aVectorOfTH1D.push_back( thisTH1D );
  }

  return aVectorOfTH1D; 
}

#endif
