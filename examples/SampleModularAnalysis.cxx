#include <iostream>
#include <vector>
using namespace std;

// My Includes 
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

  // Loop over events. 
  for (int ievent=0; ievent< fchain->GetEntries(); ievent++)
    {
      GetEntry(ievent);
      h22Event event = GetEvent();
      cout << event.q[0] << " ";
    }
  
}

int main(int argc, char * argv[])
{
  
  SampleAnalysis analysis;

  for (int iarg=1; iarg<argc; iarg++) { analysis.AddFile(argv[iarg]); } 

  analysis.loop();
  
  return 0;
}
