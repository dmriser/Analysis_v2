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

KinematicTree::KinematicTree(){
  isMonteCarlo = false; 
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

}

void KinematicTree::ProcessEvent(){

  // We can access the event directly because GenericAnalysis 
  // inherits from h22Reader which contains event. 
  int electronIndex     = -1; 
  bool eventHasElectron = false;

  if (!isMonteCarlo){
    for (int ipart=0; ipart<event.gpart; ipart++){
      // This is one way of getting electrons 
      // from data, but not the best way. 
      
      if (event.id[ipart] == 11){
      electronIndex    = ipart; 
      eventHasElectron = true; 
      break;
      }
    }

    // Process
    if (eventHasElectron){    
      electron = TLorentzVector(event.cx[electronIndex]*event.p[electronIndex], 
				event.cy[electronIndex]*event.p[electronIndex],
				event.cz[electronIndex]*event.p[electronIndex],
				event.p[electronIndex]);
    }
  }

  else {
    electron         = event.GetGeneratedParticle(11);
    eventHasElectron = true; 

    cout << "Electron set from MC pz=" << electron.Pz() << endl;
  }

  if (eventHasElectron){
    
    PhysicsEvent physicsEvent = builder.getPhysicsEvent(electron); 
    
    x  = physicsEvent.x; 
    y  = physicsEvent.y; 
    qq = physicsEvent.qq; 
    nu = physicsEvent.nu; 
    w  = physicsEvent.w; 

    cout.width(14); cout << x; 
    cout.width(14); cout << y; 
    cout.width(14); cout << qq; 
    cout.width(14); cout << nu; 
    cout.width(14); cout << w << endl; 

    // Add this entry to the output.
    kinematicTree->Fill();
  }
}

void KinematicTree::Save(string outputFilename){

  TFile *outputFile = new TFile(outputFilename.c_str(), "recreate");
  kinematicTree->Write(); 
  outputFile   ->Close();

}

#endif
