#include <iostream>
#include <vector>
#include <map>
using namespace std;

// My Includes 
#include "DCut.h"
#include "DEvent.h"
#include "DSelection.h"
#include "h22Reader.h"
#include "h22Event.h"


class SampleSelection : public DSelection 
{
public:
  SampleSelection();
  ~SampleSelection();
};

SampleSelection::SampleSelection()
{
  set_name(" Sample Selection ");


  ChargeCut * negative = new ChargeCut();  
  negative->set_max(0);
  negative->set_min(-2);
  add_cut( negative );

  MomentumCut * hi_mom = new MomentumCut();  
  hi_mom->set_max(10);
  hi_mom->set_min(4);
  add_cut( hi_mom );

  ECUCut * ecu = new ECUCut();
  ecu->set_max(450);
  ecu->set_min(50);
  add_cut( ecu );

  ECUCut * ecu2 = new ECUCut();
  ecu2->set_max(300);
  ecu2->set_min(50);
  ecu2->set_name("EC-U Tighter Cut");
  add_cut( ecu2 );

  ECVCut * ecv = new ECVCut();
  ecv->set_max(60);
  ecv->set_min(40);
  ecv->disable();
  add_cut( ecv );

  ECWCut * ecw = new ECWCut();
  ecw->set_max(100);
  ecw->set_min(0);
  ecw->disable();
  add_cut( ecw );

  SampFracCut * sf_cut = new SampFracCut();
  sf_cut->set_max(0.4);
  sf_cut->set_min(0.05);
  add_cut( sf_cut );
  
  WCut * w_cut = new WCut();
  w_cut->set_max(1.1);
  w_cut->set_min(0.8);
  add_cut( w_cut );

  // DSelection::enable_all() loops over the cuts and turns them all on
  // by calling DCut::enable()
  enable_all();
}

SampleSelection::~SampleSelection()
{

}

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

  SampleSelection selection; 
  
  // Loop over events. 
  for (int ievent=0; ievent< fchain->GetEntries(); ievent++)
    {
      GetEntry(ievent);
      DEvent event( GetEvent() );
      event.set_e_index(0);
      event.print();      
      selection.passes(event, 0);

    }

  selection.summarize();
  
}



int main(int argc, char * argv[])
{
  
  SampleAnalysis analysis;
  
  for (int iarg=1; iarg<argc; iarg++) { analysis.AddFile(argv[iarg]); } 

  analysis.loop();
  
  return 0;
}
