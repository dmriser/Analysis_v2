#ifndef plotting_h 
#define plotting_h 

#include <iostream>
#include <vector>
#include <map>

// this project 
#include "PhiFits.h"
#include "PhiHistograms.h"
#include "MissingMassHistograms.h"
#include "Config.h"
#include "ConfigLoader.h"
#include "ConfigLoader.cxx"
#include "Types.h"

// h22 libraries 
#include "DBins.h"

// root 
#include "TCanvas.h"

class Plotting {

public:

  static void PlotOnePhiHisto(PhiHistos *h, 
			      std::string name, 
			      std::string pdf){

    // retrieve binning from histograms 
    std::pair<int, DLineBins> bins = h->GetBins();
    
    // setup a reasonably sized canvas 
    TCanvas *canvas = new TCanvas("canvas","",1600,1200);
    canvas->Divide(4,4);
    canvas->Print(Form("%s[",pdf.c_str()));

    int index = 1;

    for(int b=0; b<bins[axis::x].GetNumber(); b++){
      canvas->cd(index);

      h->histos[axis::x][b]->Draw();
      index++; 
      
      // we need to print a page 
      if (index == 17){
	index = 1; 
	canvas->Print(pdf.c_str());
      }
    }

    canvas->Print(Form("%s]",pdf.c_str()));
  }

};

#endif 
