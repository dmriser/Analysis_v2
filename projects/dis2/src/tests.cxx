#include "DInformation.h"
#include "MomCorr.h"
#include "PhysicsEventCut.h"
#include "PhysicsEventSelector.h"
#include "TLorentzVector.h"

#include "BaseDISHistograms.h"
#include "DataLoader.h"
#include "FaradayCupLoader.h"
#include "HistogramLoader.h"
#include "MCLoader.h"

#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::string;
using std::vector;

void testBaseHistograms();
void testHistogramBuilder();
void testDataHistogramBuilder();
void testMCHistogramBuilder();
void testFaradayCupLoader();


int main(){

  //  testBaseHistograms();
  //  testHistogramBuilder();
  //  testDataHistogramBuilder();
  //  testMCHistogramBuilder(); 
  testFaradayCupLoader();
  
  return 0; 
}

void testBaseHistograms(){
  BaseDISHistograms dataEvents;
  dataEvents.Init("dataEvents","Data Events ");
  dataEvents.Save("../out/test.root","RECREATE");
}

void testHistogramBuilder(){

  PhysicsEventSelector *sel = new PhysicsEventSelector();
  PhysicsEventCut_w * w_cut = new PhysicsEventCut_w();
  w_cut->set_min(2.05);
  w_cut->set_max(4.00);
  sel->add_cut(w_cut); 
  sel->enable_all();
  
  HistogramLoader testLoader(sel, "../out/testLoader.root", "RECREATE");  
  testLoader.Execute();
  
}

void testDataHistogramBuilder(){

  PhysicsEventSelector *sel = new PhysicsEventSelector();
  PhysicsEventCut_w * w_cut = new PhysicsEventCut_w();
  w_cut->set_min(2.05);
  w_cut->set_max(400.00);
  sel->add_cut(w_cut); 
  sel->enable_all();

  MomCorr_e1f * momCorr = new MomCorr_e1f("/Users/dmriser/Work/analysis/momCorr/");
  
  DataLoader testLoader(sel, momCorr, "../out/testBothLoader.root", "RECREATE");  
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
  
  MCLoader testLoader(sel, "out/testBothLoader.root", "UPDATE", "NoRad");  
  testLoader.AddFile("/Users/dmriser/Data/mc/dis/ftp/dis_pass0.2510.root");
  testLoader.Execute();
  
}

void testFaradayCupLoader(){
  DInformation * thisInformation = new DInformation();
  thisInformation->load("/Users/dmriser/Work/analysis/lists/runs.info"); 

  vector<string> files; 
  files.push_back("/Users/dmriser/Data/clas_038534.A01.root");
  files.push_back("/Users/dmriser/Data/clas_038534.A02.root");
  files.push_back("/Users/dmriser/Data/clas_038534.A03.root"); 

  FaradayCupAppender testAppender(thisInformation);
  testAppender.AddFiles(files);
  testAppender.AppendToFile("out/testfile.root"); 
}

