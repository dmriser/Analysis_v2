#ifndef printer_h
#define printer_h

// -----------------------
//     c++
// -----------------------
#include <iostream>
#include <vector>

// -----------------------
//   root
// -----------------------
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TStyle.h"

// -----------------------
//   this
// -----------------------
#include "Style.h"
#include "Tools.h"

namespace SimplePlot{
  class Printer{
  public:
    Printer();
    ~Printer();
    
  protected:
    TCanvas            *canvas;
    std::string         path; 
    std::string         outputTitle; 
    std::string         currentStyle; 

    Tools               tools; 
    Style              *style; 
    
    void setupCommonOptions();

  public:
    void print(TH1 *histo, std::string title, std::string xTitle, std::string yTitle);
    void printCompare(TH1 *histo1, TH1 *histo2, std::string title, std::string xTitle, std::string yTitle);
    void printSectors(TH1D *histo[], std::string title, std::string xTitle, std::string yTitle);
    void printSectorsCompare(TH1D *histo1[], TH1D *histo2[], std::string title, std::string xTitle, std::string yTitle);

    void setStyle(std::string styleChoice);
    void setPath(std::string p)       { path=p; }
    void setOutputTitle(std::string t){ outputTitle=t; }

    std::string getPath()       { return path; }
    std::string getOutputTitle(){ return outputTitle; }



  };

}

#endif
