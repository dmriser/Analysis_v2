#ifndef kinematic_tree_cxx
#define kinematic_tree_cxx

#include <iostream>
using std::cout; 
using std::endl; 

#include "KinematicTree.h"

#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"

#include "TTree.h"
#include "TBranch.h"

KinematicTree::KinematicTree(h22Options *opts, Parameters *p) : GenericAnalysis(opts), fPars(p) {
  isMonteCarlo = false; 
  fFilter = new ParticleFilter(fPars); 
}

KinematicTree::~KinematicTree(){
}

void KinematicTree::SetupTree(){

  // Create the structure of our kinematic tree 
  // and set the memory addresses of local vars.
  kinematicTree = new TTree("kinematicTree","Tree that holds kinematics");
  kinematicTree->SetDirectory(0);

  b_qq       = kinematicTree->Branch("qq",       &qq);
  b_x        = kinematicTree->Branch("x",        &x);
  b_y        = kinematicTree->Branch("y",        &y);
  b_w        = kinematicTree->Branch("w",        &w);
  b_nu       = kinematicTree->Branch("nu",       &nu);
  b_electron = kinematicTree->Branch("electron", &electron);
  b_meson    = kinematicTree->Branch("meson", &meson);

}

void KinematicTree::ProcessEvent(){

  std::vector<int> electrons = fFilter->getVectorOfParticleIndices(event, 11); 
  std::vector<int> mesons    = fFilter->getVectorOfParticleIndices(event, 211); 

  bool eventIsGood = (!electrons.empty() && !mesons.empty()); 

    // Process
    if (eventIsGood){    
      int electronIndex = electrons[0]; 
      int mesonIndex    = mesons[0];

      electron = event.GetTLorentzVector(electronIndex, 11); 
      meson    = event.GetTLorentzVector(mesonIndex, 211); 

      PhysicsEvent ev = builder.getPhysicsEvent(electron, meson);

      x  = ev.x; 
      y  = ev.y; 
      qq = ev.qq; 
      w  = ev.w;  
      nu = ev.nu; 

      kinematicTree->Fill(); 
    }
}

void KinematicTree::Save(string outputFilename){

  TFile *outputFile = new TFile(outputFilename.c_str(), "recreate");
  kinematicTree->Write(); 
  outputFile   ->Close();

}

#endif
