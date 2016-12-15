#ifndef kinematic_tree_h
#define kinematic_tree_h

#include <iostream>

#include "GenericAnalysis.h"

#include "PhysicsEventBuilder.h"

#include "TBranch.h"
#include "TLorentzVector.h"
#include "TTree.h"

class KinematicTree : public GenericAnalysis {
 public:
  KinematicTree();
  ~KinematicTree();

  // Structure of output tree
  TTree *kinematicTree;
  TBranch *b_qq;
  TBranch *b_x;
  TBranch *b_y;
  TBranch *b_w;
  TBranch *b_nu;
  TBranch *b_electron;

  Double_t qq; 
  Double_t x; 
  Double_t y; 
  Double_t w; 
  Double_t nu;
  TLorentzVector electron; 

  bool isMonteCarlo;

  PhysicsEventBuilder builder;

 public:
  void SetupTree();
  void ProcessEvent();
  void Save(std::string outputFilename);

};

#endif
