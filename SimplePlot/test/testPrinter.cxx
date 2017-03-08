// ------------------------
//    c++
// ------------------------
#include <iostream>
#include <vector>
using namespace std;

// ------------------------
//    root
// ------------------------
#include "TH1.h"
#include "TRandom3.h"

// ------------------------
//    this
// ------------------------
#include "Printer.h"

int main(int argc, char *argv[]){

  SimplePlot::Printer p;
  
  p.setPath("image/");
  p.setOutputTitle("testImage");

  TRandom3 rand; 
  TH1D *h1 = new TH1D("h1","",100,-3,3); 
  TH1D *h2 = new TH1D("h2","",100,-3,3); 
  TH1D *sect1[6]; 
  TH1D *sect2[6]; 
  double sigma1[6];
  double sigma2[6];

  vector<TH1D*> bins; 

  for(int s=0; s<6; s++){
    sect1[s]  = new TH1D(Form("s%d_1",s),"",100,-6,6); 
    sect2[s]  = new TH1D(Form("s%d_2",s),"",100,-6,6); 
    sigma1[s] = rand.Gaus(1.0,0.3);
    sigma2[s] = rand.Gaus(1.0,0.3);
  }

  for(int b=0; b<12; b++){
    TH1D *h = new TH1D(Form("bin%d",b),"",100,-4,4); 
    bins.push_back(h);
  }

  for(int i=0; i<10000; i++){
    h1->Fill(rand.Gaus(0.0,1.0));
    h2->Fill(rand.Gaus(0.0,1.4));

    for(int s=0; s<6; s++){
      sect1[s]->Fill(rand.Gaus(0.0,sigma1[s]));
      sect2[s]->Fill(rand.Gaus(0.0,sigma2[s]));
    }

    for(int b=0; b<bins.size(); b++){
      bins[b]->Fill(rand.Gaus(0.0,1.2));
    }
  }

  
  p.setStyle("lineMarker");
  p.setOutputTitle("testCompareMarkerLine"); 
  p.printCompare(h1, h2,"Test Compare with Lines and Markers","Gaussian Random","Events"); 

  p.setStyle("bubbleGum");
  p.print(h1, "Test Image", "Gaussian Random", "Events");
  p.setOutputTitle("testCompare"); 
  p.printCompare(h1, h2,"Test Compare","Gaussian Random","Events"); 

  p.printBins(bins, "Test Bins", "Gauss", "Counts");

  p.setOutputTitle("testSectors");
  p.printSectors(sect1, "CLAS Sectors","#theta_{#gamma #gamma}","Events");

  p.setOutputTitle("testSectorsCompareDefault");
  p.printSectorsCompare(sect1, sect2, "CLAS Sectors","#theta_{#gamma #gamma}","Events");

  p.setStyle("aqua");
  p.setOutputTitle("testSectorsCompareAqua");
  p.printSectorsCompare(sect1, sect2, "CLAS Sectors","#theta_{#gamma #gamma}","Events");

  p.setStyle("mint");
  p.setOutputTitle("testSectorsCompareMint");
  p.printSectorsCompare(sect1, sect2, "Mint Style","#theta_{#gamma #gamma}","Events");

  p.setOutputTitle("testSectorsCompareMintRatio");
  p.printSectorsCompareWithRatio(sect1, sect2, "Mint Style","#theta_{#gamma #gamma}","Events");

  p.setStyle("batman");
  p.setOutputTitle("testSectorsCompareBatman");
  p.printSectorsCompare(sect1, sect2, "CLAS Sectors With a Long Title Test","#theta_{#gamma #gamma}","Events with a Long Title Test");

  p.setStyle("fuego");
  p.setOutputTitle("testSectorsCompareFuego");
  p.printSectorsCompare(sect1, sect2, "CLAS Sectors","#theta_{#gamma #gamma}","Events");
 
  p.setStyle("bubbleGum");
  p.setOutputTitle("testSectorsCompareBubbleGum");
  p.printSectorsCompare(sect1, sect2, "Bubble Gum Style","#theta_{#gamma #gamma}","Events");

  return 0;
}
