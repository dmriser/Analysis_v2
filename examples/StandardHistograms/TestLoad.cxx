
// ----------------------------------
//    c++
// ----------------------------------
#include <iostream>
using namespace std; 

// ----------------------------------
//     My Libs 
// ----------------------------------
#include "StandardHistograms.h"

// ----------------------------------
//     ROOT
// ----------------------------------
#include "TH1.h"
#include "TH2.h"

int main(int argc, char *argv[]){

  StandardHistograms h("testHistos",1);
  h.Load("out.root");

  for(int s=0; s<7; s++){
    cout << " I found something that has entries = " << h.h1_ele_relPhi[s]->GetEntries() << " with name = " << h.h1_ele_relPhi[s]->GetName() << endl; 
  }

  return 0;
}
