#ifndef tools_h
#define tools_h

// -----------------------------
//     c++ 
// -----------------------------
#include <iostream>
#include <vector>

// -----------------------------
//    root
// -----------------------------
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"

namespace SimplePlot{
  class Tools{

  public:
    Tools(){ }
    ~Tools(){ }

    bool padIsLeft(int pad, int width, int height);
    bool padIsRight(int pad, int width, int height);
    bool padIsTop(int pad, int width, int height);
    bool padIsBottom(int pad, int width, int height);

    void setIntegralSame(TH1 *histo1, TH1 *histo2);
    void setMaximumSame(TH1 *histo1, TH1 *histo2);

    void split(const std::string &s, char delim, std::vector<std::string> &elems);
    std::vector<std::string> splitStringOnDelimeter(const std::string &s, char delim);

    double getTitleCenterPosition(std::string title); 

  };
}

#endif
