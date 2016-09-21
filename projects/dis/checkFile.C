#include "TROOT.h"
#include "TControlBar.h"

void checkFile(){

  gROOT->LoadMacro("acceptance.C");
  gROOT->LoadMacro("purity.C");
  gROOT->LoadMacro("stability.C"); 

  string inputFilename = "pass2v7.root";
  
  TControlBar *bar = new TControlBar("vertical"); 
  bar->AddButton("Show Purity",Form("purity(\"%s\")",inputFilename.c_str()));
  bar->AddButton(" "," ");
  bar->AddButton("Show Stability",Form("stability(\"%s\")",inputFilename.c_str()));
  bar->AddButton(" "," ");
  bar->AddButton("Show Acceptance",Form("acceptance(\"%s\")",inputFilename.c_str()));
  bar->AddButton(" "," ");
  bar->AddButton("Exit",".q");
  bar->Show(); 

  //  gROOT->SaveContext();
  
  
}
