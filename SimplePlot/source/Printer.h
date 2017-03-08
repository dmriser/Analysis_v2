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
    TLatex              tit;
    TLatex              xtit;
    TLatex              ytit;

    std::string         path; 
    std::string         outputTitle; 
    std::string         currentStyle; 

    Tools               tools; 
    Style              *style; 
    
    void setupCommonOptions();

  public:
    void applyStylingOptions(TH1 *histo, int index); //! Index refers to primary or secondary 
    void print(TH1 *histo, std::string title, std::string xTitle, std::string yTitle);
    void printCompare(TH1 *histo1, TH1 *histo2, std::string title, std::string xTitle, std::string yTitle);
    void printSectors(TH1D *histo[], std::string title, std::string xTitle, std::string yTitle);
    void printSectorsCompare(TH1D *histo1[], TH1D *histo2[], std::string title, std::string xTitle, std::string yTitle);
    void printSectorsCompareWithRatio(TH1D *histo1[], TH1D *histo2[], std::string title, std::string xTitle, std::string yTitle);
    void printSectorsCompare(TH1D *histo1[], TH1D *histo2, std::string title, std::string xTitle, std::string yTitle);
    void printBins(std::vector<TH1D*> histos, std::string title, std::string xTitle, std::string yTitle);
    void printBinsCompare(std::vector<TH1D*> histos1, std::vector<TH1D*> histos2, std::string title, std::string xTitle, std::string yTitle);
    void printBinsCompare(std::vector<TH1D*> histos1, TH1D *histo2, std::string title, std::string xTitle, std::string yTitle);


    void setStyle(std::string styleChoice);
    void setPath(std::string p)       { path=p; }
    void setOutputTitle(std::string t){ outputTitle=t; }

    std::string getPath()       { return path; }
    std::string getOutputTitle(){ return outputTitle; }



  };

}

#endif
