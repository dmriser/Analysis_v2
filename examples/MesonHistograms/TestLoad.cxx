
// ----------------------------------
//    c++
// ----------------------------------
#include <iostream>
using namespace std; 

// ----------------------------------
//     My Libs 
// ----------------------------------
#include "StandardHistograms.h"
#include "PhotonHistograms.h"

// ----------------------------------
//     ROOT
// ----------------------------------
#include "TH1.h"
#include "TH2.h"

int main(int argc, char *argv[]){

  StandardHistograms h("testHistos",1);
  h.Load("out.root");

  PhotonHistograms ph("testPhotonHistos");
  ph.Load("out.root");

  for(int s=0; s<7; s++){
    cout << " I found something that has entries = " << h.h1_ele_relPhi[s]->GetEntries() << " with name = " << h.h1_ele_relPhi[s]->GetName() << endl; 
  }

  for(int s=0; s<7; s++){
    cout << " I found something that has entries = " << ph.h1_p[s]->GetEntries() << " with name = " << ph.h1_p[s]->GetName() << endl; 
  }

  return 0;
}
