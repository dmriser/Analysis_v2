#include <iostream>
using std::cout; 
using std::endl; 

#include "h22Option.h"
#include "GenericAnalysis.h"
#include "TH1.h"
#include "TString.h"

class MyAnalysis : public GenericAnalysis {
public:
  MyAnalysis(h22Options *opts) : GenericAnalysis(opts) { } 
  ~MyAnalysis(){}

protected:
  TH1D *momentum; 

public:

  void ProcessEvent();
  void Initialize();
  
};


void MyAnalysis::ProcessEvent(){
  momentum->Fill(event.p[0]); 
}

void MyAnalysis::Initialize(){
  momentum = new TH1D("momentum","",100,0.5,5.5); 
  rootObjects.push_back(momentum); 
}

int main(int argc, char *argv[]){

  if (argc < 2){
    cout << " No files ? " << endl; 
    return 0;
  }

  h22Options *opts = new h22Options();
  opts->set(argc, argv); 

  MyAnalysis analysis(opts);

  for (int i=0; i<opts->ifiles.size(); i++) { analysis.AddFile(opts->ifiles[i]); } 
  analysis.RunAnalysis();

  return 0;
}
