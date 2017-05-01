#include <iostream>
#include <vector>

#include "SliceFitter.h"

#include "TF1.h"
#include "TF2.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TRandom3.h"

int main(int argc, char *argv[]){

  SliceFitter slicer(20, 0.2, 0.8);
  slicer.SetExpectedMean("0.68"); 
  slicer.SetLowerTolerance(0.94);
  slicer.SetUpperTolerance(1.04); 
  
  TFile *out = new TFile("out.root","recreate"); 
  
  TH2D *h = new TH2D("h","h",100,0.0,1.0,
		     100,0.0,1.0); 
  TRandom3 rand; 
  
  for (int i=0; i<50000; i++){
    h->Fill(rand.Uniform(), rand.Gaus(0.68, 0.01)); 
  }

  for (int i=0; i<200000; i++){
    double x = rand.Uniform(); 
    h->Fill(x, rand.Gaus(0.3, 0.01) + 0.1/x); 
  }

  slicer.Fit(h, "test"); 

  std::vector<TH1D> slices = slicer.GetSlices(); 
  std::vector<TF1> fits    = slicer.GetFits();

  TGraphErrors mu    = slicer.GetGraphMu("test_graph");
  TGraphErrors sigma = slicer.GetGraphSigma("test_graph"); 

  for (TH1D s : slices){
    s.Write(); 
  }
  
  for (TF1 fit : fits){
    fit.Write(); 
  }

  mu.Write();
  sigma.Write(); 
  
  h->Write();
  out->Close(); 
  
  return 0; 
}
