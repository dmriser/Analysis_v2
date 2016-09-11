////////////////////////////////////////////////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  

*/
////////////////////////////////////////////////////////////////////////////////////

#ifndef dhistogramtools_cxx
#define dhistogramtools_cxx

#include <cmath>
#include <iostream>
#include <vector>
using namespace std; 

#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TLatex.h"
#include "TStyle.h"

#include "DBins.h"
#include "DHistogramTools.h"

VectorTH1D::VectorTH1D(string titleFormat, DBins xAxisBins, DBins vectorBins){

  for (int ibin = 0; ibin <= vectorBins.number(); ibin++){
    TH1D * hist = new TH1D(Form(titleFormat.c_str(),ibin),Form(titleFormat.c_str(),ibin),xAxisBins.number(),xAxisBins.min(),xAxisBins.max());
    histos.push_back(hist); 
  }

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  
}

VectorTH1D::VectorTH1D(string titleFormat, TH2D * sourceHistogram){

  int yBins   = sourceHistogram->GetYaxis()->GetNbins();
  double yMin = sourceHistogram->GetYaxis()->GetBinLowEdge(1);
  double yMax = sourceHistogram->GetYaxis()->GetBinUpEdge(yBins+1);

  int xBins   = sourceHistogram->GetXaxis()->GetNbins();
  double xMin = sourceHistogram->GetXaxis()->GetBinLowEdge(1);
  double xMax = sourceHistogram->GetXaxis()->GetBinUpEdge(xBins+1);
  
  DBins vectorBins(yBins, yMin, yMax);
  DBins xAxisBins (xBins, xMin, xMax);

  // Is there a better way to do this? Don't want to use if in loop. 
  TH1D * firstHist = sourceHistogram->ProjectionX(Form(titleFormat.c_str(),0),1,vectorBins.number());
  histos.push_back(firstHist);
  
  for (int ibin = 1; ibin <= vectorBins.number(); ibin++){
    string title = Form(titleFormat.c_str(),ibin);
    TH1D * hist = sourceHistogram->ProjectionX(title.c_str(),ibin,ibin+1);
    histos.push_back(hist); 
  }

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  
}

VectorTH1D::VectorTH1D(){
  // Do we need to loop on vector and destroy pointers? Or is root "automatically" doing it. 
}

void VectorTH1D::Draw(TCanvas * canvas, string pdfNameWithoutExtension, string xTitle, string vectorTitleFormat){

  TLatex lab;
  lab.SetNDC();

  int canvasSize = findCanvasDivisions(histos.size());
  canvas->Divide(canvasSize+1, canvasSize); 

  double xTitleCenterOffset = 0.5 - 0.01*xTitle.size();
  double vectorTitleOffset = 0.5 - 0.01*vectorTitleFormat.size();
  
  for (int ihist = 0; ihist < histos.size(); ihist++){
    canvas->cd(ihist+1);
    histos[ihist]->Draw("PE");
    lab.DrawLatex(vectorTitleOffset,0.95,Form(vectorTitleFormat.c_str(),ihist));
    lab.DrawLatex(xTitleCenterOffset, 0.025,xTitle.c_str());
  } 

  canvas->Print(Form("%s.pdf",pdfNameWithoutExtension.c_str()));
  canvas->Clear();
}

CLASVectorTH1D::CLASVectorTH1D(string titleFormat, DBins xAxisBins, DBins vectorBins){

  for (int isect = 0; isect < 7; isect++){
    string name = Form("%s_%d",titleFormat.c_str(),isect);
    sectors[isect] = VectorTH1D(name, xAxisBins, vectorBins);
  }
  
}

CLASVectorTH1D::~CLASVectorTH1D(){}

void CLASVectorTH1D::Draw(TCanvas * canvas, string pdfNameWithoutExtension, string xTitle, string vectorTitleFormat){

  for (int isect = 0; isect < 7; isect++){
    string sectorTitle = Form("%s Sector %d",vectorTitleFormat.c_str(),isect);
    sectors[isect].Draw(canvas, pdfNameWithoutExtension, xTitle, sectorTitle);
  } 
  
}

#endif
