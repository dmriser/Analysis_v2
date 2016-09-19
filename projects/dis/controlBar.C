#include "TROOT.h"
#include "TControlBar.h"

void controlBar(){

  gROOT->LoadMacro("purity.C");
  gROOT->LoadMacro("stability.C"); 

  string inputFilename = "thisTestFile.root";
  
  TControlBar *bar = new TControlBar("vertical"); 
  bar->AddButton("Show Purity",Form("purity(\"%s\")",inputFilename.c_str()));
  bar->AddButton(" "," ");
  bar->AddButton("Show Stability",Form("stability(\"%s\")",inputFilename.c_str()));
  bar->AddButton(" "," ");
  bar->AddButton("Exit",".q");
  bar->Show(); 

  //  gROOT->SaveContext();
  
  
}
