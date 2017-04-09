#include <iostream>
using std::cout; 
using std::endl; 

#include "h22Option.h"
#include "GenericAnalysis.h"
#include "TString.h"

class MyAnalysis : public GenericAnalysis {
public:
  MyAnalysis(h22Options *opts) : GenericAnalysis(opts) { } 
  ~MyAnalysis(){}

  void ProcessEvent();
  void Initialize();
  void Print(){
    cout << " Events Processed: " << nEventsProcessed << endl; 
    cout << " eN+: " << nNeutralPositive << endl; 
    cout << " e2N: " << n2Neutral << endl; 
    cout << " e+: " << nPositive << endl; 
    cout << " e-: " << nNegative << endl; 
  }
  
protected:
  int nEventsProcessed; 
  int nNeutralPositive; 
  int n2Neutral;
  int nPositive; 
  int nNegative; 
};


void MyAnalysis::ProcessEvent(){
  
  int nPos = 0;
  int nNeg = 0;
  int nNeu = 0; 

  if(event.id[0] == 11){
    for(int ipart=1; ipart<event.gpart; ipart++){
      if (event.q[ipart] == -1){
	nNeg++; 
      } else if (event.q[ipart] == 0){ 
	nNeu++; 
      } else if (event.q[ipart] == 1){
	nPos++; 
      }
    }

    if (nPos > 0){
      nPositive++; 
    }

    if (nNeg > 0){
      nNegative++; 
    }

    if (nNeu > 1){
      n2Neutral++; 
    }

    if (nNeu > 0 && nPos > 0){
      nNeutralPositive++; 
    }

  }

  nEventsProcessed++;
}

void MyAnalysis::Initialize(){
  nEventsProcessed = 0; 
  nNeutralPositive = 0; 
  n2Neutral = 0;
  nPositive = 0; 
  nNegative = 0; 
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
  analysis.Print(); 

  return 0;
}
