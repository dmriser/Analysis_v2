#ifndef tools_cxx
#define tools_cxx

// --------------------------
//     c++ 
// --------------------------
#include <iostream>
#include <sstream>

// --------------------------
//     root 
// --------------------------
#include "TH1.h"
#include "TH2.h"

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

  double Tools::getTitleCenterPosition(std::string title){
    return (0.5-0.0075*title.size());
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
