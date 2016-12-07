#include <iostream>
using std::cout; 
using std::endl; 

#include "GenericAnalysis.h"

#include "TString.h"

class MyAnalysis : public GenericAnalysis {
public:
  MyAnalysis(){} 
  ~MyAnalysis(){}

  void ProcessEvent();
  
};

// Example overwriting the event action 
void MyAnalysis::ProcessEvent(){
  //  event.printEvent(); 
}

int main(int argc, char *argv[]){

  if (argc < 2){
    cout << " No files ? " << endl; 
    return 0;
  }

  MyAnalysis analysis;

  for (int i=1; i<argc; i++) { analysis.AddFile(argv[i]); } 
  analysis.RunAnalysis(1e6);

  return 0;
}
