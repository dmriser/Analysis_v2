#include "THnSparse.h"
#include "TH1.h"
#include "TH2.h"
#include "TControlBar.h"

void GUI(){

  string inputFile = "rebuildJobs4Gen.root";

  // Getting file and events 
  TFile      *file   = TFile::Open(inputFile.c_str());
  THnSparseI *events = (THnSparseI*) file->Get("events");

  // Create histograms 
  x     = (TH1I*) events->Projection(0); x     ->SetName("x"); 
  y     = (TH1I*) events->Projection(1); y     ->SetName("y"); 
  p     = (TH1I*) events->Projection(2); p     ->SetName("p"); 
  w     = (TH1I*) events->Projection(3); w     ->SetName("w"); 
  qq    = (TH1I*) events->Projection(4); qq    ->SetName("qq"); 
  theta = (TH1I*) events->Projection(5); theta ->SetName("theta"); 
  phi   = (TH1I*) events->Projection(6); phi   ->SetName("phi"); 
  
  thetaPhi = (TH2I*) events->Projection(5,6); thetaPhi->SetName("thetaPhi");
  qqX      = (TH2I*) events->Projection(4,0); qqX     ->SetName("qqX");
  qqW      = (TH2I*) events->Projection(4,3); qqW     ->SetName("qqW");
    

  gROOT->LoadMacro("utils.C");
  //  gStyle->SetPalette(62);

  // Build and show GUI
  TControlBar *bar = new TControlBar("vertical","Event Monitor by David Riser");
  bar->AddButton("","");
  bar->AddButton("show x",     "draw(x,     0)");
  bar->AddButton("show y",     "draw(y,     1)");
  bar->AddButton("show p",     "draw(p,     2)");
  bar->AddButton("show w",     "draw(w,     3)");
  bar->AddButton("show Q2",    "draw(qq,    4)");
  bar->AddButton("show theta", "draw(theta, 5)");
  bar->AddButton("show phi",   "draw(phi,   6)");
  bar->AddButton("","");
  bar->AddButton("show theta vs. phi", "draw(thetaPhi, 0)");
  bar->AddButton("show Q2 vs. x",      "draw(qqX,      1)");
  bar->AddButton("show Q2 vs. w",      "draw(qqW,      2)");
  bar->AddButton("","");
  bar->AddButton("Quit",".q");
  bar->Show();

}
