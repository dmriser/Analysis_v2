#include <iostream>
#include <vector>
#include <map>
using namespace std;

// My Includes 
#include "DCut.h"
#include "DEvent.h"
#include "DSelection.h"
#include "ElectronIDPackage.h"
#include "h22Reader.h"
#include "h22Event.h"


// This Analysis 
class SampleAnalysis : public h22Reader
{
public:
  SampleAnalysis();
  ~SampleAnalysis();

public:
  void loop();
    
};

SampleAnalysis::SampleAnalysis() : h22Reader(0)
{
  // Nothing to do.
  Init();
}

SampleAnalysis::~SampleAnalysis()
{
  // Nothing to do. 
}

void SampleAnalysis::loop()
{

  ElectronSelector sel("epars.dat"); 
  //  sel.set_runno( runno() );
  sel.set_mc_status( false );

  cout << " Selector created. " << endl;

  int n_electrons = 0;
  
  // Loop over events. 
  for (int ievent=0; ievent< fchain->GetEntries(); ievent++)
    {
      GetEntry(ievent);
      DEvent event( GetEvent() );
      if (sel.passes(event, 0)) n_electrons++; // Increments the counters 
      
    }

  sel.summarize();
  cout << " Total electrons: " << n_electrons << endl; 
  
}



int main(int argc, char * argv[])
{
  
  SampleAnalysis analysis;
  
  for (int iarg=1; iarg<argc; iarg++) { analysis.AddFile(argv[iarg]); } 

  analysis.loop();
  
  return 0;
}
