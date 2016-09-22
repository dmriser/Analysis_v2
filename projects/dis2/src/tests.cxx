#include "BaseDISHistograms.h"
#include "DataLoader.h"
#include "HistogramLoader.h"
#include "MCLoader.h"
#include "PhysicsEventCut.h"
#include "PhysicsEventSelector.h"

void testBaseHistograms();
void testHistogramBuilder();
void testDataHistogramBuilder();
void testMCHistogramBuilder();

int main(){

  //  testBaseHistograms();
  //  testHistogramBuilder();
  testDataHistogramBuilder();
  testMCHistogramBuilder(); 
  
  return 0; 
}

void testBaseHistograms(){
  BaseDISHistograms dataEvents;
  dataEvents.Init("dataEvents","Data Events ");
  dataEvents.Save("out/test.root","RECREATE");
}

void testHistogramBuilder(){

  PhysicsEventSelector *sel = new PhysicsEventSelector();
  PhysicsEventCut_w * w_cut = new PhysicsEventCut_w();
  w_cut->set_min(2.05);
  w_cut->set_max(4.00);
  sel->add_cut(w_cut); 
  sel->enable_all();
  
  HistogramLoader testLoader(sel, "out/testLoader.root", "RECREATE");  
  testLoader.Execute();
  
}

void testDataHistogramBuilder(){

  PhysicsEventSelector *sel = new PhysicsEventSelector();
  PhysicsEventCut_w * w_cut = new PhysicsEventCut_w();
  w_cut->set_min(2.05);
  w_cut->set_max(400.00);
  sel->add_cut(w_cut); 
  sel->enable_all();
  
  DataLoader testLoader(sel, "out/testBothLoader.root", "RECREATE");  
  testLoader.AddFile("/Users/dmriser/Data/clas_038534.A01.root");
  testLoader.Execute();
  
}

void testMCHistogramBuilder(){

  PhysicsEventSelector *sel = new PhysicsEventSelector();
  PhysicsEventCut_w * w_cut = new PhysicsEventCut_w();
  w_cut->set_min(2.05);
  w_cut->set_max(400.00);
  sel->add_cut(w_cut); 
  sel->enable_all();
  
  MCLoader testLoader(sel, "out/testBothLoader.root", "UPDATE");  
  testLoader.AddFile("/Users/dmriser/Data/mc/dis/ftp/dis_pass0.2510.root");
  testLoader.Execute();
  
}

