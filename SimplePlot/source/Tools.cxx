#ifndef tools_cxx
#define tools_cxx

// --------------------------
//     c++ 
// --------------------------
#include <iostream>
#include <vector>
#include <sstream>
using std::vector; 
using std::string; 

// --------------------------
//     root 
// --------------------------
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TList.h"
#include "TRegexp.h"
#include "TString.h"
#include "TObject.h"

// --------------------------
//     this 
// --------------------------
#include "Tools.h"

namespace SimplePlot{

  bool Tools::padIsLeft(int pad, int width, int height){
    return (pad%width == 1);
  }

  bool Tools::padIsRight(int pad, int width, int height){
    return (pad%width == 0);
  }

  bool Tools::padIsTop(int pad, int width, int height){
    return (pad <= width);
  }

  bool Tools::padIsBottom(int pad, int width, int height){
    return (pad >= width*(height-1));
  }

  void Tools::setIntegralSame(TH1 *histo1, TH1 *histo2){
    histo2->Scale( histo1->Integral()/histo2->Integral() );
  }

  void Tools::setMaximumSame(TH1 *histo1, TH1 *histo2){
    double max = histo1->GetMaximum(); 
    if(histo2->GetMaximum() > max){ max = histo2->GetMaximum(); }
    max *= 1.2; 
    histo1->SetMaximum(max); 
    histo2->SetMaximum(max); 
  }

  double Tools::getTitleCenterPosition(std::string title, double shiftPerChar){
    return (0.5-shiftPerChar*title.size());
  }

  void Tools::setupCanvasSpacing(TCanvas *canvas, int width, int height, double innerPadding, double outerPadding){
    int numberOfPads = width*height; 

    for(int pad=1; pad<=numberOfPads; pad++){
      canvas->cd(pad);
      if(padIsLeft(pad, width, height)){
        gPad->SetLeftMargin(outerPadding);
        gPad->SetRightMargin(innerPadding);
      }
      if (padIsRight(pad, width, height)){
        gPad->SetLeftMargin(innerPadding);
        gPad->SetRightMargin(outerPadding);
      }
      if(padIsBottom(pad, width, height)){
        gPad->SetBottomMargin(outerPadding);
        gPad->SetTopMargin(innerPadding);
      }
      if(padIsTop(pad, width, height)){
        gPad->SetBottomMargin(innerPadding);
        gPad->SetTopMargin(outerPadding);
      }
    }
  }

  std::vector<TH1D*> Tools::loadVectorOfTH1D(TFile *inputFile, string titleRegexp){
    TList * tableOfContents = inputFile->GetListOfKeys();
    
    TIter next(tableOfContents);
    TRegexp reg(titleRegexp.c_str()); 
    vector<TH1D*> foundHistos;
    while(TObject *objectFromFile = next()){
      TString currentObjectName = objectFromFile->GetName();
      if (currentObjectName.Contains(reg)){
	foundHistos.push_back((TH1D*) inputFile->Get(currentObjectName));
      }
    }
    
    return foundHistos; 
  }

  void Tools::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
      elems.push_back(item);
    }
  }

  std::vector<std::string> Tools::splitStringOnDelimeter(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
  }

}

#endif
