
// ----------------------------------
//    c++
// ----------------------------------
#include <iostream>

// ----------------------------------
//     My Libs 
// ----------------------------------
#include "MesonHistograms.h"

// ----------------------------------
//     ROOT
// ----------------------------------
#include "TH1.h"
#include "TH2.h"

int main(int argc, char *argv[]){

  MesonHistograms h("testPosHistos", 321);
  h.Load("kaon_all.root");
  std::cout << h.h1_dvz[0]->GetEntries() << std::endl; 

  return 0;
}
