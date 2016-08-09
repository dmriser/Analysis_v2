////////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  August 9, 2016 

  Dumper.cxx ->  Dumping classes
  used for rapid debugging. 

*/
////////////////////////////////////////////

#ifndef dumper_cxx
#define dumper_cxx

// c++ includes 
#include <iostream>
using std::cout;
using std::endl;

// root includes
#include "TH1.h"

// my includes
#include "Dumper.h"

HistogramDumper::HistogramDumper()
{
    pWidth = 100; pHeight = 30;
}

HistogramDumper::~HistogramDumper()
{   // Destroy things.
}

void HistogramDumper::dump(TH1F * pHistogram)
{
    TH1F * histo = new TH1F( *pHistogram );

    // Handle Binning
    int xbins = histo->GetNbinsX();
    
    if (xbins < pWidth) { pWidth = xbins; }
    
    // Do A Rebin
    while (xbins > pWidth)
    {
        histo->Rebin(2);
        xbins = histo->GetNbinsX();
    }
    
    // The histogram is now pWidth wide
    pWidth = xbins;

    
    // Find Maximum
    double max = histo->GetMaximum();
    y_bin_size = (double) max/pHeight;

    // Print histogram name 
    cout << endl; 
    for (int ix=0; ix< (pWidth/2); ix++) { cout << " " ;}
    cout << histo->GetTitle() << " with " << histo->GetEntries() << " entries. " << endl; 

    // Printing the entries and the y-axis 
    for (int iy=pHeight; iy>=0; iy--)
    {
      if (iy%5 == 0) { cout.width(12); cout << (int) y_bin_size*iy << " |"; }
      else { cout.width(14); cout << "    |"; }

      for (int ix=0; ix<=pWidth; ix++)
        {
            int ybins = (int) histo->GetBinContent(ix+1)/y_bin_size;

            if (ybins >= iy){ cout << "#"; }
            else cout << " ";
        }
        cout << endl;
    }

    // Printing x-axis and bar 
    cout.width(12); cout << ""; 
    for (int ix=0; ix<=pWidth; ix++) { cout << "_"; }
    cout << endl;

    cout.width(12); cout << ""; 
    for (int ix=0; ix<=(pWidth+1); ix++) { if (ix%8 == 0) { cout.width(4); cout << (double) histo->GetBinLowEdge(ix+1); ix+=4; } else { cout << " "; } }
    cout << endl;
    cout << endl; 
    
}

#endif
