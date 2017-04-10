#include <iostream>
using std::cout; 
using std::endl; 

#include "h22Event.h"
#include "h22ElectronEvent.h"
#include "h22Option.h"
#include "GenericAnalysis.h"
#include "TString.h"

class MyAnalysis : public GenericAnalysis {
public:
  MyAnalysis(h22Options *opts) : GenericAnalysis(opts) { } 
  ~MyAnalysis(){}

  void ProcessEvent();
  void Initialize();
  
protected:

};


void MyAnalysis::ProcessEvent(){

  h22Event copyEvent(event);
  h22ElectronEvent eleEvent(event);

  eleEvent.SetElectronIndex(0); 
  eleEvent.SetCorrectedStatus(false); 
  eleEvent.SetStartTime(0.0); 
}

void MyAnalysis::Initialize(){
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
